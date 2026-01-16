/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    CampaignPlanAssignment creates combat assignments for
    assets within each combat zone as the third step in
    force tasking.
*/

#ifndef CampaignPlanAssignment_h
#define CampaignPlanAssignment_h

#include "Types.h"
#include "CampaignPlan.h"

// +--------------------------------------------------------------------+

class CombatGroup;
class CombatUnit;
class CombatZone;

// +--------------------------------------------------------------------+

class CampaignPlanAssignment : public CampaignPlan
{
public:
    static const char* TYPENAME() { return "CampaignPlanAssignment"; }

    CampaignPlanAssignment(Campaign* c) : CampaignPlan(c) { }
    virtual ~CampaignPlanAssignment()                     { }

    // operations:
    virtual void   ExecFrame();

protected:
    virtual void   ProcessCombatant(Combatant* c);
    virtual void   ProcessZone(Combatant* c, CombatZone* zone);
    virtual void   BuildZoneList(CombatGroup* g, CombatZone* zone, List<CombatGroup>& list);
    virtual void   BuildAssetList(const int* pref, List<CombatGroup>& avail, List<CombatGroup>& assets);
};

#endif  // CampaignPlanAssignment_h

