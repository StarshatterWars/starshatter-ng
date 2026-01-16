/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    The complete roster of all known persistent entities
    for all combatants in the game.
*/

#ifndef CombatRoster_h
#define CombatRoster_h

#include "Types.h"
#include "Geometry.h"
#include "List.h"

// +--------------------------------------------------------------------+

class CombatGroup;

// +--------------------------------------------------------------------+

class CombatRoster
{
    CombatRoster();
    ~CombatRoster();

public:
    static const char* TYPENAME() { return "CombatRoster"; }

    static void             Initialize();
    static void             Close();
    static CombatRoster*    GetInstance();

    CombatGroup*            GetForce(const char* name);

private:
    List<CombatGroup>       forces;
};


#endif  // CombatRoster_h
