/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network Player (Director) class
*/

#pragma once

#include <cstdint>

#include <List.h>
#include <Text.h>

#include "Geometry.h"
#include "Director.h"
#include "SimObject.h"

// +--------------------------------------------------------------------+

class Sim;
class Ship;
class NetMsg;
class NetObjLoc;
class NetObjHyper;
class NetObjTarget;
class NetObjEmcon;
class NetSysDamage;
class NetSysStatus;
class NetWepTrigger;
class NetWepRelease;
class NetWepDestroy;
class NetCommMsg;

// +--------------------------------------------------------------------+

class NetPlayer : public Director, public SimObserver
{
public:
    static const char* TYPENAME() { return "NetPlayer"; }

    NetPlayer(std::uint32_t nid) : netid(nid), objid(0), ship(0), iff(0) { }
    ~NetPlayer();

    int operator == (const NetPlayer& p) const { return netid == p.netid; }

    std::uint32_t        GetNetID()  const { return netid;   }
    std::uint32_t        GetObjID()  const { return objid;   }
    void                 SetObjID(std::uint32_t o) { objid = o;      }

    int                  GetIFF()    const { return iff;     }
    Ship*                GetShip()   const { return ship;    }
    void                 SetShip(Ship* s);

    const char*          Name()                     const { return name;    }
    void                 SetName(const char* n)           { name = n;       }
    const char*          SerialNumber()             const { return serno;   }
    void                 SetSerialNumber(const char* n)   { serno = n;      }

    virtual void         ExecFrame(double seconds);

    bool                 DoObjLoc(NetObjLoc*         obj_loc);
    bool                 DoObjHyper(NetObjHyper*     obj_hyper);
    bool                 DoObjTarget(NetObjTarget*   obj_target);
    bool                 DoObjEmcon(NetObjEmcon*     obj_emcon);
    bool                 DoWepTrigger(NetWepTrigger* trigger);
    bool                 DoWepRelease(NetWepRelease* release);
    bool                 DoCommMessage(NetCommMsg*   comm_msg);
    bool                 DoSysDamage(NetSysDamage*   sys_damage);
    bool                 DoSysStatus(NetSysStatus*   sys_status);

    virtual int          Type()      const { return 2;       }

    virtual bool         Update(SimObject* obj);
    virtual const char*  GetObserverName() const;

protected:
    std::uint32_t        netid;
    std::uint32_t        objid;
    Text                 name;
    Text                 serno;
    Ship*                ship;
    int                  iff;

    Point                loc_error;
    double               bleed_time;
};
