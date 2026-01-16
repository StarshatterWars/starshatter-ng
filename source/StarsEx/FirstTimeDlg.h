/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Main Menu Dialog Active Window class
*/

#ifndef FirstTimeDlg_h
#define FirstTimeDlg_h

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

class FirstTimeDlg : public FormWindow
{
public:
    FirstTimeDlg(Screen* s, FormDef& def, MenuScreen* mgr);
    virtual ~FirstTimeDlg();

    virtual void      RegisterControls();
    virtual void      Show();
    virtual void      ExecFrame();

    // Operations:
    virtual void      OnApply(AWEvent* event);

protected:
    MenuScreen*       manager;

    EditBox*          edt_name;
    ComboBox*         cmb_playstyle;
    ComboBox*         cmb_experience;

    Button*           btn_apply;
};

#endif  // FirstTimeDlg_h

