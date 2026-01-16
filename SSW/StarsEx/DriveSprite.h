/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Specialized Drive Sprite Object
*/

#ifndef DriveSprite_h
#define DriveSprite_h

#include "Types.h"
#include "Geometry.h"
#include "Sprite.h"

// +--------------------------------------------------------------------+

class DriveSprite : public Sprite
{
public:
    DriveSprite();
    DriveSprite(Bitmap* animation, Bitmap* glow);
    DriveSprite(Bitmap* animation, int length=1, int repeat=1, int share=1);
    virtual ~DriveSprite();

    // operations
    virtual void   Render(Video* video, DWORD flags);
    virtual void   SetFront(const Vec3& f);
    virtual void   SetBias(DWORD b);

protected:
    double         effective_radius;
    Vec3           front;
    Bitmap*        glow;
    DWORD          bias;
};

// +--------------------------------------------------------------------+

#endif  // DriveSprite_h

