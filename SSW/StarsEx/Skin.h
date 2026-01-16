/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Classes for managing run-time selectable skins on solid objects
*/

#ifndef Skin_h
#define Skin_h

#include "Polygon.h"
#include "Graphic.h"
#include "Video.h"
#include "List.h"

// +--------------------------------------------------------------------+

class Solid;
class Model;
class Surface;
class Segment;

class Skin;
class SkinCell;

// +--------------------------------------------------------------------+

class Skin
{
public:
    static const char* TYPENAME() { return "Skin"; }
    enum            { NAMELEN=64 };

    Skin(const char* name = 0);
    virtual ~Skin();

    // operations
    void              ApplyTo(Model* model)   const;
    void              Restore(Model* model)   const;

    // accessors / mutators
    const char*       Name()                  const { return name;          }
    int               NumCells()              const { return cells.size();  }
    int               Source()                const { return source;        }

    void              SetName(const char* n);
    void              SetSource(int src);
    void              AddMaterial(const Material* mtl);

protected:
    char              name[NAMELEN];
    List<SkinCell>    cells;
    int               source;
};

// +--------------------------------------------------------------------+

class SkinCell
{
    friend class Skin;

public:
    static const char* TYPENAME() { return "SkinCell"; }

    SkinCell(const Material* mtl=0);
    ~SkinCell();

    int operator == (const SkinCell& other) const;

    const char*       Name()         const;
    const Material*   Skin()         const { return skin; }
    const Material*   Orig()         const { return orig; }

    void              SetSkin(const Material* mtl);
    void              SetOrig(const Material* mtl);

private:
    const Material*   skin;
    const Material*   orig;
};

// +--------------------------------------------------------------------+

#endif  // Skin_h

