/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Quantum Warp Out special effect class
*/

#ifndef QuantumFlash_h
#define QuantumFlash_h

#include "Types.h"
#include "Geometry.h"
#include "Graphic.h"
#include "Polygon.h"
#include "SimObject.h"

// +--------------------------------------------------------------------+

class QuantumFlash : public Graphic
{
public:
    QuantumFlash();
    virtual ~QuantumFlash();

    // operations
    virtual void   Render(Video* video, DWORD flags);

    // accessors / mutators
    virtual void   SetOrientation(const Matrix& o);
    void           SetDirection(const Point& v);
    void           SetEndPoints(const Point& from, const Point& to);

    double         Shade()     const    { return shade;   }
    void           SetShade(double s);

protected:
    void           UpdateVerts(const Point& cam_pos);

    double         length;
    double         width;
    double         shade;

    int            npolys, nverts;
    Material*      mtl;
    VertexSet*     verts;
    Poly*          polys;
    Matrix*        beams;
    Bitmap*        texture;

    Matrix         orientation;
};

#endif  // QuantumFlash_h
