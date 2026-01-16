/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo

*/

#pragma once

#include <cstdint>

#include <Text.h>

#include "Types.h"
#include "GameWinDX9.h"
#include "Bitmap.h"
#include "KeyMap.h"

// +--------------------------------------------------------------------+

class Campaign;
class MenuScreen;
class CmpnScreen;
class PlanScreen;
class LoadScreen;
class GameScreen;
class Ship;
class Sim;
class FadeView;
class CameraDirector;
class MultiController;
class MouseController;
class MusicDirector;
class DataLoader;
class Font;
class TrackIR;
class Mission;
class Universe;

class NetServer;
class NetLobby;

// +--------------------------------------------------------------------+

class Starshatter : public GameWinDX9
{
public:
    Starshatter();
    virtual ~Starshatter();

    virtual bool      Init(HINSTANCE hi, HINSTANCE hpi, LPSTR cmdline, int nCmdShow);
    virtual bool      InitGame();
    virtual bool      ChangeVideo();
    virtual void      GameState();
    virtual void      Exit();

    enum LOBBY  {     NET_LOBBY_CLIENT,
        NET_LOBBY_SERVER
    };

    void      SetGameMode(int mode) override;
    void      RequestChangeVideo();
    void      LoadVideoConfig(const char* filename);
    void      SaveVideoConfig(const char* filename);
    void      SetupSplash();
    void      SetupMenuScreen();
    void      SetupCmpnScreen();
    void      SetupPlanScreen();
    void      SetupLoadScreen();
    void      SetupGameScreen();
    void      OpenTacticalReference();
    void      CreateWorld();
    int       KeyDown(int action) const;

    void      PlayerCam(int mode);
    void      ViewSelection();

    void      MapKeys();
    static  void      MapKeys(KeyMap* mapping, int nkeys);
    static  void      MapKey(int act, int key, int alt=0);

    void      SetTestMode(int t)   { test_mode = t;        }

    static Starshatter*  GetInstance()     { return instance;      }

    // graphic options:
    int               LensFlare()          { return lens_flare;    }
    int               Corona()             { return corona;        }
    int               Nebula()             { return nebula;        }
    int               Dust()               { return dust;          }

    KeyMap&           GetKeyMap()          { return keycfg;        }

    int               GetLoadProgress()    { return load_progress; }
    const char*       GetLoadActivity()    { return load_activity; }

    int               GetChatMode() const  { return chat_mode;        }
    void              SetChatMode(int c);
    const char*       GetChatText() const  { return chat_text.data(); }

    void              StopNetGame();

    int               GetLobbyMode();
    void              SetLobbyMode(int mode = NET_LOBBY_CLIENT);
    void              StartLobby();
    void              StopLobby();

    void              ExecCutscene(const char* msn_file, const char* path);
    void              BeginCutscene();
    void              EndCutscene();
    bool              InCutscene()         const { return cutscene > 0; }
    Mission*          GetCutsceneMission() const;
    const char*       GetSubtitles()       const;
    void              EndMission();

    void              StartOrResumeGame();

    static bool       UseFileSystem();

protected:
    virtual void      DoMenuScreenFrame();
    virtual void      DoCmpnScreenFrame();
    virtual void      DoPlanScreenFrame();
    virtual void      DoLoadScreenFrame();
    virtual void      DoGameScreenFrame();
    virtual void      DoMouseFrame();

    virtual void      DoChatMode();
    virtual void      DoGameKeys();

    virtual void      UpdateWorld();
    virtual void      InstantiateMission();
    virtual bool      ResizeVideo();
    virtual void      InitMouse();

    Universe*               world;

    Window*                 gamewin;
    MenuScreen*             menuscreen;
    LoadScreen*             loadscreen;
    PlanScreen*             planscreen;
    GameScreen*             gamescreen;
    CmpnScreen*             cmpnscreen;

    FadeView*               splash;
    int                     splash_index;
    Bitmap                  splash_image;
    MultiController*        input;
    MouseController*        mouse_input;
    TrackIR*                head_tracker;
    DataLoader*             loader;

    Ship*                   player_ship;
    CameraDirector*         cam_dir;
    MusicDirector*          music_dir;

    Font*                   HUDfont;
    Font*                   GUIfont;
    Font*                   GUI_small_font;
    Font*                   terminal;
    Font*                   verdana;
    Font*                   title_font;
    Font*                   limerick18;
    Font*                   limerick12;
    Font*                   ocrb;

    std::uint32_t           time_mark;
    std::uint32_t           minutes;

    double                  field_of_view;
    double                  orig_fov;

    static int              keymap[256];
    static int              keyalt[256];
    KeyMap                  keycfg;

    bool                    tactical;
    bool                    spinning;
    int                     mouse_x;
    int                     mouse_y;
    int                     mouse_dx;
    int                     mouse_dy;

    int                     test_mode;
    int                     req_change_video;
    int                     video_changed;

    int                     lens_flare;
    int                     corona;
    int                     nebula;
    int                     dust;

    double                  exit_time;

    int                     load_step;
    int                     load_progress;
    Text                    load_activity;
    int                     catalog_index;

    int                     cutscene;
    int                     lobby_mode;
    NetLobby*               net_lobby;
    int                     chat_mode;
    Text                    chat_text;

private:
    static Starshatter* instance;
};
