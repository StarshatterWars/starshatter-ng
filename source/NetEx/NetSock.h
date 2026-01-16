/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network (IP) Socket
*/

#ifndef NetSock_h
#define NetSock_h

#include <cstdint>

#include "NetAddr.h"
#include "Text.h"


#define NET_MAX_TIMEOUT 1e9


class NetSock
{
public:
    static const char* TYPENAME() { return "NetSock"; }

    enum SELECT_TYPE {
        SELECT_READ  = 1,
        SELECT_WRITE = 2
    };

    NetSock(bool stream=false);
    NetSock(const NetAddr& addr, bool stream=false);
    ~NetSock();

    int      bind(const NetAddr& addr);
    int      connect(const NetAddr& addr);
    int      listen(int max_connections=5);
    NetSock* accept(NetAddr* addr=0);
    int      send(Text msg);
    Text     recv();
    int      sendto(Text msg, const NetAddr& dest);
    Text     recvfrom(NetAddr* a=0);
    int      select(SELECT_TYPE t=SELECT_READ,
                         long seconds=0, long microseconds=0);

    int      shutdown_input();
    int      shutdown_output();
    int      set_timeout(int msecs);
    int      close();

    std::uint32_t max_packet_size() const;
    bool     is_stream()       const { return stream; }
    bool     is_closed()       const { return closed; }
    int      status()          const { return stat;   }

private:
    NetSock(int sock, bool stream);

    int      s;
    bool     stream;
    bool     closed;
    int      stat;
    int      current_timeout;
};


#endif  // NetSock_h
