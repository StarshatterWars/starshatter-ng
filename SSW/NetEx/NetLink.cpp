/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network (IP) Socket Wrapper Implementation
*/

#include "NetLink.h"

#include <chrono>
#include <cstdint>
#include <cstring>
#include <mutex>
#include <thread>

#include "List.h"
#include "NetAddr.h"
#include "NetGram.h"
#include "NetLayer.h"
#include "NetPeer.h"
#include "NetSock.h"
#include "Text.h"


const std::uint32_t UDP_HEADER_SIZE = 34;


// client-side ctor
NetLink::NetLink() :
    shutdown(false),
    traffic_time(50),
    resend_time(300),
    packets_sent(0),
    packets_recv(0),
    bytes_sent(0),
    bytes_recv(0),
    retries(0),
    drops(0),
    lag(100)
{
    std::memset(lag_samples, 0, sizeof(lag_samples));
    lag_index = 0;

    hnet = std::thread([&]{ DoSendRecv(); });
}


// server-side ctor
NetLink::NetLink(NetAddr& a) :
    addr(a),
    shutdown(false),
    traffic_time(50),
    resend_time(300),
    packets_sent(0),
    packets_recv(0),
    bytes_sent(0),
    bytes_recv(0),
    retries(0),
    drops(0),
    lag(100)
{
    std::memset(lag_samples, 0, sizeof(lag_samples));
    lag_index = 0;

    sock.bind(addr);
    hnet = std::thread([&]{ DoSendRecv(); });
}


NetLink::~NetLink()
{
    if (!shutdown) {
        shutdown = true;
    }

    if (hnet.joinable()) {
        hnet.join();
    }

    send_list.destroy();    // packets waiting to be ack'ed must be destroyed
    recv_list.clear();      // received messages are owned by the peers
    peer_list.destroy();    // but the net link owns the peers!
}


static std::uint32_t s_base_netid = 1000;


std::uint32_t
NetLink::AddPeer(const char* a, std::uint16_t p)
{
    return AddPeer(NetAddr(a, p));
}


std::uint32_t
NetLink::AddPeer(std::uint32_t a, std::uint16_t p)
{
    return AddPeer(NetAddr(a, p));
}


std::uint32_t
NetLink::AddPeer(const NetAddr& a)
{
    if (!a.IPAddr())
        return 0;

    NetPeer* peer = FindPeer(a);
    const std::lock_guard<std::mutex> lock(sync);

    if (!peer) {
        peer = new NetPeer(a, s_base_netid++);
        if (peer)
            peer_list.append(peer);
    }

    if (peer)
        return peer->NetID();

    return 0;
}


bool
NetLink::SendMessage(std::uint32_t nid, void* d, int l, std::uint8_t f)
{
    return SendMessage(new NetMsg(nid, d, l, f));
}


bool
NetLink::SendMessage(std::uint32_t nid, std::uint8_t type, const char* text, int len, std::uint8_t f)
{
    return SendMessage(new NetMsg(nid, type, text, len, f));
}


bool
NetLink::SendMessage(NetMsg* msg)
{
    if (msg) {
        if (msg->Type() != NetMsg::INVALID  &&
             msg->Type() <  NetMsg::RESERVED &&
             msg->NetID()) {

            NetPeer* p = FindPeer(msg->NetID());
            if (p)
                return p->SendMessage(msg);
        }

        delete msg;
    }

    return false;
}


NetMsg*
NetLink::GetMessage(std::uint32_t netid)
{
    NetMsg* msg = 0;

    // receive from specific host:
    if (netid) {
        NetPeer* p = FindPeer(netid);
        if (p) {
            msg = p->GetMessage();

            sync.lock();
            recv_list.remove(msg);
            sync.unlock();
        }
    }

    return msg;
}


NetMsg*
NetLink::GetMessage()
{
    NetMsg* msg = 0;

    // get first available packet:

    // Double-checked locking:
    if (recv_list.size()) {
        sync.lock();
        if (recv_list.size()) {
            msg = recv_list.removeIndex(0);
        }
        sync.unlock();

        if (msg && msg->NetID()) {
            NetPeer* p = FindPeer(msg->NetID());
            if (p) {
                p->GetMessage();  // remove message from peer's list
                                  // don't do this inside of sync block -
                                  // it might cause a deadlock
            }
        }
    }

    return msg;
}


void
NetLink::Shutdown()
{
    shutdown = true;
}


std::uint32_t
NetLink::DoSendRecv()
{
    while (!shutdown) {
        ReadPackets();
        SendPackets();

        // discard reeeeally old peers:
        sync.lock();

        ListIter<NetPeer> iter = peer_list;
        while (!shutdown && ++iter) {
            NetPeer* peer = iter.value();

            if ((NetLayer::GetUTC() - peer->LastReceiveTime()) > 300)
                delete iter.removeItem();
        }

        sync.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(traffic_time));
    }

    return 0;
}


void
NetLink::ReadPackets()
{
    while (!shutdown && sock.select(NetSock::SELECT_READ) > 0) {
        NetGram* gram = RecvNetGram();

        if (gram && gram->IsReliable()) {
            if (gram->IsAck()) {
                ProcessAck(gram);
                delete gram;
            }
            else {
                AckNetGram(gram);
                QueueNetGram(gram);
            }
        }
        else {
            QueueNetGram(gram);
        }
    }
}


