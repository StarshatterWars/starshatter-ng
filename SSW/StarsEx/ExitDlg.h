/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Navigation Active Window class
*/

#ifndef ExitDlg_h
#define ExitDlg_h

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

class ExitDlg : public FormWindow
{
public:
    ExitDlg(Screen* s, FormDef& def, MenuScreen* mgr);
    virtual ~ExitDlg();

    virtual void      RegisterControls();
    virtual void      Show();

    // Operations:
    virtual void      ExecFrame();

    virtual void      OnApply(AWEvent* event);
    virtual void      OnCancel(AWEvent* event);

protected:
    MenuScreen*       manager;

    RichTextBox*      credits;
    Button*           apply;
    Button*           cancel;
    Rect              def_rect;

    bool              exit_latch;
};

#endif  // ExitDlg_h

