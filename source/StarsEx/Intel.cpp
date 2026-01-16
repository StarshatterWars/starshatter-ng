/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    An element in the dynamic campaign
*/

#include "Intel.h"

#include "Game.h"

// +--------------------------------------------------------------------+

static const char* intel_name[] = {
    "",
    "Reserve",
    "Secret",
    "Known",
    "Located",
    "Tracked",
};

const char*
Intel::NameFromIntel(int intel)
{
    return intel_name[intel];
}

int
Intel::IntelFromName(const char* type_name)
{
    for (int i = RESERVE; i <= TRACKED; i++)
    if (!_stricmp(type_name, intel_name[i]))
    return i;

    return 0;
}
