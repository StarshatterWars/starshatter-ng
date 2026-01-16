/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network (IP) Socket Wrapper Implementation
*/

#include "NetSock.h"

#ifdef _WIN32
#include <winsock2.h>
using socklen_t = int;
#else
#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define WSAENETDOWN ENETDOWN
#define WSAENETRESET ENETRESET
#define WSAEINTR EINTR
#define WSAESHUTDOWN ESHUTDOWN
#define WSAECONNABORTED ECONNABORTED
#define WSAECONNRESET ECONNRESET
#define WSAETIMEDOUT ETIMEDOUT
#define WSAEWOULDBLOCK EWOULDBLOCK
#endif

#include <cstdint>
#include <cstring>

#include "NetAddr.h"
#include "NetLayer.h"
#include "Text.h"


/**
 * Server-side socket constructor
 */
NetSock::NetSock(bool str) :
    stream(str),
    closed(false),
    stat(0),
    current_timeout(9999)
{
    if (stream)
        s = ::socket(AF_INET, SOCK_STREAM, 0);
    else
        s = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}


/**
 * Server-side socket constructor
 *
 * PRIVATE: used only by the accept call to build a socket for
 * a client connection to this server
 */
NetSock::NetSock(int sock, bool str) :
    s(sock),
    stream(str),
    closed(false),
    stat(0),
    current_timeout(9999)
{
}


/**
 * Client-side socket constructor
 *
 * Will connect to server at "addr"
 */
NetSock::NetSock(const NetAddr& addr, bool str) :
    stream(str),
    closed(false),
    stat(0),
    current_timeout(9999)
{
    if (stream)
        s = ::socket(AF_INET, SOCK_STREAM, 0);
    else
        s = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    connect(addr);
}


NetSock::~NetSock()
{
    close();
}


int
NetSock::bind(const NetAddr& addr)
{
    if (closed || s == INVALID_SOCKET) return INVALID_SOCKET;
    return stat = ::bind(s, addr.GetSockAddr(), addr.GetSockAddrLength());
}


int
NetSock::connect(const NetAddr& addr)
{
    if (closed || s == INVALID_SOCKET) return INVALID_SOCKET;
    return stat = ::connect(s, addr.GetSockAddr(), addr.GetSockAddrLength());
}


int
NetSock::listen(int max_connections)
{
    if (closed || s == INVALID_SOCKET) return INVALID_SOCKET;
    return stat = ::listen(s, max_connections);
}


NetSock*
NetSock::accept(NetAddr* addr)
{
    if (closed || s == INVALID_SOCKET) return 0;

    int conn = INVALID_SOCKET;

    if (addr) {
        sockaddr a;
        socklen_t asize = sizeof(a);

        conn = ::accept(s, &a, &asize);

        if (conn != INVALID_SOCKET && asize > 0) {
            addr->SetSockAddr(&a, asize);
        }
    }
    else {
        conn = ::accept(s, 0, 0);
    }

    if (conn == INVALID_SOCKET)
        return 0;

    return new NetSock(conn, stream);
}


int
NetSock::send(Text msg)
{
    if (closed || s == INVALID_SOCKET) return INVALID_SOCKET;
    return stat = ::send(s, msg.data(), msg.length(), 0);
}


Text
NetSock::recv()
{
    if (closed || s == INVALID_SOCKET) return "";

    thread_local char rbuf[8192];
    int         rlen = -1;

    if (stream) {
        rlen = ::recv(s, rbuf, sizeof(rbuf), 0);
    }
    else {
        rlen = ::recvfrom(s, rbuf, sizeof(rbuf), 0, 0, 0);
    }

    if (rlen < 0) {
        stat = NetLayer::GetLastError();

        switch (stat) {
        case WSAENETDOWN:
        case WSAENETRESET:
        case WSAEINTR:
        case WSAESHUTDOWN:
        case WSAECONNABORTED:
        case WSAECONNRESET:
        case WSAETIMEDOUT:
                close();
                break;

        case WSAEWOULDBLOCK:
                stat = WSAEWOULDBLOCK;
                break;
        }

        return Text();
    }

    else if (rlen == 0) {
        return Text();
    }

    return Text(rbuf, rlen);
}


int
NetSock::sendto(Text msg, const NetAddr& dest)
{
    if (closed || s == INVALID_SOCKET) return INVALID_SOCKET;
    return stat = ::sendto(s, msg.data(), msg.length(),
                           0, dest.GetSockAddr(), dest.GetSockAddrLength());
}


