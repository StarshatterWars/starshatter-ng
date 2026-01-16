/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo

*/

#include "Game.h"

#include <cstdint>

#include "Clock.h"
#include "ContentBundle.h"
#include "Mouse.h"
#include "Panic.h"
#include "UIEventDispatch.h"


Game* Game::instance {nullptr};


Game::Game() :
    status {OK},
    exit_code {0},
    game_mode {MENU_MODE},
    active {false},
    paused {false},
    frame_number {0}
{
    if (instance != nullptr)
        throw "Game may have only one instance";
    instance = this;
    Clock::Init();
}


Game::~Game()
{
    Clock::Close();
    if (instance == this)
        instance = nullptr;
}

// +--------------------------------------------------------------------+

bool
Game::Init()
{
    status = OK;

    if (status == OK) {
        Print("  Initializing content...\n");
        ContentBundle::GetInstance()->Init();

        Print("  Initializing game...\n");
        if (!InitGame()) {
            if (Panic::Panicked())
            Panic::Panic("Could not initialize the game.");
            status = INIT_FAILED;
        }
    }

    return status == OK;
}

// +--------------------------------------------------------------------+

bool
Game::InitGame()
{
    return true;
}

// +--------------------------------------------------------------------+

int
Game::Run()
{
    status = RUN;
    Clock::GetInstance()->Set();
    while (status < EXIT && !Panic::Panicked()) {
        PumpEvents();
        GameLoop();
    }
    return exit_code;
}

// +--------------------------------------------------------------------+

void
Game::Exit()
{
    Print("\n\n*** Game::Exit()\n");
    status = EXIT;
}

// +--------------------------------------------------------------------+

void
Game::Activate(bool f)
{
    active = f;
}

// +--------------------------------------------------------------------+

void
Game::Pause(bool f)
{
    paused = f;
}

// +--------------------------------------------------------------------+

void
Game::PumpEvents()
{
}

// +--------------------------------------------------------------------+

void
Game::GameLoop()
{
    if (active) {
        auto dispatch = UIEventDispatch::GetInstance();
        if (dispatch)
            dispatch->Dispatch();
    }
    UpdateWorld();
    GameState();
    UpdateScreen();
    Clock::GetInstance()->Step();
    frame_number++;
    Mouse::w = 0;
}

// +--------------------------------------------------------------------+

void
Game::UpdateWorld()
{
}

// +--------------------------------------------------------------------+

void
Game::GameState()
{
}

// +--------------------------------------------------------------------+

void
Game::UpdateScreen()
{
}

// +--------------------------------------------------------------------+

Game*
Game::GetInstance()
{
    return instance;
}

// +--------------------------------------------------------------------+

std::uint32_t
Game::Frame()
{
    return frame_number;
}
