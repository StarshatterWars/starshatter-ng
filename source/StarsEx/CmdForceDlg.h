/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Operational Command Dialog (Order of Battle Tab)
*/

#ifndef CmdForceDlg_h
#define CmdForceDlg_h

#include "Types.h"
#include "FormWindow.h"
#include "CmdDlg.h"
#include "Bitmap.h"
#include "Button.h"
#include "ComboBox.h"
#include "ListBox.h"
#include "Font.h"
#include "Text.h"

// +--------------------------------------------------------------------+

class CmdForceDlg : public FormWindow,
public CmdDlg
{
public:
    CmdForceDlg(Screen* s, FormDef& def, CmpnScreen* mgr);
    virtual ~CmdForceDlg();

    virtual void      RegisterControls();
    virtual void      Show();
    virtual void      ExecFrame();

    // Operations:
    virtual void      OnMode(AWEvent* event);
    virtual void      OnSave(AWEvent* event);
    virtual void      OnExit(AWEvent* event);
    virtual void      OnForces(AWEvent* event);
    virtual void      OnCombat(AWEvent* event);
    virtual void      OnTransfer(AWEvent* event);

protected:
    void              ShowCombatant(Combatant* c);
    void              AddCombatGroup(CombatGroup* grp, bool last_child=false);
    bool              CanTransfer(CombatGroup* grp);
    bool              IsVisible(Combatant* c);

    CmpnScreen*       manager;

    ComboBox*         cmb_forces;
    ListBox*          lst_combat;
    ListBox*          lst_desc;
    Button*           btn_transfer;

    Campaign*         campaign;
    CombatGroup*      current_group;
    CombatUnit*       current_unit;
};

#endif  // CmdForceDlg_h

