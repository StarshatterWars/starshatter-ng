/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Utility functions for generating random numbers and locations.
*/

#ifndef Random_h
#define Random_h

#include "Types.h"
#include "Geometry.h"

// +----------------------------------------------------------------------+

void     RandomInit();
Point    RandomDirection();
Point    RandomPoint();
Vec3     RandomVector(double radius);
double   Random(double min=0, double max=1);
int      RandomIndex();
bool     RandomChance(int wins=1, int tries=2);
int      RandomSequence(int current, int range);
int      RandomShuffle(int count);

// +----------------------------------------------------------------------+

#endif  // Random_h
