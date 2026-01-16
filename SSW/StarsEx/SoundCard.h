/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Abstract Audio Output class (hides details of DirectSound)
*/

#ifndef SoundCard_h
#define SoundCard_h

#include <mutex>

#include "Types.h"
#include "List.h"

// +--------------------------------------------------------------------+

class  Sound;
class  Camera;
struct Vec3;

// +--------------------------------------------------------------------+

class SoundCard
{
public:
    static const char* TYPENAME() { return "SoundCard"; }

    SoundCard();
    virtual ~SoundCard();

    enum SoundStatus { SC_UNINITIALIZED,
        SC_OK,
        SC_ERROR,
        SC_BAD_PARAM };
    SoundStatus    Status() const { return status; }

    // Format of the sound card's primary buffer:
    virtual bool   GetFormat(LPWAVEFORMATEX format)                { return false; }
    virtual bool   SetFormat(LPWAVEFORMATEX format)                { return false; }
    virtual bool   SetFormat(int bits, int channels, int hertz)    { return false; }
    virtual bool   Pause()                                         { return false; }
    virtual bool   Resume()                                        { return false; }
    virtual bool   StopSoundEffects()                              { return false; }

    // Get a blank, writable sound buffer:
    virtual Sound* CreateSound(DWORD flags, LPWAVEFORMATEX format) { return 0;     }

    // Create a sound resource:
    virtual Sound* CreateSound(DWORD flags, LPWAVEFORMATEX format,
    DWORD len, LPBYTE data)             { return 0;     }

    // once per frame:
    virtual void   Update();

    virtual void   SetListener(const Camera& cam, const Vec3& vel) { }
    virtual DWORD  UpdateThread();
    virtual void   AddSound(Sound* s);

protected:

    bool           shutdown;
    HANDLE         hthread;
    SoundStatus    status;
    List<Sound>    sounds;
    std::mutex     sync;
};

#endif  // SoundCard_h

