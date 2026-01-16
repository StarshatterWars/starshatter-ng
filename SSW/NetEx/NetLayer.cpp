/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Wrapper for WinSock Library
*/

#include "NetLayer.h"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <errno.h>
#include <unistd.h>
#endif

#include <chrono>
#include <cstring>
#include <ctime>
#include <ratio>


static const auto base_time = std::chrono::high_resolution_clock::now();


NetLayer::NetLayer() :
    fail{false}
{
#ifdef _WIN32
    WSADATA info;
    WORD ver = MAKEWORD(2,2);
    int  err = WSAStartup(ver, &info);

    if (err)
        fail = true;
#endif
}


NetLayer::~NetLayer()
{
#ifdef _WIN32
    WSACleanup();
#endif
}


bool
NetLayer::OK() const
{
    return !fail;
}


int
NetLayer::GetLastError()
{
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}


std::uint32_t
NetLayer::GetTime()
{
    const auto now = std::chrono::high_resolution_clock::now();
    const auto diff = now - base_time;
    using target_duration = std::chrono::duration<std::uint32_t, std::milli>;
    return std::chrono::duration_cast<target_duration>(diff).count();
}


long
NetLayer::GetUTC()
{
    return static_cast<long>(std::time(nullptr));
}


Text
NetLayer::GetHostName()
{
    char hostname[256];
    std::memset(hostname, 0, sizeof(hostname));
    ::gethostname(hostname, sizeof(hostname));

    return hostname;
}
