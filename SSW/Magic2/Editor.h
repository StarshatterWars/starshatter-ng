/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2022, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Classes for rendering solid meshes of polygons
*/

#ifndef Editor_h
#define Editor_h

#include "MagicDoc.h"
#include "Command.h"
#include "Polygon.h"
#include "Solid.h"
#include "Video.h"
#include "List.h"

// +--------------------------------------------------------------------+

class Selection;
class ModelView;

// +--------------------------------------------------------------------+

class Editor
{
public:
   Editor(MagicDoc* doc) : document(doc), model(0) { }

   // accessors / mutators
   void     UseModel(Model* m)   { model = m;      }
   Model*   GetModel()     const { return model;   }

   // operations

   enum { MAP_NONE, MAP_PLANAR, MAP_CYLINDRICAL, MAP_SPHERICAL, MAP_STRETCH };

   void     ApplyMaterial(Material* material, List<Poly>& polys,
                    int mapping, int axis, float scale_u, float scale_v,
                    int flip, int mirror, int rotate);

   void     ApplyMaterialCylindrical(Material* material, List<Poly>& polys,
                    int axis, float scale_u, float scale_v,
                    int flip, int mirror, int rotate);

   void     ApplyMaterialSpherical(Material* material, List<Poly>& polys,
                    int axis, float scale_u, float scale_v,
                    int flip, int mirror, int rotate);


   void     Resegment();

protected:
   MagicDoc*   document;
   Model*      model;
};

// +--------------------------------------------------------------------+

class EditCommand : public Command
{
public:
   EditCommand(const char* name, MagicDoc* doc);
   virtual ~EditCommand();

   virtual void   Do();
   virtual void   Undo();

private:
   Model*   model1;
   Model*   model2;
};

// +--------------------------------------------------------------------+

#endif Editor_h

