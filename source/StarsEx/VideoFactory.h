/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Video Factory class
*/

#ifndef VideoFactory_h
#define VideoFactory_h

#include "Types.h"
#include "Video.h"
#include "SoundCard.h"

// +--------------------------------------------------------------------+

class VideoFactory
{
public:
    VideoFactory(HWND h);
    virtual ~VideoFactory();

    virtual Video*       CreateVideo(VideoSettings* vs);
    virtual void         DestroyVideo(Video* video);
    virtual SoundCard*   CreateSoundCard();

private:
    HWND        hwnd;

    Video*      video;
    SoundCard*  audio;
};

#endif  // VideoFactory_h

