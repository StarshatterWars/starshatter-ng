/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    CampaignSituationReport generates the situation report
    portion of the briefing for a dynamically generated
    mission in a dynamic campaign.
*/

#ifndef CampaignSituationReport_h
#define CampaignSituationReport_h

#include "Types.h"
#include "Geometry.h"
#include "List.h"
#include "Text.h"

// +--------------------------------------------------------------------+

class Campaign;
class CombatGroup;
class CombatUnit;
class CombatZone;
class Mission;
class MissionElement;

// +--------------------------------------------------------------------+

class CampaignSituationReport
{
public:
    static const char* TYPENAME() { return "CampaignSituationReport"; }

    CampaignSituationReport(Campaign* c, Mission* m);
    virtual ~CampaignSituationReport();

    virtual void      GenerateSituationReport();

protected:
    virtual void      GlobalSituation();
    virtual void      MissionSituation();
    virtual MissionElement*
    FindEscort(MissionElement* elem);
    virtual Text      GetThreatInfo();

    Campaign*         campaign;
    Mission*          mission;
    Text              sitrep;
};

#endif  // CampaignSituationReport_h

