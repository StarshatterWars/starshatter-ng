/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    D3DX image file loader
*/

#ifndef D3DXImage_h
#define D3DXImage_h

#include "Types.h"

// +--------------------------------------------------------------------+

struct D3DXImage
{
    static const char* TYPENAME() { return "D3DXImage"; }

    D3DXImage();
    D3DXImage(WORD w, WORD h, DWORD* img);
    ~D3DXImage();

    bool Load(char *filename);
    bool Save(char *filename);

    bool LoadBuffer(BYTE* buf, int len);

    DWORD*         image;
    DWORD          width;
    DWORD          height;
    DWORD          format;
};

// +--------------------------------------------------------------------+

#endif  // D3DXImage_h
