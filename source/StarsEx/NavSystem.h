/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Nav Points and so on...
*/

#ifndef NavSystem_h
#define NavSystem_h

#include "Types.h"
#include "Geometry.h"
#include "System.h"

// +--------------------------------------------------------------------+

class StarSystem;
class Orbital;
class OrbitalBody;
class OrbitalRegion;
class Ship;

// +--------------------------------------------------------------------+

class NavSystem : public System
{
public:
    NavSystem();
    NavSystem(const NavSystem& rhs);
    virtual ~NavSystem();

    virtual void   ExecFrame(double seconds);

    virtual void   Distribute(double delivered_energy, double seconds);

    bool           AutoNavEngaged();
    void           EngageAutoNav();
    void           DisengageAutoNav();

protected:
    bool           autonav;
};

// +--------------------------------------------------------------------+


#endif  // NavSystem_h

