/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo

*/

#ifndef Game_h
#define Game_h

#include <cstdint>


class Game
{
public:
    static const char* TYPENAME() { return "Game"; }
    enum STATUS { OK, RUN, EXIT, INIT_FAILED, TOO_MANY };
    enum MODE {
        MENU_MODE,
        CLOD_MODE,
        CMPN_MODE,
        PREP_MODE,
        PLAN_MODE,
        LOAD_MODE,
        PLAY_MODE,
        EXIT_MODE,
    };

    Game();
    virtual ~Game();

    virtual bool      Init();
    virtual int       Run();
    virtual void      Exit();

    virtual int GetGameMode() const { return game_mode; }
    virtual void SetGameMode(int mode) { game_mode = mode; }

    virtual void      Activate(bool f);
    virtual void      Pause(bool f);
    int               Status() const { return status; }

    //
    // GENERAL GAME CLASS UTILITY METHODS:
    //

    static Game*       GetInstance();

    std::uint32_t     Frame();

    bool              Active() const { return active;     }
    bool              Paused() const { return paused;     }
    virtual bool      Server() const { return false;      }

    virtual void      PumpEvents();
    virtual void      GameLoop();
    virtual void      UpdateWorld();
    virtual void      GameState();
    virtual void      UpdateScreen();

    virtual bool      InitGame();

protected:
    int               status;
    int               exit_code;
    int               game_mode;

    bool              active;
    bool              paused;
    std::uint32_t     frame_number;

private:
    static Game* instance;
};


#endif  // Game_h
