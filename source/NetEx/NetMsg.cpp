/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    User level network message
*/

#include "NetMsg.h"

#include <cstdint>
#include <cstring>


static std::uint32_t net_msg_sequence = 1;


NetMsg::NetMsg(std::uint32_t nid, void* d, int l, std::uint8_t f) :
    msgid(net_msg_sequence++),
    netid(nid),
    len(l),
    flags(f)
{
    data = new std::uint8_t[len];

    if (data) {
        std::memcpy(data, d, len);

        if (len < MAX_SIZE)
            data[1] = len;
        else
            data[1] = 0;
    }
    else {
        len = 0;
    }
}


NetMsg::NetMsg(std::uint32_t nid, std::uint8_t type, const char* text, int l, std::uint8_t f) :
    msgid(net_msg_sequence++),
    netid(nid),
    len(2+l),
    flags(f)
{
    data = new std::uint8_t[len];

    if (data) {
        data[0] = type;

        if (len < MAX_SIZE)
            data[1] = len;
        else
            data[1] = 0;

        if (len > 2)
            std::memcpy(data + 2, text, len - 2);
    }
    else {
        len = 0;
    }
}


NetMsg::~NetMsg()
{
    delete [] data;
}


int
NetMsg::operator < (const NetMsg& m) const
{
    if (data[0] == MULTIPART && m.data[0] == MULTIPART) {
        NetMsgMultipart* p1 = (NetMsgMultipart*) data;
        NetMsgMultipart* p2 = (NetMsgMultipart*) m.data;

        if (p1->msgid == p2->msgid)
            return p1->partno < p2->partno;

        return p1->msgid < p2->msgid;
    }

    return msgid <  m.msgid;
}
