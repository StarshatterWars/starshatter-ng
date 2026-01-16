/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2022, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Implementation of the MagicDoc class
*/

#include "StdAfx.h"
#include "Magic.h"

#include "MagicDoc.h"
#include "MagicLoad.h"
#include "ModelFileMAG.h"
#include "ModelFileOBJ.h"
#include "ModelFile3DS.h"
#include "Selection.h"
#include "Selector.h"
#include "Editor.h"
#include "Command.h"
#include "D3DXImage.h"
#include "Geometry.h"
#include "Polygon.h"
#include "Solid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// +--------------------------------------------------------------------+

IMPLEMENT_DYNCREATE(MagicDoc, CDocument)

BEGIN_MESSAGE_MAP(MagicDoc, CDocument)
    //{{AFX_MSG_MAP(MagicDoc)
    ON_COMMAND(ID_SURFACE_OPTIMIZE, OnSurfaceOptimize)
    ON_COMMAND(ID_SURFACE_EXPLODE, OnSurfaceExplode)
    ON_UPDATE_COMMAND_UI(ID_SURFACE_OPTIMIZE, OnUpdateSurfaceOptimize)
    ON_UPDATE_COMMAND_UI(ID_SURFACE_EXPLODE, OnUpdateSurfaceExplode)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// +--------------------------------------------------------------------+

MagicDoc::MagicDoc()
  : solid(0), selection(0)
{
   solid     = new Solid;
   selection = new Selection;
   selector  = new Selector(selection);
   editor    = new Editor(this);
}

MagicDoc::~MagicDoc()
{
   if (editor)    delete editor;
   if (selector)  delete selector;
   if (selection) delete selection;
   if (solid)     delete solid;
}

// +--------------------------------------------------------------------+

void
MagicDoc::InitCommandStack()
{
   nundo = 0;
   commands.destroy();
}

void
MagicDoc::Exec(Command* command)
{
   int nredo = commands.size() - nundo;

   while (nredo) {
      delete commands.removeIndex(commands.size()-1);
      nredo--;
   }

   if (nundo < 100) {
      nundo++;
   }
   else {
      delete commands.removeIndex(0);
   }

   command->Do();
   commands.append(command);
}

int
MagicDoc::NumUndo() const
{
   return nundo;
}

int
MagicDoc::NumRedo() const
{
   return commands.size() - nundo;
}

const char*
MagicDoc::GetUndoName() const
{
   if (nundo > 0 && nundo <= commands.size())
      return commands[nundo-1]->Name();
   else
      return "";
}

const char*
MagicDoc::GetRedoName() const
{
   if (nundo >= 0 && nundo < commands.size())
      return commands[nundo]->Name();
   else
      return "";
}

void
MagicDoc::Undo()
{
   if (nundo > 0 && nundo <= commands.size())
      commands[--nundo]->Undo();
}

void
MagicDoc::Redo()
{
   if (nundo >= 0 && nundo < commands.size())
      commands[nundo++]->Do();
}

// +--------------------------------------------------------------------+

BOOL MagicDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

   InitCommandStack();

   if (solid) delete solid;
   solid = new Solid;

   if (selection)
      selection->Clear();

    return TRUE;
}

// +--------------------------------------------------------------------+

void MagicDoc::Serialize(CArchive& ar)
{
}

// +--------------------------------------------------------------------+

#ifdef _DEBUG
void MagicDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void MagicDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

// +--------------------------------------------------------------------+

BOOL MagicDoc::OnSaveDocument(LPCTSTR path_name)
{
   SetModifiedFlag(FALSE);

   ModelFileMAG mod_file(path_name);
   mod_file.Save(solid->GetModel());

   SetModifiedFlag(FALSE);
   UpdateAllViews(NULL);
   return TRUE;
}

BOOL MagicDoc::OnOpenDocument(LPCTSTR path_name)
{
   FILE* fp = fopen(path_name, "rb");
   if (!fp) {
      ::MessageBox(0, "Open Failed:  could not open file", "ERROR", MB_OK);
      return FALSE;
   }

   int version = 1;
   char file_id[5];
   fread(file_id, 4, 1, fp);
   file_id[4] = '\0';
   fclose(fp);

   if (strncmp(file_id, "MAG", 3)) {
      ::MessageBox(0, "Open Failed:  Invalid file type", "ERROR", MB_OK);
      return FALSE;
   }

   switch (file_id[3]) {
   case '6':   version = 6;   break;
   case '5':   version = 5;   break;
   default:    version = 0;   break;
   }

   if (version < 5 || version > 6) {
      ::MessageBox(0, "Open Failed:  Unsupported version", "ERROR", MB_OK);
      return FALSE;
   }

   DeleteContents();

   ModelFileMAG mod_file(path_name);
   solid->Load(&mod_file);
   solid->CreateShadows();

   SetModifiedFlag(FALSE);
   UpdateAllViews(NULL);
   return TRUE;
}


bool
MagicDoc::ImportFile(LPCTSTR path_name)
{
   if (strstr(path_name, ".obj") || strstr(path_name, ".OBJ")) {
      if (!ImportInto<ModelFileOBJ>(path_name, solid))
         return false;
      SetModifiedFlag(FALSE);
      UpdateAllViews(NULL);
      return true;
   }
   if (strstr(path_name, ".3ds") || strstr(path_name, ".3DS")) {
      if (!ImportInto<ModelFile3DS>(path_name, solid))
         return false;
      SetModifiedFlag(FALSE);
      UpdateAllViews(NULL);
      return true;
   }
   if (!ImportInto<ModelFile3DS>(path_name, solid))
      return false;
   InitCommandStack();
   SetModifiedFlag(FALSE);
   UpdateAllViews(NULL);
   return true;
}

bool
MagicDoc::ExportFile(LPCTSTR path_name)
{
   if (!solid->GetModel())
      return false;

   if (strstr(path_name, ".obj") || strstr(path_name, ".OBJ")) {
      ModelFileOBJ obj_file(path_name);
      obj_file.Save(solid->GetModel());
      return true;
   }

   if (strstr(path_name, ".3ds") || strstr(path_name, ".3DS")) {
      return false;
   }

   if (strstr(path_name, ".mag") || strstr(path_name, ".MAG")) {
      ModelFileMAG mod_file(path_name);
      mod_file.Save(solid->GetModel());
      return true;
   }

   return false;
}

// +--------------------------------------------------------------------+

void MagicDoc::DeleteContents()
{
    CDocument::DeleteContents();
   InitCommandStack();

   if (solid) {
      delete solid;
      solid = new Solid;
   }

   if (selection)
      selection->Clear();
}

void MagicDoc::OnSurfaceOptimize()
{
   if (solid && solid->GetModel()) {
      solid->GetModel()->OptimizeMesh();
      solid->InvalidateSurfaceData();
      solid->InvalidateSegmentData();
   }
}

void MagicDoc::OnUpdateSurfaceOptimize(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(solid && solid->GetModel());
}

void MagicDoc::OnSurfaceExplode()
{
   if (solid && solid->GetModel()) {
      solid->GetModel()->ExplodeMesh();
      solid->InvalidateSurfaceData();
      solid->InvalidateSegmentData();
   }
}

void MagicDoc::OnUpdateSurfaceExplode(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(solid && solid->GetModel());
}
