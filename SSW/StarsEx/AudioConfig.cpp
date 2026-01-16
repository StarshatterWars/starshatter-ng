/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Audio Configuration class
*/

#include "AudioConfig.h"

#include <algorithm>
#include <cstddef>

#include "DataLoader.h"
#include "ParseUtil.h"
#include "Button.h"
#include "Game.h"

// +--------------------------------------------------------------------+

static AudioConfig* audio_config = nullptr;

// +--------------------------------------------------------------------+

AudioConfig::AudioConfig() :
    master_volume(100),
    music_volume(100),
    world_volume(100),
    menu_music(80),
    game_music(80),
    efx_volume(80),
    gui_volume(80),
    wrn_volume(80),
    vox_volume(80),
    training(false)
{
    if (!audio_config)
        audio_config = this;
}

AudioConfig::~AudioConfig()
{
    if (audio_config == this)
        audio_config = nullptr;
}

// +--------------------------------------------------------------------+

void
AudioConfig::Initialize()
{
    audio_config = new AudioConfig;
    if (audio_config)
        audio_config->Load();
}

void
AudioConfig::Close()
{
    delete audio_config;
    audio_config = nullptr;
}

AudioConfig*
AudioConfig::GetInstance()
{
    return audio_config;
}

// +--------------------------------------------------------------------+

static constexpr
int
denormalize(const double& v)
{
    return -50 * (100.0 - v);
}

int
AudioConfig::MenuMusic()
{
    if (audio_config)
        return denormalize(
            audio_config->menu_music * (audio_config->music_volume / 100.0) * (audio_config->master_volume / 100.0));
    return 0;
}

int
AudioConfig::GameMusic()
{
    int vol = 0;

    if (audio_config) {
        vol = denormalize(
            audio_config->game_music * (audio_config->music_volume / 100.0) * (audio_config->master_volume / 100.0));

        if (audio_config->training)
        vol -= 2000;
    }

    return vol;
}

int
AudioConfig::EfxVolume()
{
    int vol = 0;

    if (audio_config) {
        vol = denormalize(
            audio_config->efx_volume * (audio_config->world_volume / 100.0) * (audio_config->master_volume / 100.0));

        if (audio_config->training)
        vol -= 2000;
    }

    return vol;
}

int
AudioConfig::GuiVolume()
{
    if (audio_config)
        return denormalize(audio_config->gui_volume * (audio_config->master_volume / 100.0));
    return 0;
}

int
AudioConfig::WrnVolume()
{
    int vol = 0;

    if (audio_config) {
        vol = denormalize(
            audio_config->wrn_volume * (audio_config->world_volume / 100.0) * (audio_config->master_volume / 100.0));

        if (audio_config->training)
        vol -= 2000;
    }

    return vol;
}

int
AudioConfig::VoxVolume()
{
    int vol = 0;

    if (audio_config) {
        vol = denormalize(
            audio_config->vox_volume * (audio_config->world_volume / 100.0) * (audio_config->master_volume / 100.0));

        if (audio_config->training && vol < -750)
        vol = -750;
    }

    return vol;
}

int
AudioConfig::Silence()
{
    return -5000;
}

void
AudioConfig::SetTraining(bool t)
{
    if (audio_config)
    audio_config->training = t;
}

// +--------------------------------------------------------------------+

void
AudioConfig::SetMasterVolume(int v)
{
    master_volume = std::clamp(v, 0, 100);
    Button::SetVolume(GuiVolume());
}

void
AudioConfig::SetMusicVolume(int v)
{
    music_volume = std::clamp(v, 0, 100);
}

void
AudioConfig::SetWorldVolume(int v)
{
    world_volume = std::clamp(v, 0, 100);
}

void
AudioConfig::SetMenuMusic(int v)
{
    menu_music = std::clamp(v, 0, 100);
}

void
AudioConfig::SetGameMusic(int v)
{
    game_music = std::clamp(v, 0, 100);
}

void
AudioConfig::SetEfxVolume(int v)
{
    efx_volume = std::clamp(v, 0, 100);
}

void
AudioConfig::SetGuiVolume(int v)
{
    gui_volume = std::clamp(v, 0, 100);
    Button::SetVolume(GuiVolume());
}

