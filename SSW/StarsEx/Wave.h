/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo

*/

#ifndef Wave_h
#define Wave_h

#include "Types.h"

// +--------------------------------------------------------------------+

struct WAVE_HEADER
{
    DWORD RIFF;
    DWORD file_len;
    DWORD WAVE;
};

struct WAVE_FMT
{
    DWORD FMT;
    DWORD chunk_size;
    WORD  wFormatTag;
    WORD  nChannels;
    DWORD nSamplesPerSec;
    DWORD nAvgBytesPerSec;
    WORD  nBlockAlign;
    WORD  wBitsPerSample;
};

struct WAVE_FACT
{
    DWORD FACT;
    DWORD chunk_size;
};

struct WAVE_DATA
{
    DWORD DATA;
    DWORD chunk_size;
};

// +--------------------------------------------------------------------+

#endif  // Wave_h

