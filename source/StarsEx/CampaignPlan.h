/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    CampaignPlan defines the interface for all campaign
    planning algorithms.  Known subclasses:
    CampaignPlanStrategic  - strategic planning
    CampaignPlanAssignment - logistics planning
    CampaignPlanMission    - mission planning
    CampaignPlanMovement   - starship movement
    CampaignPlanEvent      - scripted events
*/

#ifndef CampaignPlan_h
#define CampaignPlan_h

#include "Types.h"
#include "Text.h"
#include "List.h"

// +--------------------------------------------------------------------+

class Campaign;
class Combatant;
class CombatGroup;
class CombatUnit;

// +--------------------------------------------------------------------+

class CampaignPlan
{
public:
    static const char* TYPENAME() { return "CampaignPlan"; }

    CampaignPlan(Campaign* c) : campaign(c), exec_time(-1e6) { }
    virtual ~CampaignPlan()                                  { }

    int operator == (const CampaignPlan& p) const { return this == &p; }

    // operations:
    virtual void      ExecFrame() { }
    virtual void      SetLockout(int seconds) { }

protected:
    Campaign*         campaign;
    double            exec_time;
};

#endif  // CampaignPlan_h

