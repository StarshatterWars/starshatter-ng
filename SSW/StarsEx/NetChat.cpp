/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Single chat message and sender
*/

#include "NetChat.h"
#include "NetLayer.h"

// +-------------------------------------------------------------------+

static int chat_id_key = 1000;

// +-------------------------------------------------------------------+

NetChatEntry::NetChatEntry(const NetUser* u, const char* s)
: id(chat_id_key++), msg(s)
{
    if (u) {
        user = u->Name();
        color = u->GetColor();
    }
    else {
        user = "unknown";
        color = Color::Gray;
    }

    time = NetLayer::GetUTC();
}

NetChatEntry::NetChatEntry(int msg_id, const char* u, const char* s)
: id(msg_id), user(u), msg(s)
{
    color = Color::Gray;
    time = NetLayer::GetUTC();

    if (id >= chat_id_key)
    chat_id_key = id + 1;
}

NetChatEntry::~NetChatEntry()
{ }
