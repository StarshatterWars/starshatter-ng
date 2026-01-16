/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    CampaignSaveGame contains the logic needed to save and load
    campaign games in progress.
*/

#ifndef CampaignSaveGame_h
#define CampaignSaveGame_h

#include "Types.h"
#include "Geometry.h"
#include "Text.h"
#include "List.h"

// +--------------------------------------------------------------------+

class Campaign;
class CampaignPlan;
class Combatant;
class CombatGroup;
class CombatZone;
class DataLoader;
class Mission;
class Player;
class StarSystem;

// +--------------------------------------------------------------------+

class CampaignSaveGame
{
public:
    static const char* TYPENAME() { return "CampaignSaveGame"; }

    CampaignSaveGame(Campaign* c=0);
    virtual ~CampaignSaveGame();

    virtual Campaign* GetCampaign()     { return campaign;   }

    virtual void      Load(const char* name);
    virtual void      Save(const char* name);
    static  void      Delete(const char* name);
    static  void      RemovePlayer(Player* p);

    virtual void      LoadAuto();
    virtual void      SaveAuto();

    static  Text      GetResumeFile();
    static  int       GetSaveGameList(List<Text>& save_list);

private:
    static  Text      GetSaveDirectory();
    static  Text      GetSaveDirectory(Player* p);
    static  void      CreateSaveDirectory();

    Campaign*   campaign;
};

#endif  // CampaignSaveGame_h

