/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Water surface effect w/ reflection and refraction
*/

#ifndef Water_h
#define Water_h

#include "Geometry.h"
#include "Polygon.h"
#include "Color.h"

// +--------------------------------------------------------------------+

struct WATER_SURFACE;

// +--------------------------------------------------------------------+

class Water
{
public:
    Water();
    virtual ~Water();

    virtual void   Init(int nVerts, float size, float depth);
    virtual void   CalcWaves(double seconds);
    virtual void   UpdateSurface(Vec3& eyePos, VertexSet* vset);

protected:
    float          size;
    float          depth;
    float          scaleTex;
    float          avgHeight;

    DWORD          nVertices;

    WATER_SURFACE* surface;
    float*         waves;
    float          offsets[16];
};

// +--------------------------------------------------------------------+

#endif  // Water_h

