/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Direct 3D Texture Cache
*/

#ifndef TexDX9_h
#define TexDX9_h

#include <cstdint>

#include "Bitmap.h"

// +--------------------------------------------------------------------+

class  Video;
class  VideoDX9;
class  Bitmap;
struct VD3D_texture_format;

// +--------------------------------------------------------------------+

struct TexCacheDX9Entry
{
    TexCacheDX9Entry() : bitmap_id(0), texture(0), used_last(0),
    last_modified(0), normal(false) { }

    void Release();
    void Unload();

    std::uint64_t        bitmap_id;
    IDirect3DTexture9*   texture;
    DWORD                used_last;
    DWORD                last_modified;
    bool                 normal;
};

// +--------------------------------------------------------------------+

class TexCacheDX9
{
public:
    TexCacheDX9(VideoDX9* video);
    virtual ~TexCacheDX9();

    void                 FrameNumber(int n) { frame_number = n; }
    IDirect3DTexture9*   FindTexture(Bitmap* bmp);
    IDirect3DTexture9*   FindNormalMap(Bitmap* bmp, float amp=1);
    bool                 LoadTexture(Bitmap* bmp, TexCacheDX9Entry* entry);
    void                 InvalidateCache();

    int                  count;

private:
    int                  FreeLRU(int tries=4);
    int                  FreeUpCache();
    void                 CreateNormalMap(int index, float amp=1);

    VideoDX9*            video;
    IDirect3D9*          d3d;
    IDirect3DDevice9*    d3ddevice;

    DWORD                vidmem;

    int                  bad_frame;
    DWORD                frame_number;

    int                  mru;
    TexCacheDX9Entry*    cache;
};

#endif // TexDX9_h

