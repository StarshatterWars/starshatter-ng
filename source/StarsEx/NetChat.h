/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Single chat message and sender
*/

#pragma once

#include <cstdint>

#include "NetUser.h"

// +-------------------------------------------------------------------+

class NetChatEntry
{
public:
    static const char* TYPENAME() { return "NetChatEntry"; }

    NetChatEntry(const NetUser* user, const char* msg);
    NetChatEntry(int id, const char* user, const char* msg);
    ~NetChatEntry();

    int operator == (const NetChatEntry& c) const { return id == c.id; }
    int operator <  (const NetChatEntry& c) const { return id <  c.id; }

    int            GetID()        const { return id;         }
    const Text&    GetUser()      const { return user;       }
    Color          GetColor()     const { return color;      }
    const Text&    GetMessage()   const { return msg;        }
    std::uint32_t  GetTime()      const { return time;       }

private:
    int            id;
    Text           user;
    Text           msg;
    Color          color;
    std::uint32_t  time;
};
