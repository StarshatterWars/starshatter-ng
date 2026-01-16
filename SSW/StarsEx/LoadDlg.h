/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Loading progress dialog box
*/

#ifndef LoadDlg_h
#define LoadDlg_h

#include "Types.h"
#include "FormWindow.h"

// +--------------------------------------------------------------------+

class LoadDlg : public FormWindow
{
public:
    LoadDlg(Screen* s, FormDef& def);
    virtual ~LoadDlg();

    virtual void      RegisterControls();

    // Operations:
    virtual void      ExecFrame();

protected:
    ActiveWindow*     title;
    ActiveWindow*     activity;
    Slider*           progress;
};

#endif  // LoadDlg_h

