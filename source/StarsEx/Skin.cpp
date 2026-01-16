/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Classes for rendering solid meshes of polygons
*/

#include "Skin.h"
#include "Solid.h"
#include "Utils.h"

// +--------------------------------------------------------------------+

Skin::Skin(const char* n)
{
    if (n && *n) {
        strncpy_s(name, n, NAMELEN);
        name[NAMELEN-1] = 0;
    }

    else {
        ZeroMemory(name, NAMELEN);
    }
}

// +--------------------------------------------------------------------+

Skin::~Skin()
{
    cells.destroy();
}

// +--------------------------------------------------------------------+

void
Skin::SetName(const char* n)
{
    if (n && *n) {
        strncpy_s(name, n, NAMELEN);
        name[NAMELEN-1] = 0;
    }
}

void
Skin::SetSource(int src)
{
    source = src;
}

// +--------------------------------------------------------------------+

void
Skin::AddMaterial(const Material* mtl)
{
    if (!mtl) return;

    bool found = false;

    ListIter<SkinCell> iter = cells;
    while (++iter && !found) {
        SkinCell* s = iter.value();

        if (s->skin && !strcmp(s->skin->name, mtl->name)) {
            s->skin = mtl;
            found   = true;
        }
    }

    if (!found) {
        SkinCell* s = new SkinCell(mtl);
        cells.append(s);
    }
}

// +--------------------------------------------------------------------+

void
Skin::ApplyTo(Model* model) const
{
    if (model) {
        for (int i = 0; i < cells.size(); i++) {
            SkinCell* s = cells[i];

            if (s->skin) {
                s->orig = model->ReplaceMaterial(s->skin);
            }
        }
    }
}

void
Skin::Restore(Model* model) const
{
    if (model) {
        for (int i = 0; i < cells.size(); i++) {
            SkinCell* s = cells[i];

            if (s->orig) {
                model->ReplaceMaterial(s->orig);
                s->orig = 0;
            }
        }
    }
}

// +--------------------------------------------------------------------+
// +--------------------------------------------------------------------+
// +--------------------------------------------------------------------+

SkinCell::SkinCell(const Material* mtl)
    : skin(mtl), orig(0)
{
}

SkinCell::~SkinCell()
{
    delete skin;
}

// +--------------------------------------------------------------------+

int
SkinCell::operator == (const SkinCell& other) const
{
    if (skin == other.skin)
    return true;

    if (skin && other.skin)
    return !strcmp(skin->name, other.skin->name);

    return false;
}

// +--------------------------------------------------------------------+

const char*
SkinCell::Name() const
{
    if (skin)
    return skin->name;

    return "Invalid Skin Cell";
}

// +--------------------------------------------------------------------+

void
SkinCell::SetSkin(const Material* mtl)
{
    skin = mtl;
}

void
SkinCell::SetOrig(const Material* mtl)
{
    orig = mtl;
}
