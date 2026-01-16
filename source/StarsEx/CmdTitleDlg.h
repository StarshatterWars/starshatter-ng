/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Campaign Title Card
*/

#ifndef CmdTitleDlg_h
#define CmdTitleDlg_h

#include "Types.h"
#include "FormWindow.h"
#include "Bitmap.h"
#include "Button.h"
#include "ImageBox.h"
#include "ListBox.h"
#include "Font.h"
#include "Text.h"

// +--------------------------------------------------------------------+

class CmpnScreen;
class Campaign;
class Starshatter;

// +--------------------------------------------------------------------+

class CmdTitleDlg : public FormWindow
{
public:
    CmdTitleDlg(Screen* s, FormDef& def, CmpnScreen* mgr);
    virtual ~CmdTitleDlg();

    virtual void      RegisterControls();
    virtual void      Show();
    virtual void      ExecFrame();

protected:
    CmpnScreen*       manager;

    ImageBox*         img_title;

    Campaign*         campaign;
    double            showTime;
};

#endif  // CmdTitleDlg_h

