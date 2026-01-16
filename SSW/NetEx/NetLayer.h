/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Wrapper for WinSock Library
*/

#ifndef NetLayer_h
#define NetLayer_h

#include <cstdint>

#include "Text.h"


class NetLayer
{
public:
    static const char* TYPENAME() { return "NetLayer"; }

    NetLayer();
    ~NetLayer();

    bool           OK()           const;
    const char*    Description()  const;

    static int     GetLastError();
    static std::uint32_t GetTime();
    static long    GetUTC();
    static Text    GetHostName();

private:
    bool     fail;
};


#endif  // NetLayer_h
