/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    One side of a UDP net link connection
*/

#ifndef NetPeer_h
#define NetPeer_h

#include <cstdint>
#include <mutex>

#include "List.h"
#include "NetAddr.h"
#include "NetGram.h"
#include "NetMsg.h"


class NetPeer
{
public:
    static const char* TYPENAME() { return "NetPeer"; }

    enum STATUS { OK, SEND_OVERFLOW, RECV_OVERFLOW };

    NetPeer(const NetAddr& addr, std::uint32_t id);
    ~NetPeer();

    int operator == (const NetPeer& p)  const { return netid == p.netid; }

    bool              SendMessage(NetMsg* msg);
    NetMsg*           GetMessage();

    NetGram*          ComposeGram();
    bool              ReceiveGram(NetGram* g, List<NetMsg>* q=0);

    const NetAddr&    Address()         const { return addr;       }
    std::uint32_t     NetID()           const { return netid;      }
    std::uint32_t     Sequence()        const { return sequence;   }

    int               GetMaxPPS()       const { return pps;        }
    void              SetMaxPPS(int p)        { pps = p;           }
    int               GetMaxBPS()       const { return bps;        }
    void              SetMaxBPS(int b)        { bps = b;           }
    int               GetMaxQSize()     const { return max_qsize;  }
    void              SetMaxQSize(int q)      { max_qsize = q;     }

    std::uint32_t     GetChunkSize()    const         { return chunk_size; }
    void              SetChunkSize(std::uint32_t s)   { chunk_size = s;    }

    std::uint32_t     LastReceiveTime() const             { return last_recv_time; }
    void              SetLastReceiveTime(std::uint32_t t) { last_recv_time = t; }

private:
    bool              OKtoSend()        const;
    void              CheckMultiRecv(List<NetMsg>* q);

    NetAddr           addr;          // remote network address
    std::uint32_t     sequence;      // highest packet id received
    std::uint32_t     netid;         // unique id for this peer
    int               pps;           // max packets per second
    int               bps;           // max bits per second
    int               max_qsize;     // max bytes in either queue
    int               status;        // ok or error code
    std::uint32_t     chunk_size;    // size of multipart message chunk

    enum HIST { HIST_SIZE=8 };

    std::uint32_t     last_recv_time;         // time of last received packet
    std::uint32_t     hist_time[HIST_SIZE];   // history for pps check
    std::uint32_t     hist_size[HIST_SIZE];   // history for bps check
    int               hist_indx;              // index into history

    int               send_size;     // total bytes in send list
    int               recv_size;     // total bytes in recv list
    List<NetMsg>      send_list;     // queue of messages waiting to be sent
    List<NetMsg>      recv_list;     // queue of messages waiting to be read

    List<NetMsg>      multi_send_list;
    List<NetMsg>      multi_recv_list;

    std::mutex        sync;
};


#endif  // NetPeer_h
