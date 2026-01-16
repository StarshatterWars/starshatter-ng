/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Navigation Active Window class
*/

#ifndef CmdMsgDlg_h
#define CmdMsgDlg_h

#include "Types.h"
#include "FormWindow.h"

// +--------------------------------------------------------------------+

class CmpnScreen;

// +--------------------------------------------------------------------+

class CmdMsgDlg : public FormWindow
{
public:
    CmdMsgDlg(Screen* s, FormDef& def, CmpnScreen* mgr);
    virtual ~CmdMsgDlg();

    virtual void      RegisterControls();
    virtual void      Show();

    // Operations:
    virtual void      ExecFrame();
    virtual void      OnApply(AWEvent* event);

    ActiveWindow*     Title()     { return title;   }
    ActiveWindow*     Message()   { return message; }

protected:
    CmpnScreen*       manager;

    ActiveWindow*     title;
    ActiveWindow*     message;

    Button*           apply;
    bool              exit_latch;
};

#endif  // CmdMsgDlg_h

