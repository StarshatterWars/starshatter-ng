/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Sensor Contact class
*/

#pragma once

#include <cstdint>

#include "SimObject.h"
#include "System.h"
#include "Geometry.h"

// +--------------------------------------------------------------------+

class Ship;
class Shot;

class Contact : public SimObserver
{
    friend class Sensor;

public:
    static const char* TYPENAME() { return "Contact"; }

    Contact();
    Contact(Ship* s, float p, float a);
    Contact(Shot* s, float p, float a);
    virtual ~Contact();

    int operator == (const Contact& c) const;

    Ship*    GetShip()   const { return ship;  }
    Shot*    GetShot()   const { return shot;  }
    Point    Location()  const { return loc;   }

    double   PasReturn() const { return d_pas; }
    double   ActReturn() const { return d_act; }
    bool     PasLock()   const;
    bool     ActLock()   const;
    double   Age()       const;
    bool     IsProbed()  const { return probe; }

    std::uint32_t AcquisitionTime() const { return acquire_time; }

    int      GetIFF(const Ship* observer)    const;
    void     GetBearing(const Ship* observer, double& az, double& el, double& r) const;
    double   Range(const Ship* observer,
    double      limit=75e3)   const;

    bool     InFront(const Ship* observer)   const;
    bool     Threat(const Ship* observer)    const;
    bool     Visible(const Ship* observer)   const;

    void     Reset();
    void     Merge(Contact* c);
    void     ClearTrack();
    void     UpdateTrack();
    int      TrackLength()     const { return ntrack;   }
    Point    TrackPoint(int i) const;

    virtual bool         Update(SimObject* obj);
    virtual const char*  GetObserverName() const;

private:
    Ship*    ship;
    Shot*    shot;
    Point    loc;
    std::uint32_t  acquire_time;
    std::uint32_t  time;

    Point*   track;
    int      ntrack;
    std::uint32_t  track_time;

    float    d_pas;   // power output
    float    d_act;   // mass, size
    bool     probe;   // scanned by probe
};
