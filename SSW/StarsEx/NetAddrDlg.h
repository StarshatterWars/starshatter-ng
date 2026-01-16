/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network Server Address/Port Dialog Active Window class
*/

#ifndef NetAddrDlg_h
#define NetAddrDlg_h

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

class NetAddrDlg : public FormWindow
{
public:
    NetAddrDlg(Screen* s, FormDef& def, MenuScreen* mgr);
    virtual ~NetAddrDlg();

    virtual void      RegisterControls();
    virtual void      Show();

    // Operations:
    virtual void      ExecFrame();

    virtual void      OnSave(AWEvent* event);
    virtual void      OnCancel(AWEvent* event);

    void SelectConfig(int index=NEW);

protected:
    MenuScreen*       manager;

    Button*           btn_save;
    Button*           btn_cancel;
    EditBox*          edt_name;
    EditBox*          edt_addr;
    EditBox*          edt_port;
    EditBox*          edt_pass;

    static constexpr int NEW = {-1};
    int selected_config;
};

#endif  // NetAddrDlg_h

