/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Package Callsign catalog class
*/

#ifndef Callsign_h
#define Callsign_h

#include "Types.h"

// +--------------------------------------------------------------------+

class Callsign
{
public:
    static const char*   GetCallsign(int IFF=1);
};

#endif  // Callsign_h

