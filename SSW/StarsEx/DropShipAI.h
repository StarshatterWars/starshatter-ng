/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Drop Ship (orbit/surface and surface/orbit) AI class
*/

#ifndef DropShipAI_h
#define DropShipAI_h

#include "Types.h"
#include "Geometry.h"
#include "System.h"
#include "ShipAI.h"

// +--------------------------------------------------------------------+

class Ship;

// +--------------------------------------------------------------------+

class DropShipAI : public ShipAI
{
public:
    DropShipAI(Ship* s);
    virtual ~DropShipAI();

    enum { DIR_TYPE = 2001 };
    virtual int       Type() const { return DIR_TYPE; }

protected:
    // accumulate behaviors:
    virtual void      Navigator();
    virtual void      FindObjective();
};

// +--------------------------------------------------------------------+

#endif  // DropShipAI_h

