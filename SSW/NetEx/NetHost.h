/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network Host
*/

#ifndef NetHost_h
#define NetHost_h

#include "List.h"
#include "NetAddr.h"
#include "Text.h"


class NetHost
{
public:
    static const char* TYPENAME() { return "NetHost"; }

    NetHost();
    NetHost(const char* host_addr);
    NetHost(const NetHost& n);
    ~NetHost();

    const char*    Name();
    NetAddr        Address();

    List<Text>&    Aliases()      { return aliases; }
    List<NetAddr>& AddressList()  { return addresses; }

private:
    void Init(const char* host_name);

    Text           name;
    List<Text>     aliases;
    List<NetAddr>  addresses;
};


#endif  // NetHost_h
