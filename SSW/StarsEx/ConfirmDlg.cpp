/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    General-purpose confirmation dialog class
*/

#include "ConfirmDlg.h"
#include "MenuScreen.h"
#include "FormatUtil.h"

#include "Game.h"
#include "Keyboard.h"
#include "Button.h"

// +--------------------------------------------------------------------+
// DECLARE MAPPING FUNCTIONS:

DEF_MAP_CLIENT(ConfirmDlg, OnApply);
DEF_MAP_CLIENT(ConfirmDlg, OnCancel);

// +--------------------------------------------------------------------+

ConfirmDlg::ConfirmDlg(Screen* s, FormDef& def, MenuScreen* mgr)
: FormWindow(s,  0,  0, s->Width(), s->Height()), manager(mgr),
parent_control(0), btn_apply(0), btn_cancel(0)
{
    Init(def);
}

ConfirmDlg::~ConfirmDlg()
{
}

void
ConfirmDlg::RegisterControls()
{
    if (btn_apply)
    return;

    btn_apply   = (Button*) FindControl(1);
    REGISTER_CLIENT(EID_CLICK, btn_apply,  ConfirmDlg, OnApply);

    btn_cancel  = (Button*) FindControl(2);
    REGISTER_CLIENT(EID_CLICK, btn_cancel, ConfirmDlg, OnCancel);

    lbl_title   = FindControl(100);
    lbl_message = FindControl(101);
}

// +--------------------------------------------------------------------+

ActiveWindow*
ConfirmDlg::GetParentControl()
{
    return parent_control;
}

void
ConfirmDlg::SetParentControl(ActiveWindow* p)
{
    parent_control = p;
}

Text
ConfirmDlg::GetTitle()
{
    if (lbl_title)
    return lbl_title->GetText();

    return "";
}

void
ConfirmDlg::SetTitle(const char* t)
{
    if (lbl_title)
    lbl_title->SetText(t);
}

Text
ConfirmDlg::GetMessage()
{
    if (lbl_message)
    return lbl_message->GetText();

    return "";
}

void
ConfirmDlg::SetMessage(const char* m)
{
    if (lbl_message)
    lbl_message->SetText(m);
}

// +--------------------------------------------------------------------+

void
ConfirmDlg::ExecFrame()
{
    if (Keyboard::KeyDown(VK_RETURN)) {
        OnApply(0);
    }

    if (Keyboard::KeyDown(VK_ESCAPE)) {
        OnCancel(0);
    }
}

// +--------------------------------------------------------------------+

void
ConfirmDlg::Show()
{
    if (!IsShown()) {
        Button::PlaySound(Button::SND_CONFIRM);
    }

    FormWindow::Show();
    SetFocus();
}

// +--------------------------------------------------------------------+

void
ConfirmDlg::OnApply(AWEvent* event)
{
    manager->HideConfirmDlg();

    if (parent_control)
    parent_control->ClientEvent(EID_USER_1);
}

void
ConfirmDlg::OnCancel(AWEvent* event)
{
    manager->HideConfirmDlg();
}

// +--------------------------------------------------------------------+
