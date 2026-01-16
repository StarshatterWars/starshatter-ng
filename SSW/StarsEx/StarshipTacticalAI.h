/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Starship-specific mid-level (tactical) AI
*/

#pragma once

#include <cstdint>

#include "TacticalAI.h"


class StarshipTacticalAI : public TacticalAI
{
public:
    StarshipTacticalAI(ShipAI* ai);
    virtual ~StarshipTacticalAI();

    virtual void      ExecFrame(double seconds);

protected:
    virtual void      FindThreat();
    virtual void      FindSupport();

    virtual void      CheckBugOut(Ship* c_ship, double range);

    std::uint32_t     THREAT_REACTION_TIME;
    int               ai_level;
    double            drop_time;
    double            initial_integrity;
    bool              bugout;
};
