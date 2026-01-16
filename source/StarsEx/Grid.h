/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Tactical Grid
*/

#ifndef Grid_h
#define Grid_h

#include "Types.h"
#include "Graphic.h"
#include "Geometry.h"

// +--------------------------------------------------------------------+

class Window;
class Projector;
class PolyRender;

// +--------------------------------------------------------------------+

class Grid : public Graphic
{
public:
    Grid(int size, int step);
    virtual ~Grid();

    virtual void      Render(Video* video, DWORD flags);
    virtual int       CollidesWith(Graphic& o);

protected:
    virtual void      DrawLine(Video* video, Point& p1, Point& p2, Color c);

    int               size;
    int               step;
    int               drawn;
};

// +--------------------------------------------------------------------+

#endif  // Grid_h

