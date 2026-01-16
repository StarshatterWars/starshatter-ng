/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Loading progress dialog box
*/


#include "LoadDlg.h"
#include "Starshatter.h"

#include "Game.h"
#include "ContentBundle.h"
#include "Video.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "ListBox.h"
#include "ComboBox.h"
#include "Slider.h"
#include "FormatUtil.h"

// +--------------------------------------------------------------------+

LoadDlg::LoadDlg(Screen* s, FormDef& def)
: FormWindow(s, 0, 0, s->Width(), s->Height()),
progress(0), activity(0)
{
    Init(def);
}

LoadDlg::~LoadDlg()
{
}

void
LoadDlg::RegisterControls()
{
    title    = FindControl(100);
    activity = FindControl(101);
    progress = (Slider*) FindControl(102);
}

// +--------------------------------------------------------------------+

void
LoadDlg::ExecFrame()
{
    Starshatter* stars = Starshatter::GetInstance();

    if (stars) {
        if (title) {
            if (stars->GetGameMode() == Game::CLOD_MODE ||
                    stars->GetGameMode() == Game::CMPN_MODE)
            title->SetText(ContentBundle::GetInstance()->GetText("LoadDlg.campaign"));

            else if (stars->GetGameMode() == Game::MENU_MODE)
            title->SetText(ContentBundle::GetInstance()->GetText("LoadDlg.tac-ref"));

            else
            title->SetText(ContentBundle::GetInstance()->GetText("LoadDlg.mission"));
        }

        activity->SetText(stars->GetLoadActivity());
        progress->SetValue(stars->GetLoadProgress());
    }
}

// +--------------------------------------------------------------------+

