/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
*/

#include "Terrain.h"
#include "TerrainHaze.h"
#include "TerrainRegion.h"

#include "Light.h"
#include "CameraView.h"
#include "Bitmap.h"
#include "Game.h"

// +====================================================================+

static Bitmap terrain_texture;

// +--------------------------------------------------------------------+

TerrainHaze::TerrainHaze()
: tregion(0)
{
}

// +--------------------------------------------------------------------+

TerrainHaze::~TerrainHaze()
{
}

// +--------------------------------------------------------------------+

void
TerrainHaze::Render(Video* video, DWORD flags)
{
    if (flags & RENDER_ADDITIVE)
    return;

    if (model) {
        if (!Luminous()) {
            SetLuminous(true);
            model->SetDynamic(true);
        }

        Surface* surface = model->GetSurfaces().first();

        if (!surface) return;

        int i;
        DWORD sky = 0;
        DWORD fog = 0;

        if (tregion) {
            sky = tregion->SkyColor().Value();
            fog = tregion->FogColor().Value();
        }

        // clear the solid lights to ambient:
        VertexSet* vset = surface->GetVertexSet();

        for (i = 0; i < vset->nverts; i++) {
            if (vset->loc[i].y > 0)
            vset->diffuse[i] = sky;
            else
            vset->diffuse[i] = fog;
        }

        InvalidateSurfaceData();
        Solid::Render(video, flags);
    }
}

// +--------------------------------------------------------------------+

int
TerrainHaze::CheckRayIntersection(Point Q, Point w, double len, Point& ipt, bool ttpas)
{
    return 0;
}
