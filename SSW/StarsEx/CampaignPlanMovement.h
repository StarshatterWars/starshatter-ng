/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    CampaignPlanMovement simulates random patrol movements
    of starship groups between missions.  This agitation
    keeps the ships from bunching up in the middle of a
    sector.
*/

#ifndef CampaignPlanMovement_h
#define CampaignPlanMovement_h

#include "Types.h"
#include "CampaignPlan.h"

// +--------------------------------------------------------------------+

class CampaignPlanMovement : public CampaignPlan
{
public:
    static const char* TYPENAME() { return "CampaignPlanMovement"; }

    CampaignPlanMovement(Campaign* c) : CampaignPlan(c) { }
    virtual ~CampaignPlanMovement()                     { }

    // operations:
    virtual void      ExecFrame();

protected:
    void              MoveUnit(CombatUnit* u);

    List<CombatUnit>  all_units;
};

#endif  // CampaignPlanMovement_h

