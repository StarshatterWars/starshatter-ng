/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    A Single Edge Section of a Terrain Object
*/

#ifndef TerrainClouds_h
#define TerrainClouds_h

#include "Types.h"
#include "Graphic.h"
#include "Geometry.h"
#include "Polygon.h"

// +--------------------------------------------------------------------+

class Terrain;
class TerrainRegion;

// +--------------------------------------------------------------------+

class TerrainClouds : public Graphic
{
public:
    TerrainClouds(Terrain* terr, int type);
    virtual ~TerrainClouds();

    virtual void      Render(Video* video, DWORD flags);
    virtual void      Update();

    // accessors:
    virtual int       CollidesWith(Graphic& o)   { return 0;    }
    virtual bool      Luminous()           const { return true; }
    virtual bool      Translucent()        const { return true; }

    void              Illuminate(Color ambient, List<Light>& lights);

protected:
    void              BuildClouds();

    Terrain*          terrain;
    Vec3*             mverts;
    VertexSet*        verts;
    Poly*             polys;
    Material          mtl_cloud;
    Material          mtl_shade;

    int               type;
    int               nbanks;
    int               nverts;
    int               npolys;
};


#endif  // TerrainClouds_h