void
NetLink::SendPackets()
{
    if (shutdown)
        return;

    if (sock.select(NetSock::SELECT_WRITE) > 0) {
        DoRetries();
    }

    const std::lock_guard<std::mutex> lock(sync);

    ListIter<NetPeer> iter = peer_list;
    while (!shutdown && ++iter) {
        NetPeer* p = iter.value();
        NetGram* g = 0;

        do {
            if (sock.select(NetSock::SELECT_WRITE) > 0) {
                g = p->ComposeGram();
                if (g) {
                    SendNetGram(g);
                }
            }
            else {
                g = 0;
            }
        }
        while (!shutdown && g);
    }
}


void
NetLink::SendNetGram(NetGram* gram)
{
    if (gram) {
        if (gram->IsReliable()) {
            send_list.append(gram);
        }

        int err = sock.sendto(gram->Body(), gram->Address());

        if (err < 0) {
            err = NetLayer::GetLastError();
        }
        else {
            packets_sent += 1;
            bytes_sent   += gram->Size() + UDP_HEADER_SIZE;
        }

        if (!gram->IsReliable())
            delete gram;
    }
}


NetGram*
NetLink::RecvNetGram()
{
    NetAddr from;
    Text msg = sock.recvfrom(&from);

    packets_recv += 1;
    bytes_recv   += msg.length() + UDP_HEADER_SIZE;

    return new NetGram(from, msg);
}


void
NetLink::AckNetGram(NetGram* gram)
{
    if (gram) {
        NetGram ack = gram->Ack();

        int err = sock.sendto(ack.Body(), gram->Address());
        if (err < 0)
            err = NetLayer::GetLastError();
    }
    else {
        Print("NetLink::AckNetGram( NULL!!! )\n");
    }
}


void
NetLink::ProcessAck(NetGram* gram)
{
    if (!shutdown && send_list.size()) {
        sync.lock();

        // remove the ack flag:
        gram->ClearAck();

        // find a matching outgoing packet:
        int sent = send_list.index(gram);
        if (sent >= 0) {
            NetGram* orig = send_list.removeIndex(sent);
            std::uint32_t    time = NetLayer::GetTime();
            std::uint32_t    msec = time - orig->SendTime();
            double   dlag = 0.75 * lag + 0.25 * msec;

            if (lag_index >= 10) lag_index = 0;
            lag_samples[lag_index++] = msec;

            sync.unlock();
            NetPeer* peer = FindPeer(orig->Address());
            sync.lock();
            if (peer)
                peer->SetLastReceiveTime(NetLayer::GetUTC());

            delete orig;

            lag = (std::uint32_t) dlag;

            if (lag > 100)
                resend_time = 3 * lag;
            else
                resend_time = 300;
        }
        sync.unlock();
    }
}


void
NetLink::QueueNetGram(NetGram* gram)
{
    if (!shutdown) {
        std::uint32_t sequence = 0;
        NetPeer* peer = FindPeer(gram->Address());
        const std::lock_guard<std::mutex> lock(sync);

        if (peer) {
            sequence = peer->Sequence();
        }
        else {
            peer = new NetPeer(gram->Address(), s_base_netid++);
            if (peer)
                peer_list.append(peer);
        }

        if (!gram->IsReliable()) {
            if (gram->Sequence() < sequence) {  // discard, too old
                delete gram;
                return;
            }
        }

        // sort this gram into the recv list(s) based on sequence:
        if (peer) {
            peer->ReceiveGram(gram, &recv_list);
        }
    }
}


void
NetLink::DoRetries()
{
    if (!shutdown) {
        const std::lock_guard<std::mutex> lock(sync);

        if (send_list.size()) {
            int time = (int) NetLayer::GetTime();

            ListIter<NetGram> iter = send_list;
            while (!shutdown && ++iter) {
                NetGram* gram = iter.value();

                // still trying ?
                if (gram->Retries() > 0) {
                    std::uint32_t last_send = gram->SendTime();
                    std::uint32_t delta     = time - last_send;

                    if (delta > resend_time) {
                        gram->Retry();
                        sock.sendto(gram->Body(), gram->Address());
                        retries++;
                    }
                }

                // oh, give it up:
                else {
                    iter.removeItem();
                    delete gram;
                    drops++;
                }
            }
        }
    }
}


NetPeer*
NetLink::FindPeer(std::uint32_t netid)
{
    const std::lock_guard<std::mutex> lock(sync);
    NetPeer* peer = 0;

    ListIter<NetPeer> iter = peer_list;
    while (++iter && !peer) {
        NetPeer* p = iter.value();

        if (p->NetID() == netid)
            peer = p;
    }

    return peer;
}


NetPeer*
NetLink::FindPeer(const NetAddr& a)
{
    const std::lock_guard<std::mutex> lock(sync);
    NetPeer* peer = 0;

    ListIter<NetPeer> iter = peer_list;
    while (++iter && !peer) {
        NetPeer* p = iter.value();

        if (p->Address() == a)
            peer = p;
    }

    return peer;
}
