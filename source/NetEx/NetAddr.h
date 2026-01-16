/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network Address (specifically, Internet Protocol)
*/

#ifndef NetAddr_h
#define NetAddr_h

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#include <cstdint>


class NetAddr
{
public:
    static const char* TYPENAME() { return "NetAddr"; }

    NetAddr(const char* a, std::uint16_t p=0);
    NetAddr(std::uint32_t a=0, std::uint16_t p=0);
    NetAddr(const NetAddr& n);

    int operator == (const NetAddr& a) const { return addr==a.addr && port==a.port; }

    std::uint32_t  IPAddr() const { return addr; }
    std::uint8_t   B4()     const { return static_cast<std::uint8_t>((addr & 0xff000000) >> 24); }
    std::uint8_t   B3()     const { return static_cast<std::uint8_t>((addr & 0x00ff0000) >> 16); }
    std::uint8_t   B2()     const { return static_cast<std::uint8_t>((addr & 0x0000ff00) >>  8); }
    std::uint8_t   B1()     const { return static_cast<std::uint8_t>((addr & 0x000000ff)      ); }

    std::uint16_t  Port()   const    { return port; }
    void           SetPort(std::uint16_t p)   { port = p;    }

    sockaddr*      GetSockAddr()       const;
    size_t         GetSockAddrLength() const;

    void           SetSockAddr(sockaddr* s, int size);
    void           InitFromSockAddr();

private:
    void        Init();

    std::uint32_t addr; // IP addr in host byte order
    std::uint16_t port; // IP port in host byte order
    sockaddr_in sadr;
};


#endif  // NetAddr_h
