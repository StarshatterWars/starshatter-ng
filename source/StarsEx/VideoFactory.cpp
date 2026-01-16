/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Video and Polygon Renderer Factory class
*/

#include "VideoFactory.h"

#include "VideoDX9.h"
#include "SoundD3D.h"

// +--------------------------------------------------------------------+

VideoFactory::VideoFactory(HWND h)
: hwnd(h), video(0), audio(0)
{ }

VideoFactory::~VideoFactory()
{ }

// +--------------------------------------------------------------------+

Video*
VideoFactory::CreateVideo(VideoSettings* vs)
{
    if (!video) {
        video = (Video*) new VideoDX9(hwnd, vs);

        if (!video) {
            delete video;
            video = 0;
        }
    }

    return video;
}

// +--------------------------------------------------------------------+

void
VideoFactory::DestroyVideo(Video* v)
{
    if (v == video) {
        delete video;
        video = 0;
    }
}

// +--------------------------------------------------------------------+

SoundCard*
VideoFactory::CreateSoundCard()
{
    if (!audio) {
        audio = new SoundCardD3D(hwnd);
        Sound::UseSoundCard(audio);
    }

    return audio;
}


