/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Direct 3D Texture Cube for Env Mapping
*/

#ifndef TexCubeDX9_h
#define TexCubeDX9_h

#include "Bitmap.h"

// +--------------------------------------------------------------------+

class  Video;
class  VideoDX9;
class  Bitmap;
struct VD3D_texture_format;

// +--------------------------------------------------------------------+

class TexCubeDX9
{
public:
    TexCubeDX9(VideoDX9* video);
    virtual ~TexCubeDX9();

    IDirect3DCubeTexture9*  GetTexture();
    bool                    LoadTexture(Bitmap* bmp, int face);

private:
    VideoDX9*               video;
    IDirect3D9*             d3d;
    IDirect3DDevice9*       d3ddevice;

    IDirect3DCubeTexture9*  texture;
    Bitmap*                 faces[6];
    DWORD                   last_modified[6];
};

#endif // TexCubeDX9_h

