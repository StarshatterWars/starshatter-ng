/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Abstract Director (AI or Human Input) for Physical Objects
*/

#ifndef Director_h
#define Director_h

#include "Types.h"
#include "Geometry.h"

// +--------------------------------------------------------------------+

class Physical;

// +--------------------------------------------------------------------+

class Director
{
public:
    Director()           { }
    virtual ~Director()  { }

    // accessors:
    virtual int       Type()      const { return 0; }
    virtual int       Subframe()  const { return 0; }

    // operations
    virtual void      ExecFrame(double factor) {    }
};

// +--------------------------------------------------------------------+

#endif  // Director_h

