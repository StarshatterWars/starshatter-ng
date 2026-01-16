/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Mod Info Splash Dialog Active Window class
*/

#ifndef ModInfoDlg_h
#define ModInfoDlg_h

#include "Types.h"
#include "FormWindow.h"
#include "Bitmap.h"
#include "Button.h"
#include "ComboBox.h"
#include "ListBox.h"
#include "Font.h"
#include "Text.h"

// +--------------------------------------------------------------------+

class BaseScreen;
class ModConfig;
class ModInfo;

// +--------------------------------------------------------------------+

class ModInfoDlg : public FormWindow
{
public:
    ModInfoDlg(Screen* s, FormDef& def, BaseScreen* mgr);
    virtual ~ModInfoDlg();

    virtual void      RegisterControls();
    virtual void      Show();
    virtual void      ExecFrame();

    // Operations:
    virtual void      SetModInfo(ModInfo* info);
    virtual void      OnAccept(AWEvent* event);

protected:
    BaseScreen*       manager;

    ActiveWindow*     lbl_name;
    ActiveWindow*     lbl_desc;
    ActiveWindow*     lbl_copy;
    ImageBox*         img_logo;

    Button*           btn_accept;

    ModInfo*          mod_info;

    Bitmap            bmp_default;
};

#endif  // ModInfoDlg_h

