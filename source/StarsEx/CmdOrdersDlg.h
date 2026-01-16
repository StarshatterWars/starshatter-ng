/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Operational Command Dialog (Campaign Orders Tab)
*/

#ifndef CmdOrdersDlg_h
#define CmdOrdersDlg_h

#include "Types.h"
#include "FormWindow.h"
#include "CmdDlg.h"
#include "Bitmap.h"
#include "Button.h"
#include "ComboBox.h"
#include "ListBox.h"
#include "Font.h"
#include "Text.h"

// +--------------------------------------------------------------------+

class CmdOrdersDlg : public FormWindow,
public CmdDlg
{
public:
    CmdOrdersDlg(Screen* s, FormDef& def, CmpnScreen* mgr);
    virtual ~CmdOrdersDlg();

    virtual void      RegisterControls();
    virtual void      ExecFrame();
    virtual void      Show();

    // Operations:
    virtual void      OnMode(AWEvent* event);
    virtual void      OnSave(AWEvent* event);
    virtual void      OnExit(AWEvent* event);

protected:
    CmpnScreen*       manager;

    ActiveWindow*     lbl_orders;

    Campaign*         campaign;
};

#endif  // CmdOrdersDlg_h

