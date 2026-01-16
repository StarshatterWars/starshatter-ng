/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Campaign title card and load progress dialog
*/

#pragma once

#include <cstdint>

#include "FormWindow.h"

// +--------------------------------------------------------------------+

class CmpLoadDlg : public FormWindow
{
public:
    CmpLoadDlg(Screen* s, FormDef& def);
    virtual ~CmpLoadDlg();

    // Operations:
    virtual void      ExecFrame();
    virtual void      MoveTo(const Rect& r);
    virtual void      RegisterControls();
    virtual void      Show();

    virtual bool      IsDone();

protected:
    ActiveWindow*     lbl_activity;
    Slider*           lbl_progress;
    ActiveWindow*     lbl_title;
    ImageBox*         img_title;
    std::uint32_t     show_time;
};
