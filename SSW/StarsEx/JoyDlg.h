/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Navigation Active Window class
*/

#ifndef JoyDlg_h
#define JoyDlg_h

#include "Types.h"
#include "FormWindow.h"

// +--------------------------------------------------------------------+

class BaseScreen;

// +--------------------------------------------------------------------+

class JoyDlg : public FormWindow
{
public:
    JoyDlg(Screen* s, FormDef& def, BaseScreen* mgr);
    virtual ~JoyDlg();

    virtual void      RegisterControls();
    virtual void      Show();

    // Operations:
    virtual void      ExecFrame();

    virtual void      OnApply(AWEvent* event);
    virtual void      OnCancel(AWEvent* event);

    virtual void      OnAxis(AWEvent* event);

protected:
    BaseScreen*       manager;

    ActiveWindow*     message;
    Button*           axis_button[4];
    Button*           invert_checkbox[4];

    Button*           apply;
    Button*           cancel;
};

#endif  // JoyDlg_h

