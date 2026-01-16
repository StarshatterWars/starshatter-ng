/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    File loader for 3DStudio MAX 3DS format models
*/

#pragma once

#include <Solid.h>


class ModelFile3DS : public ModelFile
{
public:
   ModelFile3DS(const char* fname);
   virtual ~ModelFile3DS();

   virtual bool   Load(Model* m, double scale=1.0);
   virtual bool   Save(Model* m);
};
