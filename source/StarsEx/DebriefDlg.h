/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Mission Debriefing Dialog Active Window class
*/

#ifndef DebriefDlg_h
#define DebriefDlg_h

#include "Types.h"
#include "FormWindow.h"
#include "Bitmap.h"
#include "Button.h"
#include "ComboBox.h"
#include "ListBox.h"
#include "Font.h"
#include "Text.h"

// +--------------------------------------------------------------------+

class PlanScreen;
class Campaign;
class Mission;
class MissionInfo;
class Sim;
class Ship;

// +--------------------------------------------------------------------+

class DebriefDlg : public FormWindow
{
public:
    DebriefDlg(Screen* s, FormDef& def, PlanScreen* mgr);
    virtual ~DebriefDlg();

    virtual void      RegisterControls();
    virtual void      ExecFrame();
    virtual void      Show();

    // Operations:
    virtual void      OnClose(AWEvent* event);
    virtual void      OnUnit(AWEvent* event);

protected:
    virtual void      DrawUnits();

    PlanScreen*       manager;
    Button*           close_btn;

    ActiveWindow*     mission_name;
    ActiveWindow*     mission_system;
    ActiveWindow*     mission_sector;
    ActiveWindow*     mission_time_start;

    ActiveWindow*     objectives;
    ActiveWindow*     situation;
    ActiveWindow*     mission_score;

    ListBox*          unit_list;
    ListBox*          summary_list;
    ListBox*          event_list;

    Campaign*         campaign;
    Mission*          mission;
    MissionInfo*      info;
    int               unit_index;

    Sim*              sim;
    Ship*             ship;
};

#endif  // DebriefDlg_h

