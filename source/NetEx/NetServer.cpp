/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network Server Pump for HTTP Server
*/

#include "NetServer.h"

#include <chrono>
#include <cstdint>
#include <cstring>
#include <thread>

#include "NetHost.h"
#include "NetLayer.h"

// +-------------------------------------------------------------------+

NetServer::NetServer(std::uint16_t port, int nthreads)
    : sock(true), pool(nullptr), conn(nullptr), poolsize(nthreads), err(0),
      server_shutdown(false)
{
    NetHost host;
    addr = NetAddr(host.Address().IPAddr(), port);

    sock.bind(addr);
    sock.listen(3);

    if (poolsize < 1) poolsize = 1;

    pool = new std::thread[poolsize];
    conn = new NetSock*[poolsize];
    clients = new NetAddr[poolsize];

    if (pool && conn && clients) {
        std::memset(pool, 0, poolsize * sizeof(std::thread));
        std::memset(conn, 0, poolsize * sizeof(NetSock*));

        for (int i = 0; i < poolsize; i++) {
            pool[i] = std::thread([this, i]{ Reader(i); });
        }

        hreader = std::thread([this]{ Listener(); });
    }
}

NetServer::~NetServer()
{
    if (!server_shutdown) {
        server_shutdown = true;
        sock.close();
    }

    if (hreader.joinable()) {
        hreader.join();
    }

    if (pool && poolsize) {
        for (int i = 0; i < poolsize; i++) {
            if (pool[i].joinable()) {
                pool[i].join();
            }
            delete conn[i];
            conn[i] = nullptr;
        }

        delete [] pool;
        delete [] conn;
        delete [] clients;
    }
}

// +--------------------------------------------------------------------+

void
NetServer::Shutdown()
{
    server_shutdown = true;
}

// +--------------------------------------------------------------------+

void
NetServer::Listener()
{
    while (!server_shutdown) {
        NetSock* s = sock.accept(&client_addr);

        while (s) {
            sync.lock();

            for (int i = 0; i < poolsize; i++) {
                if (conn[i] == 0) {
                    conn[i] = s;
                    clients[i] = client_addr;
                    s = 0;
                    break;
                }
            }

            sync.unlock();

            // wait for a thread to become not busy
            if (s)
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

// +--------------------------------------------------------------------+

void
NetServer::Reader(int index)
{
    while (!server_shutdown) {
        sync.lock();
        NetSock* s = conn[index];
        sync.unlock();

        if (s) {
            const int MAX_REQUEST = 4096;
            Text request;

            /***
             *** NOT SURE WHY, BUT THIS DOESN'T WORK FOR SHIT
             ***
             *** Setting the socket timeout to 2 seconds caused it
             *** to wait for two seconds, read nothing, and give up
             *** with a WSAETIMEDOUT error.  Meanwhile, the client
             *** immediately registered a failure (during the 2 sec
             *** delay) and aborted the request.
             ***

            s->set_timeout(2000);
            Text msg = s->recv();

            while (msg.length() > 0 && request.length() < MAX_REQUEST) {
                request += msg;
                msg = s->recv();
            }

             ***/

            request = s->recv();

            if (request.length() > 0 && !s->is_closed()) {
                Text response = ProcessRequest(request, clients[index]);
                err = s->send(response);
                if (err < 0) {
                    err = NetLayer::GetLastError();
                }
            }

            sync.lock();
            delete conn[index];
            conn[index] = 0;
            sync.unlock();
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
}

// +--------------------------------------------------------------------+

Text
NetServer::ProcessRequest(Text msg, const NetAddr& addr)
{
    if (msg.indexOf("GET ") == 0)
        return DefaultResponse();

    return ErrorResponse();
}

Text
NetServer::DefaultResponse()
{
    Text response =
        "HTTP/1.0 200 OK\nServer: Generic NetServer 1.0\nMIME-Version: 1.0\nContent-type: text/html\n\n";

    response += "<html><head><title>Generic NetServer 1.0</title></head>\n\n";
    response += "<body bgcolor=\"black\" text=\"white\">\n";
    response += "<h1>Generic NetServer 1.0</h1>\n";
    response += "<p>Didn't think I could do it, did ya?\n";
    response += "</body></html>\n\n";

    return response;
}

Text
NetServer::ErrorResponse()
{
    Text response =
        "HTTP/1.0 501 Not Implemented\nServer: Generic NetServer 1.0\nMIME-Version: 1.0\nContent-type: text/html\n\n";

    response += "<html><head><title>Generic NetServer 1.0</title></head>\n\n";
    response += "<body bgcolor=\"black\" text=\"white\">\n";
    response += "<h1>Generic NetServer 1.0</h1>\n";
    response += "<p>Sorry charlie...  I'm not a magician.\n";
    response += "</body></html>\n\n";

    return response;
}
