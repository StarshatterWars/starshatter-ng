/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Power generation and usage classes
*/

#ifndef Power_h
#define Power_h

#include "Types.h"
#include "System.h"
#include "List.h"

// +--------------------------------------------------------------------+

class PowerSource : public System
{
public:
    enum SUBTYPE { BATTERY, AUX, FUSION };

    PowerSource(SUBTYPE s, double max_output, double fuel_ratio=0);
    PowerSource(const PowerSource& rhs);

    virtual void ExecFrame(double seconds);

    void AddClient(System* client);
    void RemoveClient(System* client);

    List<System>& Clients() { return clients; }

    virtual int    Charge() const;

    virtual void   SetFuelRange(double hours);

    bool           RouteChanged() const { return route_changed;  }
    void           RouteScanned()       { route_changed = false; }

    // override from System:
    virtual void   SetPowerLevel(double level);
    virtual void   SetOverride(bool over);

    // for power drain damage:
    virtual void   DrainPower(double to_level);

protected:
    float          max_output;
    float          fuel_ratio;
    List<System>   clients;
    bool           route_changed;
    float          requested_power_level;
};

#endif  // Power_h

