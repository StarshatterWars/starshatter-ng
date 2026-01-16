/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Event Target interface class
*/

#ifndef UIEventTarget_h
#define UIEventTarget_h

#include "Types.h"
#include "Geometry.h"


class UIEventTarget
{
public:
    static const char* TYPENAME() { return "UIEventTarget"; }

    virtual ~UIEventTarget() { }

    int operator == (const UIEventTarget& t) const { return this == &t; }

    virtual int  OnMouseMove(int x, int y)          { return 0; }
    virtual int  OnLButtonDown(int x, int y)        { return 0; }
    virtual int  OnLButtonUp(int x, int y)          { return 0; }
    virtual int  OnClick()                          { return 0; }
    virtual int  OnSelect()                         { return 0; }
    virtual int  OnRButtonDown(int x, int y)        { return 0; }
    virtual int  OnRButtonUp(int x, int y)          { return 0; }
    virtual int  OnMouseEnter(int x, int y)         { return 0; }
    virtual int  OnMouseExit(int x, int y)          { return 0; }
    virtual int  OnMouseWheel(int wheel)            { return 0; }

    virtual int  OnKeyDown(int vk, int flags)       { return 0; }

    virtual void SetFocus()                         { }
    virtual void KillFocus()                        { }
    virtual bool HasFocus() const                   { return false; }

    virtual bool IsEnabled() const                  { return true; }
    virtual bool IsVisible() const                  { return true; }
    virtual bool IsFormActive() const               { return true; }

    virtual Rect TargetRect() const                 { return Rect(); }

    virtual const char* GetDescription() const      { return "UIEventTarget"; }
};


#endif  // UIEventTarget_h
