/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network Server Pump for HTTP Server
*/


#ifndef NetServer_h
#define NetServer_h

#include <cstdint>
#include <mutex>
#include <thread>

#include "NetAddr.h"
#include "NetGram.h"
#include "NetSock.h"
#include "List.h"

// +-------------------------------------------------------------------+

class NetServer
{
public:
    static const char* TYPENAME() { return "NetServer"; }

    NetServer(std::uint16_t port, int poolsize=1);
    virtual ~NetServer();

    int operator == (const NetServer& l) const { return addr == l.addr; }

    virtual void      Shutdown();
    virtual void      Listener();
    virtual void      Reader(int index);

    virtual Text      ProcessRequest(Text request, const NetAddr& addr);
    virtual Text      DefaultResponse();
    virtual Text      ErrorResponse();

    const NetAddr&    GetAddress() const { return addr; }
    int               GetLastError() const { return err; }

protected:
    NetAddr           addr;
    NetSock           sock;
    NetAddr           client_addr;

    int               poolsize;
    std::thread       hreader;
    std::thread*      pool;
    NetSock**         conn;
    NetAddr*          clients;
    int               err;
    bool              server_shutdown;

    std::mutex        sync;
};


#endif  // NetServer_h
