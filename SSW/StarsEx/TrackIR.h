/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    TrackIR head tracker interface class
*/

#ifndef TrackIR_h
#define TrackIR_h

#include "Types.h"

// +--------------------------------------------------------------------+

class TrackIR
{
public:
    TrackIR();
    ~TrackIR();

    DWORD    ExecFrame();

    bool     IsRunning()    const { return running; }
    double   GetAzimuth()   const { return az; }
    double   GetElevation() const { return el; }

    double   GetX()         const { return x;  }
    double   GetY()         const { return y;  }
    double   GetZ()         const { return z;  }

protected:

    bool     running;
    DWORD    stale_frames;
    DWORD    frame_signature;

    double   az;
    double   el;

    double   x; // vrt
    double   y; // vup
    double   z; // vpn (i think)
};

#endif  // TrackIR_h

