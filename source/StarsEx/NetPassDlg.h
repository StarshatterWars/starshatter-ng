/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network Server Password Dialog Active Window class
*/

#ifndef NetPassDlg_h
#define NetPassDlg_h

#include "Types.h"
#include "FormWindow.h"
#include "Bitmap.h"
#include "Button.h"
#include "ComboBox.h"
#include "ListBox.h"
#include "Font.h"
#include "Text.h"

// +--------------------------------------------------------------------+

class MenuScreen;

// +--------------------------------------------------------------------+

class NetPassDlg : public FormWindow
{
public:
    NetPassDlg(Screen* s, FormDef& def, MenuScreen* mgr);
    virtual ~NetPassDlg();

    virtual void      RegisterControls();
    virtual void      Show();

    // Operations:
    virtual void      ExecFrame();

    virtual void      OnApply(AWEvent* event);
    virtual void      OnCancel(AWEvent* event);

protected:
    MenuScreen*       manager;

    Button*           btn_apply;
    Button*           btn_cancel;
    EditBox*          edt_pass;
    ActiveWindow*     lbl_name;
};

#endif  // NetPassDlg_h

