/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Abstract Universe class
*/

#ifndef Universe_h
#define Universe_h

#include "Types.h"

// +--------------------------------------------------------------------+

class Universe
{
public:
    Universe()           { }
    virtual ~Universe()  { }

    virtual void   ExecFrame(double seconds) { }
};

#endif  // Universe_h

