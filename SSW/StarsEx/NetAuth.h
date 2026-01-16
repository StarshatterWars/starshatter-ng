/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    This class authenticates a user connecting to the multiplayer lobby
*/


#ifndef NetAuth_h
#define NetAuth_h

#include "Types.h"
#include "NetAddr.h"
#include "NetLobby.h"
#include "Text.h"

// +-------------------------------------------------------------------+

class NetAuth
{
public:
    enum AUTH_STATE {
        NET_AUTH_INITIAL  = 0,
        NET_AUTH_FAILED   = 1,
        NET_AUTH_OK       = 2
    };

    enum AUTH_LEVEL {
        NET_AUTH_MINIMAL  = 0,
        NET_AUTH_STANDARD = 1,
        NET_AUTH_SECURE   = 2
    };

    static int     AuthLevel();
    static void    SetAuthLevel(int n);

    static Text    CreateAuthRequest(NetUser* u);
    static Text    CreateAuthResponse(int level, const char* salt);
    static bool    AuthUser(NetUser* u, Text response);
};

// +-------------------------------------------------------------------+

#endif  // NetAuth_h