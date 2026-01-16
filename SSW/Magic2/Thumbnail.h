/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2022, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Function declarations for thumbnail img previewer
*/

#ifndef Thumbnail_h
#define Thumbnail_h

#ifndef __AFXWIN_H__
   #error include 'StdAfx.h' before including 'Thumbnail.h' file for PCH
#endif

// +--------------------------------------------------------------------+

class Bitmap;

// +--------------------------------------------------------------------+

void ThumbPreview(HWND hprev, Bitmap* bitmap);

#endif Thumbnail_h

