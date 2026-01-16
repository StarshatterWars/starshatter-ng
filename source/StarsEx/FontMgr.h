/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Font Resource Manager class
*/

#ifndef FontMgr_h
#define FontMgr_h

#include "Types.h"
#include "Color.h"
#include "List.h"
#include "Text.h"

class Font;

// +--------------------------------------------------------------------+

struct FontItem
{
    static const char* TYPENAME() { return "FontItem"; }

    Text  name;
    int   size;
    Font* font;
};

class FontMgr
{
public:
    static const char* TYPENAME() { return "FontMgr"; }

    static void    Close();
    static void    Register(const char* name, Font* font);
    static Font*   Find(const char* name);

private:
    static List<FontItem> fonts;
};

#endif  // FontMgr_h

