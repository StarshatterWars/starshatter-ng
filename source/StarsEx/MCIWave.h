/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    MCI Wave Output stuff
*/

#ifndef MCI_WAVE_H
#define MCI_WAVE_H

// +--------------------------------------------------------------------+

int  load_wave_file(const char* fname, LPWAVEHDR hdr, LPWAVEFORMATEX format);
void delete_wave_file(LPWAVEHDR hdr, LPWAVEFORMATEX format);

// +--------------------------------------------------------------------+

#endif
