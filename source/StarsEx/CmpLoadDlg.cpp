/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
*/

#include "CmpLoadDlg.h"
#include "Campaign.h"
#include "Starshatter.h"
#include "FormatUtil.h"

#include "Clock.h"
#include "Video.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "ImageBox.h"
#include "Slider.h"

// +--------------------------------------------------------------------+

CmpLoadDlg::CmpLoadDlg(Screen* s, FormDef& def)
: FormWindow(s, 0, 0, s->Width(), s->Height()),
lbl_progress(0), lbl_activity(0), lbl_title(0), img_title(0), show_time(0)
{
    Init(def);
}

CmpLoadDlg::~CmpLoadDlg()
{
}

void
CmpLoadDlg::RegisterControls()
{
    img_title    = (ImageBox*) FindControl(100);
    lbl_title    =             FindControl(200);
    lbl_activity =             FindControl(101);
    lbl_progress = (Slider*)   FindControl(102);
}

void
CmpLoadDlg::Show()
{
    FormWindow::Show();

    Campaign* campaign = Campaign::GetCampaign();

    if (campaign) {
        Bitmap* bmp = campaign->GetImage(3);
        if (img_title && bmp) {
            Rect tgt_rect;
            tgt_rect.w = img_title->Width();
            tgt_rect.h = img_title->Height();

            img_title->SetTargetRect(tgt_rect);
            img_title->SetPicture(*bmp);
        }

        if (lbl_title)
        lbl_title->SetText(campaign->Name());
    }

    show_time = Clock::GetInstance()->RealTime();
}

// +--------------------------------------------------------------------+

void
CmpLoadDlg::ExecFrame()
{
    Starshatter* stars = Starshatter::GetInstance();

    if (stars) {
        if (lbl_activity) lbl_activity->SetText(stars->GetLoadActivity());
        if (lbl_progress) lbl_progress->SetValue(stars->GetLoadProgress());
    }
}

// +--------------------------------------------------------------------+

void
CmpLoadDlg::MoveTo(const Rect& r)
{
    FormWindow::MoveTo(r);

    Campaign* campaign = Campaign::GetCampaign();

    if (campaign && img_title && campaign->GetImage(3)) {
        Bitmap* bmp = campaign->GetImage(3);

        Rect tgt_rect;
        tgt_rect.w = img_title->Width();
        tgt_rect.h = img_title->Height();

        img_title->SetTargetRect(tgt_rect);
        img_title->SetPicture(*bmp);
    }
}

// +--------------------------------------------------------------------+

bool
CmpLoadDlg::IsDone()
{
    if (Clock::GetInstance()->RealTime() - show_time < 5000)
    return false;

    return true;
}
