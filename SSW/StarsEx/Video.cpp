/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Abstract Video Interface (singleton definition)
*/

#include "Video.h"
#include "VideoSettings.h"

// +--------------------------------------------------------------------+

Video* Video::video_instance = 0;

// +--------------------------------------------------------------------+

Video::Video()
{
    status = VIDEO_OK;
    video_instance = this;

    shadow_enabled = true;
    bump_enabled   = true;
    spec_enabled   = true;

    camera         = 0;
}

Video::~Video()
{
    if (video_instance == this)
    video_instance = 0;
}

// +--------------------------------------------------------------------+

bool
Video::IsWindowed() const
{
    const VideoSettings* vs = GetVideoSettings();

    if (vs)
    return vs->IsWindowed();

    return false;
}

bool
Video::IsFullScreen() const
{
    const VideoSettings* vs = GetVideoSettings();

    if (vs)
    return !vs->IsWindowed();

    return true;
}
