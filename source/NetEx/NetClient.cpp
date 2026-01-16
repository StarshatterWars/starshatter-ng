/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network Server Pump for HTTP Server
*/

#include "NetClient.h"

#include <chrono>
#include <cstdint>
#include <ratio>
#include <thread>

#include "NetAddr.h"
#include "NetSock.h"
#include "NetLayer.h"


NetClient::NetClient(const NetAddr& server_addr) :
    addr(server_addr),
    sock(nullptr),
    delta(std::chrono::high_resolution_clock::duration::zero()),
    time(),
    err(0)
{
}


NetClient::~NetClient()
{
    if (sock)
        delete sock;
}


bool
NetClient::Send(Text msg)
{
    if (msg.length() > 0) {
        if (sock)
            delete sock;

        sock  = new NetSock(addr, true);
        delta = std::chrono::high_resolution_clock::duration::zero();
        time  = std::chrono::high_resolution_clock::now();

        if (!sock) {
            err = ERR_NOBUFS;
            return false;
        }

        err = sock->send(msg);
        if (err < 0) {
            err = NetLayer::GetLastError();
            return false;
        }

        err = sock->shutdown_output();
        if (err < 0) {
            err = NetLayer::GetLastError();
            return false;
        }

        return true;
    }

    else {
        delete sock;
        sock = 0;
    }

    return false;
}


Text
NetClient::Recv()
{
    Text response;

    if (sock) {
        int ready = sock->select();

        while (!ready && std::chrono::high_resolution_clock::now() - time < std::chrono::seconds(2000)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            ready = sock->select();
        }

        if (ready) {
            Text msg = sock->recv();

            while (msg.length() > 0) {
                response += msg;
                msg = sock->recv();
            }

            delta = std::chrono::high_resolution_clock::now() - time;
        }

        delete sock;
        sock = 0;
    }

    return response;
}


Text
NetClient::SendRecv(Text msg)
{
    Text response;

    if (msg.length() > 0 && Send(msg)) {
        response = Recv();
    }

    return response;
}


std::uint32_t
NetClient::GetTime() const
{
    using target_duration = std::chrono::duration<std::uint32_t, std::milli>;
    return std::chrono::duration_cast<target_duration>(delta).count();
}
