/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Main Menu Dialog Active Window class
*/

#ifndef AwardShowDlg_h
#define AwardShowDlg_h

#include "Types.h"
#include "FormWindow.h"
#include "Bitmap.h"
#include "Button.h"
#include "ComboBox.h"
#include "ListBox.h"
#include "Font.h"

// +--------------------------------------------------------------------+

class MenuScreen;

// +--------------------------------------------------------------------+

class AwardShowDlg : public FormWindow
{
public:
    AwardShowDlg(Screen* s, FormDef& def, MenuScreen* mgr);
    virtual ~AwardShowDlg();

    virtual void      RegisterControls();
    virtual void      Show();
    virtual void      ExecFrame();

    // Operations:
    virtual void      OnClose(AWEvent* event);
    virtual void      ShowAward();
    virtual void      SetRank(int r);
    virtual void      SetMedal(int r);

protected:
    MenuScreen*       manager;

    ActiveWindow*     lbl_name;
    ActiveWindow*     lbl_info;
    ImageBox*         img_rank;
    Button*           btn_close;

    bool              exit_latch;

    int               rank;
    int               medal;
};

#endif  // AwardShowDlg_h

