/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Missile Trail (Graphic) class
*/

#ifndef Trail_h
#define Trail_h

#include "Types.h"
#include "Geometry.h"
#include "Polygon.h"
#include "SimObject.h"
#include "Graphic.h"

// +--------------------------------------------------------------------+

class Trail : public Graphic
{
public:
    Trail(Bitmap* tex, int n=512);
    virtual ~Trail();

    virtual void      UpdateVerts(const Point& cam_pos);
    virtual void      Render(Video* video, DWORD flags);
    virtual void      AddPoint(const Point& v);
    virtual double    AverageLength();

    virtual void      SetWidth(double w)   { width = w; }
    virtual void      SetDim(int d)        { dim   = d; }

protected:
    int            ntrail;
    int            maxtrail;
    Point*         trail;

    double         length;
    double         width;
    int            dim;

    int            npolys, nverts;
    Poly*          polys;
    VertexSet*     verts;
    Bitmap*        texture;
    Material       mtl;

    double         length0, length1;
    double         last_point_time;
};

#endif  // Trail_h

