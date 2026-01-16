/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    General Screen class - maintains and displays a list of windows
*/

#ifndef Screen_h
#define Screen_h

#include "Color.h"
#include "List.h"
#include "Video.h"

// +--------------------------------------------------------------------+

class  Bitmap;
class  Window;
struct Rect;

// +--------------------------------------------------------------------+

class Screen
{
public:
    static const char* TYPENAME() { return "Screen"; }

    Screen(Video* v);
    virtual ~Screen();

    virtual bool      SetBackgroundColor(Color c);

    virtual bool      Resize(int w, int h);
    virtual bool      Refresh();
    virtual bool      AddWindow(Window* c);
    virtual bool      DelWindow(Window* c);

    int               Width()     const { return width;   }
    int               Height()    const { return height;  }

    virtual void      ClearAllFrames(bool clear_all);
    virtual void      ClearNextFrames(int num_frames);

    virtual Video*    GetVideo()  const { return video;   }

protected:
    int               width;
    int               height;
    int               clear;
    int               closed;

    Video*            video;

    List<Window>      window_list;
};

#endif  // Screen_h

