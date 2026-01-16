/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network Host
*/

#include "NetHost.h"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <cstdint>


NetHost::NetHost()
{
    char host_name[256];
    ::gethostname(host_name, sizeof(host_name));

    Init(host_name);
}


NetHost::NetHost(const char* host_name)
{
    Init(host_name);
}


void
NetHost::Init(const char* host_name)
{
    if (host_name && *host_name) {
        struct hostent* h = nullptr;

        if (std::isdigit(*host_name)) {
            auto a = inet_addr(host_name);
            h = gethostbyaddr((const char*) &a, 4, AF_INET);
        }
        else {
            h = gethostbyname(host_name);
        }

        if (h) {
            name = h->h_name;

            char** alias = h->h_aliases;
            while (*alias) {
                aliases.append(new Text(*alias));
                alias++;
            }

            char** addr = h->h_addr_list;
            while (*addr) {
                NetAddr* pna = new NetAddr(**(std::uint32_t**) addr);
                if (pna)
                    addresses.append(pna);
                addr++;
            }
        }
    }
}


NetHost::NetHost(const NetHost& n)
{
    if (&n != this) {
        NetHost& nh = (NetHost&) n;

        name = nh.name;

        ListIter<Text> alias = nh.aliases;
        while (++alias)
            aliases.append(new Text(*alias.value()));

        ListIter<NetAddr> addr = nh.addresses;
        while (++addr)
            addresses.append(new NetAddr(*addr.value()));
    }
}


NetHost::~NetHost()
{
    aliases.destroy();
    addresses.destroy();
}

const char*
NetHost::Name()
{
    return name;
}


NetAddr
NetHost::Address()
{
    if (addresses.size())
        return *(addresses[0]);

    return NetAddr((std::uint32_t) 0);
}