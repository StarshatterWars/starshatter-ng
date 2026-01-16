/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Event Dispatch class
*/

#ifndef UIEventDispatch_h
#define UIEventDispatch_h

#include "List.h"
#include "Types.h"
#include "UIEventTarget.h"


class UIEventDispatch
{
public:
    static const char* TYPENAME() { return "UIEventDispatch"; }

    UIEventDispatch();
    virtual ~UIEventDispatch();

    static void    Create();
    static void    Close();
    static UIEventDispatch* GetInstance()  { return dispatcher; }

    virtual void   Dispatch();
    virtual void   Register(UIEventTarget* tgt);
    virtual void   Unregister(UIEventTarget* tgt);

    virtual UIEventTarget* GetCapture();
    virtual int    CaptureMouse(UIEventTarget* tgt);
    virtual int    ReleaseMouse(UIEventTarget* tgt);

    virtual UIEventTarget* GetFocus();
    virtual void   SetFocus(UIEventTarget* tgt);
    virtual void   KillFocus(UIEventTarget* tgt);

    virtual void   MouseEnter(UIEventTarget* tgt);

protected:
    int                     mouse_x, mouse_y, mouse_l, mouse_r;
    List<UIEventTarget>       clients;
    UIEventTarget*            capture;
    UIEventTarget*            current;
    UIEventTarget*            focus;
    UIEventTarget*            click_tgt;

    static UIEventDispatch*   dispatcher;
};


#endif  // UIEventDispatch_h
