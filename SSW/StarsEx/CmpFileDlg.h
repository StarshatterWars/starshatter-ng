/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Mission Select Dialog Active Window class
*/

#ifndef CmpFileDlg_h
#define CmpFileDlg_h

#include "Types.h"
#include "FormWindow.h"
#include "Bitmap.h"
#include "Button.h"
#include "ComboBox.h"
#include "ListBox.h"
#include "Font.h"
#include "Text.h"

// +--------------------------------------------------------------------+

class  CmpnScreen;
class  Campaign;
class  Starshatter;

// +--------------------------------------------------------------------+

class CmpFileDlg : public FormWindow
{
public:
    CmpFileDlg(Screen* s, FormDef& def, CmpnScreen* mgr);
    virtual ~CmpFileDlg();

    virtual void      RegisterControls();
    virtual void      Show();

    // Operations:
    virtual void      ExecFrame();

    virtual void      OnSave(AWEvent* event);
    virtual void      OnCancel(AWEvent* event);
    virtual void      OnCampaign(AWEvent* event);

protected:
    CmpnScreen*       manager;
    Campaign*         campaign;

    Button*           btn_save;
    Button*           btn_cancel;
    EditBox*          edt_name;
    ListBox*          lst_campaigns;

    bool              exit_latch;
};

#endif  // CmpFileDlg_h

