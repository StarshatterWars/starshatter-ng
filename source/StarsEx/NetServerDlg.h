/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Main Menu Dialog Active Window class
*/

#ifndef NetServerDlg_h
#define NetServerDlg_h

#include "Types.h"
#include "FormWindow.h"
#include "Bitmap.h"
#include "Button.h"
#include "ComboBox.h"
#include "ListBox.h"
#include "Font.h"

// +--------------------------------------------------------------------+

class MenuScreen;
class NetServerConfig;

// +--------------------------------------------------------------------+

class NetServerDlg : public FormWindow
{
public:
    NetServerDlg(Screen* s, FormDef& def, MenuScreen* mgr);
    virtual ~NetServerDlg();

    virtual void      RegisterControls();
    virtual void      Show();
    virtual void      ExecFrame();

    // Operations:
    virtual void      OnApply(AWEvent* event);
    virtual void      OnCancel(AWEvent* event);

protected:
    MenuScreen*       manager;
    NetServerConfig*  config;

    EditBox*          edt_name;
    ComboBox*         cmb_type;
    EditBox*          edt_game_port;
    EditBox*          edt_admin_port;
    EditBox*          edt_game_pass;
    EditBox*          edt_admin_name;
    EditBox*          edt_admin_pass;

    Button*           btn_apply;
    Button*           btn_cancel;
};

#endif  // NetServerDlg_h

