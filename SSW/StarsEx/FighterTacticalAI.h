/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Fighter-specific mid-level (tactical) AI
*/

#pragma once

#include <cstdint>

#include <List.h>

#include "TacticalAI.h"

// +--------------------------------------------------------------------+

class WeaponGroup;

// +--------------------------------------------------------------------+

class FighterTacticalAI : public TacticalAI
{
public:
    FighterTacticalAI(ShipAI* ai);
    virtual ~FighterTacticalAI();

protected:
    virtual bool      CheckFlightPlan();
    virtual bool      IsStrikeComplete(Instruction* instr=0);

    virtual void      SelectTarget();
    virtual void      SelectTargetDirected(Ship* tgt=0);
    virtual void      SelectTargetOpportunity();
    virtual void      FindFormationSlot(int formation);
    virtual void      FindThreat();

    virtual void      SelectSecondaryForTarget(Ship* tgt);
    virtual int       ListSecondariesForTarget(Ship* tgt, List<WeaponGroup>& weps);

    bool              winchester[4];
    std::uint32_t     THREAT_REACTION_TIME;
    std::uint32_t     secondary_selection_time;
    int               ai_level;
};