void
AudioConfig::SetWrnVolume(int v)
{
    wrn_volume = std::clamp(v, 0, 100);
}

void
AudioConfig::SetVoxVolume(int v)
{
    vox_volume = std::clamp(v, 0, 100);
}

// +--------------------------------------------------------------------+

void
AudioConfig::Load()
{
    DataLoader* loader = DataLoader::GetLoader();
    Text old_path = loader->GetDataPath();
    loader->SetDataPath(0);

    // read the config file:
    BYTE*       block    = 0;
    std::size_t blocklen = 0;
    const char* filename = "audio.cfg";

    FILE* f;
    ::fopen_s(&f, filename, "rb");

    if (f) {
        ::fseek(f, 0, SEEK_END);
        blocklen = ftell(f);
        ::fseek(f, 0, SEEK_SET);

        block = new BYTE[blocklen+1];
        block[blocklen] = 0;

        ::fread(block, blocklen, 1, f);
        ::fclose(f);
    }

    if (blocklen == 0)
    return;

    Parser parser({reinterpret_cast<const char*>(block), blocklen});
    Term*  term = parser.ParseTerm();

    if (!term) {
        Print("ERROR: could not parse '%s'.\n", filename);
        exit(-3);
    }
    else {
        TermText* file_type = term->isText();
        if (!file_type || file_type->value() != "AUDIO") {
            Print("WARNING: invalid %s file.  Using defaults\n", filename);
            return;
        }
    }

    do {
        delete term;

        term = parser.ParseTerm();

        if (term) {
            int      v   = 0;
            TermDef* def = term->isDef();

            if (def) {
                if (def->name()->value() == "master_volume") {
                    GetDefNumber(v, def, filename);
                    SetMasterVolume(v);
                }

                if (def->name()->value() == "music_volume") {
                    GetDefNumber(v, def, filename);
                    SetMusicVolume(v);
                }

                if (def->name()->value() == "world_volume") {
                    GetDefNumber(v, def, filename);
                    SetWorldVolume(v);
                }

                if (def->name()->value() == "menu_music") {
                    GetDefNumber(v, def, filename);
                    SetMenuMusic(v);
                }

                else if (def->name()->value() == "game_music") {
                    GetDefNumber(v, def, filename);
                    SetGameMusic(v);
                }

                else if (def->name()->value() == "efx_volume") {
                    GetDefNumber(v, def, filename);
                    SetEfxVolume(v);
                }

                else if (def->name()->value() == "gui_volume") {
                    GetDefNumber(v, def, filename);
                    SetGuiVolume(v);
                }

                else if (def->name()->value() == "wrn_volume") {
                    GetDefNumber(v, def, filename);
                    SetWrnVolume(v);
                }

                else if (def->name()->value() == "vox_volume") {
                    GetDefNumber(v, def, filename);
                    SetVoxVolume(v);
                }

                else
                Print("WARNING: unknown label '%s' in '%s'\n",
                def->name()->value().data(), filename);
            }
            else {
                Print("WARNING: term ignored in '%s'\n", filename);
                term->print();
                Print("\n");
            }
        }
    }
    while (term);

    loader->ReleaseBuffer(block);
    loader->SetDataPath(old_path);
}

void
AudioConfig::Save()
{
    FILE* f;
    fopen_s(&f, "audio.cfg", "wb");
    if (f) {
        fprintf(f, "AUDIO\n\n");
        fprintf(f, "master_volume: %3d\n",   master_volume);
        fprintf(f, "music_volume:  %3d\n",   music_volume);
        fprintf(f, "world_volume:  %3d\n\n", world_volume);
        fprintf(f, "menu_music: %3d\n",   menu_music);
        fprintf(f, "game_music: %3d\n\n", game_music);
        fprintf(f, "efx_volume: %3d\n",   efx_volume);
        fprintf(f, "gui_volume: %3d\n",   gui_volume);
        fprintf(f, "wrn_volume: %3d\n",   wrn_volume);
        fprintf(f, "vox_volume: %3d\n",   vox_volume);
        fclose(f);
    }
}

// +--------------------------------------------------------------------+



