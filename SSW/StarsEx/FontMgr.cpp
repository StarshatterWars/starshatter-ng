/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Font Resource Manager class implementation
*/

#include "FontMgr.h"

// +--------------------------------------------------------------------+

List<FontItem> FontMgr::fonts;

// +--------------------------------------------------------------------+

void
FontMgr::Close()
{
    fonts.destroy();
}

// +--------------------------------------------------------------------+

void
FontMgr::Register(const char* name, Font* font)
{
    FontItem* item = new FontItem;

    if (item) {
        item->name = name;
        item->size = 0;
        item->font = font;

        fonts.append(item);
    }
}

// +--------------------------------------------------------------------+

Font*
FontMgr::Find(const char* name)
{
    ListIter<FontItem> item = fonts;
    while (++item) {
        if (item->name == name)
        return item->font;
    }

    return 0;
}