Text
NetSock::recvfrom(NetAddr* a)
{
    if (closed || s == INVALID_SOCKET) return "";

    thread_local char rbuf[4096];
    int rlen = 0;

    if (a) {
        socklen_t addrlen = a->GetSockAddrLength();
        rlen = ::recvfrom(s, rbuf, sizeof(rbuf), 0, a->GetSockAddr(), &addrlen);
        a->InitFromSockAddr();
    }
    else {
        rlen = ::recvfrom(s, rbuf, sizeof(rbuf), 0, 0, 0);
    }

    if (rlen < 0) {
        stat = NetLayer::GetLastError();
        return Text();
    }

    else if (rlen == 0) {
        return Text();
    }

    return Text(rbuf, rlen);
}


int
NetSock::select(SELECT_TYPE t, long seconds, long microseconds)
{
    if (closed || s == INVALID_SOCKET) return INVALID_SOCKET;

    fd_set fd;
    std::memset(&fd, 0, sizeof(fd));

    FD_SET(s, &fd);
    struct timeval timeout = {seconds, microseconds};

    if (t == SELECT_READ)
        return stat = ::select(1, &fd, 0, 0, &timeout);

    else if (t == SELECT_WRITE)
        return stat = ::select(1, 0, &fd, 0, &timeout);

    else if (t == (SELECT_READ|SELECT_WRITE))
        return stat = ::select(1, &fd, &fd, 0, &timeout);

    return 0;
}


int
NetSock::set_timeout(int msecs)
{
    if (closed || s == INVALID_SOCKET) return 0;
    if (msecs == current_timeout)      return 1;

    // zero timeout means non-blocking
    if (msecs == 0) {
#ifdef _WIN32
            u_long nonblocking = 1;
            if (::ioctlsocket(s, FIONBIO, &nonblocking) == SOCKET_ERROR) {
#else
            if (::fcntl(s, F_SETFL, (fcntl(s, F_GETFL, 0) | O_NONBLOCK)) == SOCKET_ERROR) {
#endif
            stat = NetLayer::GetLastError();
            return stat;
        }
    }

    // non-zero timeout means blocking
    else {
        if (current_timeout == 0) {
#ifdef _WIN32
            u_long nonblocking = 0;   // disable non-blocking
            if (::ioctlsocket(s, FIONBIO, &nonblocking) == SOCKET_ERROR) {
#else
            if (::fcntl(s, F_SETFL, (fcntl(s, F_GETFL, 0) & ~O_NONBLOCK)) == SOCKET_ERROR) {
#endif
                stat = NetLayer::GetLastError();
                return stat;
            }
        }

        // max timeout means infinite wait
        if (msecs >= NET_MAX_TIMEOUT) {
            int maxto = 0;
            ::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*) &maxto, sizeof(maxto));
            ::setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char*) &maxto, sizeof(maxto));
        }

        // otherwise, set the timeout
        else {
            ::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*) &msecs, sizeof(msecs));
            ::setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char*) &msecs, sizeof(msecs));
        }
    }

    current_timeout = msecs;
    return 1;
}


int
NetSock::shutdown_input()
{
    if (closed || s == INVALID_SOCKET) return INVALID_SOCKET;
#ifdef _WIN32
    ::shutdown(s, SD_RECEIVE);
#else
    ::shutdown(s, SHUT_RD);
#endif
    return 0;
}


int
NetSock::shutdown_output()
{
    if (closed || s == INVALID_SOCKET) return INVALID_SOCKET;
#ifdef _WIN32
    ::shutdown(s, SD_SEND);
#else
    ::shutdown(s, SHUT_WR);
#endif
    return 0;
}


int
NetSock::close()
{
    if (s != INVALID_SOCKET && !closed) {
#ifdef _WIN32
        ::shutdown(s, SD_BOTH);
        ::closesocket(s);
#else
        ::shutdown(s, SHUT_RDWR);
        ::close(s);
#endif

        closed = true;
    }

    return 0;
}


std::uint32_t
NetSock::max_packet_size() const
{
    std::uint32_t size = 0;
    socklen_t len  = sizeof(size);

    ::getsockopt(s, SOL_SOCKET, 0x2003, (char*) &size, &len);

    return size;
}
