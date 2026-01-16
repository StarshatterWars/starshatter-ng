/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    CampaignPlanMission generates missions and mission
    info for the player's combat group as part of a
    dynamic campaign.
*/

#ifndef CampaignPlanMission_h
#define CampaignPlanMission_h

#include "Types.h"
#include "CampaignPlan.h"

// +--------------------------------------------------------------------+

class CampaignMissionRequest;
class CombatGroup;
class CombatUnit;
class CombatZone;

// +--------------------------------------------------------------------+

class CampaignPlanMission : public CampaignPlan
{
public:
    static const char* TYPENAME() { return "CampaignPlanMission"; }

    CampaignPlanMission(Campaign* c) : CampaignPlan(c), start(0), slot(0) { }
    virtual ~CampaignPlanMission() { }

    // operations:
    virtual void   ExecFrame();

protected:
    virtual void                     SelectStartTime();
    virtual CampaignMissionRequest*  PlanCampaignMission();
    virtual CampaignMissionRequest*  PlanStrategicMission();
    virtual CampaignMissionRequest*  PlanRandomStarshipMission();
    virtual CampaignMissionRequest*  PlanRandomFighterMission();
    virtual CampaignMissionRequest*  PlanStarshipMission();
    virtual CampaignMissionRequest*  PlanFighterMission();

    CombatGroup*   player_group;
    int            start;
    int            slot;
};

#endif  // CampaignPlanMission_h

