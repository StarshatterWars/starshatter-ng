/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network Link for Remote Player
*/

#ifndef NetLink_h
#define NetLink_h

#include <cstdint>
#include <mutex>
#include <thread>

#include "List.h"
#include "NetAddr.h"
#include "NetGram.h"
#include "NetMsg.h"
#include "NetPeer.h"
#include "NetSock.h"


class NetLink
{
public:
    static const char* TYPENAME() { return "NetLink"; }

    NetLink();
    NetLink(NetAddr& a);
    virtual ~NetLink();

    int operator == (const NetLink& that) const { return this == &that; }

    const NetAddr&    GetAddress() const { return addr;  }

    std::uint32_t     AddPeer(const char*   a, std::uint16_t p=12345);
    std::uint32_t     AddPeer(std::uint32_t a, std::uint16_t p=12345);
    std::uint32_t     AddPeer(const NetAddr& a);

    bool              SendMessage(std::uint32_t nid, void* d, int l, std::uint8_t f=0);
    bool              SendMessage(std::uint32_t nid, std::uint8_t type, const char* text, int len, std::uint8_t f=0);
    bool              SendMessage(NetMsg* msg);

    NetMsg*           GetMessage();
    NetMsg*           GetMessage(std::uint32_t netid);

    virtual void      Shutdown();
    std::uint32_t     DoSendRecv();

    std::uint32_t     GetResendInterval()     const         { return resend_time;   }
    void              SetResendInterval(std::uint32_t t)    { resend_time = t;      }
    std::uint32_t     GetTrafficInterval()    const         { return traffic_time;  }
    void              SetTrafficInterval(std::uint32_t t)   { traffic_time = t;     }

    std::uint32_t     GetPacketsSent()        const { return packets_sent;  }
    std::uint32_t     GetPacketsRecv()        const { return packets_recv;  }
    std::uint32_t     GetBytesSent()          const { return bytes_sent;    }
    std::uint32_t     GetBytesRecv()          const { return bytes_recv;    }
    std::uint32_t     GetRetries()            const { return retries;       }
    std::uint32_t     GetDrops()              const { return drops;         }
    std::uint32_t     GetLag()                const { return lag;           }

    NetPeer*          FindPeer(const NetAddr& a);
    NetPeer*          FindPeer(std::uint32_t netid);

protected:
    void              SendNetGram(NetGram* g);
    NetGram*          RecvNetGram();
    void              AckNetGram(NetGram* gram);
    void              ProcessAck(NetGram* gram);
    void              QueueNetGram(NetGram* gram);

    void              ReadPackets();
    void              SendPackets();
    void              DoRetries();

    NetAddr           addr;
    NetSock           sock;
    List<NetGram>     send_list;
    List<NetMsg>      recv_list;
    List<NetPeer>     peer_list;

    std::thread       hnet;
    bool              shutdown;
    std::mutex        sync;

    std::uint32_t     resend_time;
    std::uint32_t     traffic_time;

    std::uint32_t     packets_sent;
    std::uint32_t     packets_recv;
    std::uint32_t     bytes_sent;
    std::uint32_t     bytes_recv;
    std::uint32_t     retries;
    std::uint32_t     drops;
    std::uint32_t     lag;

    std::uint32_t     lag_samples[10];
    int               lag_index;
};


#endif  // NetLink_h
