/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Starship (low-level) Artifical Intelligence class
*/

#pragma once

#include <cstdint>

#include "ShipAI.h"


class StarshipAI : public ShipAI
{
public:
    StarshipAI(SimObject* s);
    virtual ~StarshipAI();

    // convert the goal point from world to local coords:
    virtual void      FindObjective();

protected:
    // accumulate behaviors:
    virtual void      Navigator();
    virtual Steer     SeekTarget();
    virtual Steer     AvoidCollision();

    // steering functions:
    virtual Steer     Seek(const Point& point);
    virtual Steer     Flee(const Point& point);
    virtual Steer     Avoid(const Point& point, float radius);

    virtual Point     Transform(const Point& pt);

    // fire on target if appropriate:
    virtual void      FireControl();
    virtual void      HelmControl();
    virtual void      ThrottleControl();

    System*           SelectSubtarget();
    bool              AssessTargetPointDefense();

    std::uint32_t     sub_select_time;
    std::uint32_t     point_defense_time;
    System*           subtarget;
    bool              tgt_point_defense;
};
