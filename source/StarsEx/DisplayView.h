/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    View class for Radio Communications HUD Overlay
*/

#ifndef DisplayView_h
#define DisplayView_h

#include "Types.h"
#include "View.h"
#include "SimObject.h"
#include "Color.h"
#include "Text.h"

// +--------------------------------------------------------------------+

class Bitmap;
class DisplayElement;
class Font;

// +--------------------------------------------------------------------+

class DisplayView : public View
{
public:
    DisplayView(Window* c);
    virtual ~DisplayView();

    // Operations:
    virtual void      Refresh();
    virtual void      OnWindowMove();
    virtual void      ExecFrame();
    virtual void      ClearDisplay();

    virtual void      AddText(const char*  txt,
    Font*       font,
    Color       color,
    const Rect& rect,
    double      hold     = 1e9,
    double      fade_in  = 0,
    double      fade_out = 0);

    virtual void      AddImage(Bitmap*     bmp,
    Color       color,
    int         blend,
    const Rect& rect,
    double      hold     = 1e9,
    double      fade_in  = 0,
    double      fade_out = 0);

    static DisplayView* GetInstance();

protected:
    int         width, height;
    double      xcenter, ycenter;

    List<DisplayElement>
    elements;
};

#endif  // DisplayView_h

