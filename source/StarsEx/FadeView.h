/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Non-rendering view class that controls the fade level (fade-in/fade-out)
*/

#ifndef FadeView_h
#define FadeView_h

#include "Types.h"
#include "View.h"

// +--------------------------------------------------------------------+

class FadeView : public View
{
public:
    static const char* TYPENAME() { return "FadeView"; }

    enum FadeState { StateStart, State2, StateIn, StateHold, StateOut, StateDone };

    FadeView(Window* c, double fade_in=1, double fade_out=1, double hold_time=4);
    virtual ~FadeView();

    // Operations:
    virtual void      Refresh();
    virtual bool      Done()    const   { return state == StateDone; }
    virtual bool      Holding() const   { return state == StateHold; }

    // Control:
    virtual void      FastFade(int fade_fast);
    virtual void      FadeIn(double fade_in);
    virtual void      FadeOut(double fade_out);
    virtual void      StopHold();

protected:
    double      fade_in;
    double      fade_out;
    double      hold_time;
    double      step_time;

    int         fast;
    FadeState   state;
};

#endif  // FadeView_h

