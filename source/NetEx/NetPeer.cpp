/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    One side of a UDP net link connection
*/

#include "NetPeer.h"

#include <atomic>
#include <cstdint>
#include <cstring>
#include <mutex>

#include "List.h"
#include "NetAddr.h"
#include "NetGram.h"
#include "NetLayer.h"
#include "NetMsg.h"


const int MULTIPART_CHUNKSIZE = 232;
const int MULTIPART_HEADER    =  16;
const int UDP_HEADER_SIZE     =  34;

thread_local NetMsgMultipart multi_part_buffer;
static std::atomic<std::uint32_t> multi_msg_sequence {1};


NetPeer::NetPeer(const NetAddr& a, std::uint32_t id) :
    addr(a),
    netid(id),
    sequence(0),
    pps(0),
    bps(0),
    max_qsize(0),
    status(OK),
    hist_indx(0),
    send_size(0),
    recv_size(0),
    chunk_size(MULTIPART_CHUNKSIZE)
{
    std::memset(hist_time, 0, sizeof(hist_time));
    std::memset(hist_size, 0, sizeof(hist_size));

    last_recv_time = NetLayer::GetUTC();
}


NetPeer::~NetPeer()
{
    send_list.destroy();
    recv_list.destroy();

    multi_send_list.destroy();
    multi_recv_list.destroy();
}


bool
NetPeer::SendMessage(NetMsg* msg)
{
    if (msg) {
        if (max_qsize > 0 && msg->Length() + send_size > max_qsize) {
            status = SEND_OVERFLOW;
            delete msg;
            return false;
        }

        // simple message
        if (msg->Length() <= (int) chunk_size) {
            if (msg->IsPriority())
                send_list.insert(msg);
            else
                send_list.append(msg);

            send_size += msg->Length();
        }

        // multipart message
        else {
            List<NetMsg>* list = &send_list;

            if (msg->IsScatter())
                list = &multi_send_list;

            std::uint32_t nparts = msg->Length() / chunk_size;
            std::uint32_t extra  = msg->Length() % chunk_size;

            if (extra > 0) nparts++;

            multi_part_buffer.type   = NetMsg::MULTIPART;
            multi_part_buffer.msgid  = multi_msg_sequence++;
            multi_part_buffer.nparts = nparts;

            auto header_size = static_cast<std::uint32_t>(
                reinterpret_cast<std::uint8_t*>(&multi_part_buffer.payload) -
                reinterpret_cast<std::uint8_t*>(&multi_part_buffer));

            const std::uint8_t* p = msg->Data();

            for (std::uint32_t i = 0; i < nparts; i++) {
                multi_part_buffer.partno = i;
                NetMsg* part = 0;
                std::uint32_t   part_size = chunk_size;

                if (i == nparts-1 && extra > 0) // last partial payload
                    part_size = extra;

                std::memcpy(multi_part_buffer.payload, p, part_size);
                p += part_size;
                part = new NetMsg(msg->NetID(),
                                  &multi_part_buffer,
                                  header_size + part_size,
                                  msg->Flags());

                if (part) {
                    list->append(part);
                    send_size += part->Length();
                }
            }
        }

        return true;
    }

    return false;
}


NetMsg*
NetPeer::GetMessage()
{
    if (recv_list.size() > 0) {
        NetMsg* msg = recv_list.removeIndex(0);
        recv_size -= msg->Length();
        return msg;
    }

    return 0;
}


NetGram*
NetPeer::ComposeGram()
{
    NetGram* g = 0;

    if ((send_list.size() || multi_send_list.size()) && OKtoSend()) {
        const std::lock_guard<std::mutex> lock(sync);

        int   xmit_size = send_size;
        int   nmsg      = send_list.size();
        int   limit     = NET_GRAM_MAX_SIZE;
        bool  reliable  = false;
        bool  is_multi  = false;

        NetMsg*        multi_msg = 0;
        List<NetMsg>*  list      = &send_list;

        if (xmit_size > limit) {
            xmit_size = 0;
            nmsg      = 0;

            if (send_list.size() > 0) {

                // if there is regular traffic, and multipart traffic
                if (multi_send_list.size()) {
                    // just send one multipart message in this packet
                    multi_msg = multi_send_list.removeIndex(0);
                    limit -= multi_msg->Length();
                    reliable = true;
                    is_multi = true;
                }

                for (int i = 0; i < send_list.size(); i++) {
                    NetMsg* msg = send_list[i];

                    if (xmit_size + msg->Length() < limit) {
                        xmit_size += msg->Length();
                        nmsg++;
                    }
                    else {
                        break;
                    }
                }
            }
            else {
                // if there is only multipart traffic,
                // send as many multipart messages as will fit:
                list = &multi_send_list;
                reliable = true;
                is_multi = true;

                for (int i = 0; i < multi_send_list.size(); i++) {
                    NetMsg* msg = multi_send_list[i];

                    if (xmit_size + msg->Length() < limit) {
                        xmit_size += msg->Length();
                        nmsg++;
                    }
                    else {
                        break;
                    }
                }
            }
        }

        if (xmit_size > 0 && nmsg > 0) {
            std::uint8_t* buffer   = new std::uint8_t[xmit_size];
            std::uint8_t* p        = buffer;

            if (multi_msg) {
                if (buffer) {
                    std::memcpy(p, multi_msg->Data(), multi_msg->Length());
                    p[1] = multi_msg->Length();
                    p += multi_msg->Length();
                }
                delete multi_msg;
            }

            while (nmsg-- && p < buffer + xmit_size) {
                NetMsg* msg = list->removeIndex(0);

                if (msg) {
                    if (msg->IsReliable()) reliable = true;
                    if (buffer) {
                        std::memcpy(p, msg->Data(), msg->Length());
                        p[1] = msg->Length();
                        p += msg->Length();
                    }
                    delete msg;
                }
            }

            if (buffer) {
                Text user_data((const char*) buffer, xmit_size);
                int  retries = 0;

                if (reliable)
                    retries = 5;

                if (is_multi)
                    retries = 10;

                send_size -= xmit_size;

                hist_size[hist_indx] = xmit_size + UDP_HEADER_SIZE;
                hist_time[hist_indx] = NetLayer::GetTime();
                hist_indx++;

                if (hist_indx >= HIST_SIZE)
                    hist_indx = 0;

                g = new NetGram(addr, user_data, retries);
                delete[] buffer;
            }
        }

        // the next msg is too big to fit in a single packet
        else {
            NetMsg* m = send_list.removeIndex(0);
            send_size -= m->Length();
            delete m;
        }
    }

    return g;
}


