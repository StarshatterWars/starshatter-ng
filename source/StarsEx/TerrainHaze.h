/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Atmospheric fog along the horizon
*/

#ifndef TerrainHaze_h
#define TerrainHaze_h

#include "Types.h"
#include "Solid.h"
#include "Geometry.h"

// +--------------------------------------------------------------------+

class TerrainRegion;

// +--------------------------------------------------------------------+

class TerrainHaze : public Solid
{
public:
    TerrainHaze();
    virtual ~TerrainHaze();

    virtual void      Render(Video* video, DWORD flags);

    virtual int       CheckRayIntersection(Point pt, Point vpn, double len, Point& ipt,
    bool treat_translucent_polys_as_solid=true);

    virtual void      UseTerrainRegion(TerrainRegion* tr) { tregion = tr; }

protected:
    TerrainRegion*    tregion;
};

#endif  // TerrainHaze_h

