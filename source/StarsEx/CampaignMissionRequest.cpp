/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    CampaignMissionRequest
*/

#include "CampaignMissionRequest.h"
#include "Campaign.h"
#include "Combatant.h"
#include "CombatAssignment.h"
#include "CombatGroup.h"
#include "CombatUnit.h"
#include "CombatZone.h"
#include "Mission.h"
#include "Instruction.h"
#include "ShipDesign.h"
#include "StarSystem.h"
#include "Random.h"

// +--------------------------------------------------------------------+

CampaignMissionRequest::CampaignMissionRequest(Campaign*    c,
                                               int          t,
                                               int          s,
                                               CombatGroup* p,
                                               CombatGroup* tgt)
    : campaign(c), type(t), opp_type(-1), start(s),
      primary_group(p), secondary_group(0),
      objective(tgt), use_loc(false)
{ }
