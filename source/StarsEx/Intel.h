/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
*/

#ifndef Intel_h
#define Intel_h

#include "Types.h"

// +--------------------------------------------------------------------+

class Intel
{
public:
    enum INTEL_TYPE {
        RESERVE = 1,   // out-system reserve: this group is not even here
        SECRET,        // enemy is completely unaware of this group
        KNOWN,         // enemy knows this group is in the system
        LOCATED,       // enemy has located at least the lead ship
        TRACKED        // enemy is tracking all elements
    };

    static int           IntelFromName(const char* name);
    static const char*   NameFromIntel(int intel);
};

#endif  // Intel_h

