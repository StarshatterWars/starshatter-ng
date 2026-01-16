/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Wrapper for low-level datagram class
*/

#pragma once

#include <cstdint>

#include "Geometry.h"
#include "NetData.h"

// +--------------------------------------------------------------------+

class NetLink;
class NetMsg;

// +--------------------------------------------------------------------+

class NetPacket
{
public:
    static const char* TYPENAME() { return "NetPacket"; }

    NetPacket(NetMsg* g);
    NetPacket(std::uint32_t netid, std::uint8_t type);
    ~NetPacket();

    bool Send(NetLink& link);

    // various accessors:
    std::uint32_t NetID()  const;
    std::uint8_t  Type()   const;

    std::uint32_t GetPingSequence();
    void          SetPingSequence(std::uint32_t seq);
    std::uint32_t GetNetID();
    void          SetNetID(std::uint32_t id);

    Point       GetShipLocation();
    void        SetShipLocation(const Point& loc);
    Point       GetShipVelocity();
    void        SetShipVelocity(const Point& vel);
    Point       GetShipOrientation();
    void        SetShipOrientation(const Point& rpy);
    double      GetThrottle();
    void        SetThrottle(double t);
    const char* GetName();
    void        SetName(const char* name);
    const char* GetDesign();
    void        SetDesign(const char* design);
    const char* GetRegion();
    void        SetRegion(const char* rgn_name);
    bool        GetTrigger(int i);
    void        SetTrigger(int i, bool trigger);

protected:
    NetMsg* msg;
};
