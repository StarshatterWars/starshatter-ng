/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Operational Command Dialog (Intel/Newsfeed Tab)
*/

#include "CmdTitleDlg.h"
#include "CmpnScreen.h"
#include "Campaign.h"
#include "Combatant.h"
#include "CombatEvent.h"
#include "CombatGroup.h"
#include "CombatUnit.h"
#include "ShipDesign.h"

#include "Game.h"
#include "Button.h"
#include "ComboBox.h"
#include "ListBox.h"
#include "Slider.h"
#include "Video.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "ParseUtil.h"
#include "FormatUtil.h"

// +--------------------------------------------------------------------+
// DECLARE MAPPING FUNCTIONS:

// +--------------------------------------------------------------------+

CmdTitleDlg::CmdTitleDlg(Screen* s, FormDef& def, CmpnScreen* mgr)
: FormWindow(s, 0, 0, s->Width(), s->Height()), manager(mgr),
campaign(0), showTime(0)
{
    campaign = Campaign::GetCampaign();

    Init(def);
}

CmdTitleDlg::~CmdTitleDlg()
{
}

// +--------------------------------------------------------------------+

void
CmdTitleDlg::RegisterControls()
{
    img_title = (ImageBox*)  FindControl(200);
}

// +--------------------------------------------------------------------+

void
CmdTitleDlg::Show()
{
    FormWindow::Show();
}

// +--------------------------------------------------------------------+

void
CmdTitleDlg::ExecFrame()
{
}
