/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    View class for Radio Communications HUD Overlay
*/

#ifndef RadioVox_h
#define RadioVox_h

#include "Types.h"
#include "List.h"
#include "Text.h"

// +--------------------------------------------------------------------+

class Element;
class Ship;
class RadioMessage;
class Sound;

// +--------------------------------------------------------------------+

class RadioVox
{
    friend class RadioVoxController;

public:
    static const char* TYPENAME() { return "RadioVox"; }

    RadioVox(int channel, const char* path, const char* message=0);
    virtual ~RadioVox();

    // Operations:
    virtual bool      AddPhrase(const char* key);
    virtual bool      Start();

    static void       Initialize();
    static void       Close();

protected:
    virtual bool      Update();

    Text              path;
    Text              message;
    List<Sound>       sounds;
    int               index;
    int               channel;
};

#endif  // RadioVox_h

