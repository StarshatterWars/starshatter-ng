/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Generic Network Packet (Datagram) Implementation
*/

#include "NetGram.h"

#include <atomic>
#include <cstdint>
#include <cstring>

#include "NetAddr.h"
#include "NetLayer.h"


static std::atomic<std::uint32_t> net_gram_sequence {1};


/**
 * NetGram constructor for ACK packets
 */
NetGram::NetGram() :
    retries(0),
    packet_id(0),
    send_time(0)
{
}


/**
 * NetGram constructor for receiving packets from remote hosts
 */
NetGram::NetGram(const NetAddr& src, Text msg) :
    addr(src),
    retries(0),
    send_time(0)
{
    body = msg;

    if (body.length() >= NET_GRAM_HEADER_SIZE) {
        std::uint8_t* data = (std::uint8_t*) body.data();
        packet_id =
            (static_cast<std::uint32_t>(data[0]) << 24) +
            (static_cast<std::uint32_t>(data[1]) << 16) +
            (static_cast<std::uint32_t>(data[2]) <<  8) +
            (static_cast<std::uint32_t>(data[3]));
    }
}

/**
 * NetGram constructor for composing packets to send to remote hosts
 */
NetGram::NetGram(const NetAddr& dst, Text user_data, int r) :
    addr(dst),
    retries(r)
{
    send_time = NetLayer::GetTime();
    packet_id = net_gram_sequence++;

    if (retries)
        packet_id |= NET_GRAM_RELIABLE;

    static std::uint8_t buf[NET_GRAM_MAX_SIZE];
    buf[0] = static_cast<std::uint8_t>((packet_id >> 24) & 0xff);
    buf[1] = static_cast<std::uint8_t>((packet_id >> 16) & 0xff);
    buf[2] = static_cast<std::uint8_t>((packet_id >>  8) & 0xff);
    buf[3] = static_cast<std::uint8_t>((packet_id)       & 0xff);

    int len = user_data.length();
    if (len >= NET_GRAM_MAX_SIZE - NET_GRAM_HEADER_SIZE)
        len = NET_GRAM_MAX_SIZE - NET_GRAM_HEADER_SIZE - 1;

    std::memcpy(buf + NET_GRAM_HEADER_SIZE, user_data.data(), len);

    body = Text((char*) buf, len+NET_GRAM_HEADER_SIZE);
}


void
NetGram::Retry()
{
    if (retries > 0) {
        retries--;
        send_time = NetLayer::GetTime();
    }
}


NetGram
NetGram::Ack()
{
    NetGram ack;

    ack.packet_id  = packet_id | NET_GRAM_ACK;
    ack.send_time  = NetLayer::GetTime();

    static std::uint8_t buf[NET_GRAM_HEADER_SIZE];
    buf[0] = static_cast<std::uint8_t>((ack.packet_id >> 24) & 0xff);
    buf[1] = static_cast<std::uint8_t>((ack.packet_id >> 16) & 0xff);
    buf[2] = static_cast<std::uint8_t>((ack.packet_id >>  8) & 0xff);
    buf[3] = static_cast<std::uint8_t>((ack.packet_id)       & 0xff);

    ack.body = Text((char*) buf, NET_GRAM_HEADER_SIZE);

    return ack;
}
