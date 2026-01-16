/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Audio Configuration class
*/

#ifndef AudioConfig_h
#define AudioConfig_h

#include "Types.h"

// +--------------------------------------------------------------------+

class AudioConfig
{
public:
    AudioConfig();
    ~AudioConfig();

    static void          Initialize();
    static void          Close();
    static AudioConfig*  GetInstance();

    void     Load();
    void     Save();

    static int  MenuMusic();
    static int  GameMusic();
    static int  EfxVolume();
    static int  GuiVolume();
    static int  WrnVolume();
    static int  VoxVolume();
    static int  Silence();
    static void SetTraining(bool t);

    int      GetMasterVolume() const { return master_volume; }
    int      GetMusicVolume() const { return music_volume; }
    int      GetWorldVolume() const { return world_volume; }
    int      GetMenuMusic() const { return menu_music; }
    int      GetGameMusic() const { return game_music; }
    int      GetEfxVolume() const { return efx_volume; }
    int      GetGuiVolume() const { return gui_volume; }
    int      GetWrnVolume() const { return wrn_volume; }
    int      GetVoxVolume() const { return vox_volume; }

    void     SetMasterVolume(int v);
    void     SetMusicVolume(int v);
    void     SetWorldVolume(int v);
    void     SetMenuMusic(int v);
    void     SetGameMusic(int v);
    void     SetEfxVolume(int v);
    void     SetGuiVolume(int v);
    void     SetWrnVolume(int v);
    void     SetVoxVolume(int v);

protected:
    int      master_volume;
    int      music_volume;
    int      world_volume;

    int      menu_music;
    int      game_music;

    int      efx_volume;
    int      gui_volume;
    int      wrn_volume;
    int      vox_volume;

    bool     training;
};

#endif  // AudioConfig_h

