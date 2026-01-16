/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network Address
*/

#include "NetAddr.h"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#endif

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstring>


NetAddr::NetAddr(const char* host_name, std::uint16_t p) :
    addr(0),
    port(p)
{
    if (host_name && *host_name) {
        struct hostent* h = nullptr;

        if (std::isdigit(*host_name)) {
            auto a = inet_addr(host_name);
            h = gethostbyaddr((const char*) &a, 4, AF_INET);
        }
        else {
            h = gethostbyname(host_name);
        }

        if (h) {
            if (h->h_addr_list) {
                addr = **(std::uint32_t**) (h->h_addr_list);
            }
        }
    }

    Init();
}


NetAddr::NetAddr(std::uint32_t a, std::uint16_t p) : 
    addr(a),
    port(p)
{
    Init();
}


NetAddr::NetAddr(const NetAddr& n) :
    addr(n.addr),
    port(n.port)
{
    Init();
}


void
NetAddr::Init()
{
    std::memset(&sadr, 0, sizeof(sadr));

    sadr.sin_family      = AF_INET;
    sadr.sin_port        = ::htons(port);
    sadr.sin_addr.s_addr = addr;
}


void
NetAddr::InitFromSockAddr()
{
    addr = sadr.sin_addr.s_addr;
    port = ::ntohs(sadr.sin_port);
}


sockaddr*
NetAddr::GetSockAddr() const
{
    return (sockaddr*) &sadr;
}


std::size_t
NetAddr::GetSockAddrLength() const
{
    return sizeof(sadr);
}


void
NetAddr::SetSockAddr(sockaddr* s, int size)
{
    if (s) {
        std::memset(&sadr, 0, sizeof(sadr));

        if (size > sizeof(sadr))
            std::memcpy(&sadr, s, sizeof(sadr));
        else
            std::memcpy(&sadr, s, sizeof(sadr));

        InitFromSockAddr();
    }
}
