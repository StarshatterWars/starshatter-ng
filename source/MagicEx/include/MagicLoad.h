/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#pragma once

#include <Bitmap.h>
#include <Solid.h>


int LoadTexture(const char* name, Bitmap*& bmp, int type=0);
int LoadAlpha(const char* name, Bitmap& bitmap, int type=0);
template <typename ModelLoader> bool ImportInto(const char* pathname, Solid* target);


#include "MagicLoad.inl.h"
