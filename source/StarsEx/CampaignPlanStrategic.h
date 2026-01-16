/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    CampaignPlanStrategic prioritizes targets and defensible
    allied forces as the first step in force tasking.  This
    algorithm computes which enemy resources are most important
    to attack, based on the AI value of each combat group, and
    strategic weighting factors that help shape the strategy
    to the objectives for the current campaign.
*/

#ifndef CampaignPlanStrategic_h
#define CampaignPlanStrategic_h

#include "Types.h"
#include "CampaignPlan.h"

// +--------------------------------------------------------------------+

class CampaignPlanStrategic : public CampaignPlan
{
public:
    static const char* TYPENAME() { return "CampaignPlanStrategic"; }

    CampaignPlanStrategic(Campaign* c) : CampaignPlan(c) { }
    virtual ~CampaignPlanStrategic()                     { }

    // operations:
    virtual void   ExecFrame();

protected:
    void           PlaceGroup(CombatGroup* g);

    void           ScoreCombatant(Combatant* c);

    void           ScoreDefensible(Combatant* c);
    void           ScoreDefend(Combatant* c, CombatGroup* g);

    void           ScoreTargets(Combatant* c, Combatant* t);
    void           ScoreTarget(Combatant* c, CombatGroup* g);

    void           ScoreNeeds(Combatant* c);

    // zone alocation:
    void           BuildGroupList(CombatGroup* g, List<CombatGroup>& groups);
    void           AssignZones(Combatant* c);
    void           ResolveZoneMovement(CombatGroup* g);
};

#endif  // CampaignPlanStrategic_h

