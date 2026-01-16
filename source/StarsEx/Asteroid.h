/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Asteroid Sprite class
*/

#ifndef Asteroid_h
#define Asteroid_h

#include "Types.h"
#include "Geometry.h"
#include "SimObject.h"
#include "Debris.h"

// +--------------------------------------------------------------------+

class Asteroid : public Debris
{
public:
    Asteroid(int type, const Vec3& pos, double mass);

    static void Initialize();
    static void Close();
};

#endif  // Asteroid_h

