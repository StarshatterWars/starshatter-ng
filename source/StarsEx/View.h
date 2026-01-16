/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Abstract View class
*/

#ifndef View_h
#define View_h

#include "Types.h"

// +--------------------------------------------------------------------+

class Window;

// +--------------------------------------------------------------------+

class View
{
    friend class Window;

public:
    static const char* TYPENAME() { return "View"; }

    View(Window* c) : window(c)               { }
    virtual ~View()                           { }

    int operator == (const View& that) const { return this == &that; }

    // Operations:
    virtual void      Refresh()               { }
    virtual void      OnWindowMove()          { }
    virtual void      OnShow()                { }
    virtual void      OnHide()                { }

    virtual void      SetWindow(Window* w)    { window = w; OnWindowMove(); }
    virtual Window*   GetWindow()             { return window; }

protected:
    Window*  window;
};

#endif  // View_h