bool
NetPeer::ReceiveGram(NetGram* g, List<NetMsg>* q)
{
    if (g) {
        if (max_qsize > 0 && recv_size + g->Size() > max_qsize) {
            status = RECV_OVERFLOW;
            delete g;
            return false;
        }

        sequence  =  g->Sequence();
        recv_size += g->Size() - NET_GRAM_HEADER_SIZE;

        // PARSE THE BLOCKS:
        std::uint8_t* p = g->UserData();

        while (p < g->Data() + g->Size()) {
            std::uint8_t block_type = p[0];
            std::uint8_t block_size = p[1];

            if (!block_type || !block_size)
                break;

            NetMsg* msg = new NetMsg(netid, p, block_size);

            if (msg) {
                if (msg->Type() < NetMsg::RESERVED) {
                    msg->SetSequence(sequence);

                    recv_list.insertSort(msg);

                    if (q)
                        q->insertSort(msg);

                    p += block_size;
                }

                else if (msg->Type() == NetMsg::MULTIPART) {
                    multi_recv_list.insertSort(msg);
                    p += block_size;

                    CheckMultiRecv(q);
                }
            }
        }

        last_recv_time = NetLayer::GetUTC();

        delete g;
        return true;
    }

    return false;
}


bool
NetPeer::OKtoSend() const
{
    if (pps || bps) {
        std::uint32_t hist_total  = 0;
        std::uint32_t hist_count  = 0;
        std::uint32_t now         = NetLayer::GetTime();
        std::uint32_t hist_oldest = now;
        std::uint32_t hist_newest = 0;

        for (int i = 0; i < HIST_SIZE; i++) {
            if (hist_size[i] > 0) {
                hist_total += hist_size[i];
                hist_count++;
            }

            if (hist_time[i] > 0) {
                if (hist_time[i] < hist_oldest)
                    hist_oldest = hist_time[i];

                if (hist_time[i] > hist_newest)
                    hist_newest = hist_time[i];
            }
        }

        if (now - hist_newest < (std::uint32_t) pps)
            return false;

        std::uint32_t delta   = now - hist_oldest;
        std::uint32_t avg_bps = hist_total / delta;

        if (bps > 0 && avg_bps > (std::uint32_t) bps)
            return false;
    }

    return true;
}


struct PacketAssembly {
    std::uint32_t msgid;
    std::uint32_t netid;
    int   nreq;
    int   nparts;
    int   nbytes;
};


void
NetPeer::CheckMultiRecv(List<NetMsg>* q)
{
    const int MAX_SIMULTANEOUS_MULTI_SEQUENCES = 8;

    PacketAssembly  assy[MAX_SIMULTANEOUS_MULTI_SEQUENCES];
    std::memset(assy, 0, sizeof(assy));

    auto header_size = static_cast<std::uint32_t>(
        reinterpret_cast<std::uint8_t*>(&multi_part_buffer.payload) -
        reinterpret_cast<std::uint8_t*>(&multi_part_buffer));

    // Catalog how much of each multipart sequence has been received:
    for (int i = 0; i < multi_recv_list.size(); i++) {
        NetMsg*           msg = multi_recv_list[i];
        NetMsgMultipart*  m   = (NetMsgMultipart*) msg->Data();

        for (int n = 0; n < MAX_SIMULTANEOUS_MULTI_SEQUENCES; n++) {
            PacketAssembly* a = assy + n;

            if (a->msgid == 0 || (a->msgid == m->msgid && a->netid == msg->NetID())) {
                a->msgid  = m->msgid;
                a->netid  = msg->NetID();
                a->nreq   = m->nparts;
                a->nparts += 1;
                a->nbytes += m->len - header_size;
                break;
            }
        }
    }

    for (int n = 0; n < MAX_SIMULTANEOUS_MULTI_SEQUENCES; n++) {
        PacketAssembly* a = assy + n;

        // is this sequence complete?
        if (a->msgid && a->nparts == a->nreq) {
            std::uint8_t* buffer = new std::uint8_t[a->nbytes];
            std::uint8_t* p      = buffer;
            std::uint16_t  nid   = 0;

            ListIter<NetMsg> iter = multi_recv_list;
            while (++iter) {
                netid = iter->NetID();
                NetMsgMultipart* m = (NetMsgMultipart*) iter->Data();

                // found part of the sequence
                if (m->msgid == a->msgid && netid == a->netid) {
                    // copy it into the buffer
                    std::memcpy(p, m->payload, m->len - header_size);
                    p += m->len - header_size;

                    delete iter.removeItem();
                }
            }

            NetMsg* msg = new NetMsg(netid, buffer, a->nbytes, NetMsg::RELIABLE);
            if (msg) {
                recv_list.insertSort(msg);

                if (q)
                    q->insertSort(msg);
            }
        }
    }
}
