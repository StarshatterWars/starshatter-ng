/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Client for Starshatter.com GameNet Broker
*/


#ifndef NetBrokerClient_h
#define NetBrokerClient_h

#include "HttpClient.h"
#include "NetLobby.h"

// +-------------------------------------------------------------------+

class NetBrokerClient
{
public:
    static void Enable()  { broker_available = true;  }
    static void Disable() { broker_available = false; }

    static bool GameOn(const char* name,
    const char* type,
    const char* addr,
    WORD        port,
    const char* password);
    static bool GameList(const char* type, List<NetServerInfo>& server_list);

protected:
    static bool broker_available;
    static bool broker_found;
};


#endif  // NetBrokerClient_h