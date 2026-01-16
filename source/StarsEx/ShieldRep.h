/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    ShieldRep Solid class
*/

#ifndef ShieldRep_h
#define ShieldRep_h

#include "Types.h"
#include "Solid.h"

// +--------------------------------------------------------------------+

struct ShieldHit;
class  Shot;

class ShieldRep : public Solid
{
public:
    ShieldRep();
    virtual ~ShieldRep();

    // operations
    virtual void   Render(Video* video, DWORD flags);
    virtual void   Energize(double seconds, bool bubble=false);
    int            ActiveHits() const { return nhits; }
    virtual void   Hit(Vec3 impact, Shot* shot, double damage=0);
    virtual void   TranslateBy(const Point& ref);
    virtual void   Illuminate();

protected:
    int            nhits;
    ShieldHit*     hits;
    Point          true_eye_point;
    bool           bubble;
};

#endif  // ShieldRep_h

