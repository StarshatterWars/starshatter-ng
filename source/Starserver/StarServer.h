/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo
*/

#pragma once

#include <Game.h>
#include <Mission.h>
#include <NetLobbyServer.h>
#include <NetServer.h>
#include <Universe.h>


class StarServer : public Game
{
public:
    static StarServer*   GetInstance();

    StarServer();
    virtual ~StarServer();

    virtual bool      Init();
    virtual bool      InitGame();

    virtual void      UpdateWorld();
    virtual void      GameState();

    bool      Server() const override;
    void      SetGameMode(int mode) override;

    void      CreateWorld();

protected:
    virtual void      InstantiateMission();

    Universe*               world;
    Mission*                current_mission;

    NetServer*              admin_server;
    NetLobbyServer*         lobby_server;

private:
    static StarServer* instance;
};
