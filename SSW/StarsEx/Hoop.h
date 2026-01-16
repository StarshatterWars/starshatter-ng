/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    ILS Hoop (HUD display) class
*/

#ifndef Hoop_h
#define Hoop_h

#include "Types.h"
#include "Solid.h"
#include "Geometry.h"

// +--------------------------------------------------------------------+

class Hoop : public Solid
{
public:
    Hoop();
    virtual ~Hoop();

    virtual void      Update();
    static  void      SetColor(Color c);

protected:
    virtual void      CreatePolys();

    Bitmap*           hoop_texture;
    Material*         mtl;
    int               width;
    int               height;
};

// +--------------------------------------------------------------------+

#endif  // Hoop_h

