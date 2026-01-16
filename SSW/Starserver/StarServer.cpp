/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo
*/

#include "StarServer.h"

#include <chrono>
#include <iostream>
#include <thread>

#include <Campaign.h>
#include <Clock.h>
#include <CombatRoster.h>
#include <DataLoader.h>
#include <Drive.h>
#include <Explosion.h>
#include <FlightDeck.h>
#include <Galaxy.h>
#include <Game.h>
#include <ModConfig.h>
#include <NetGame.cpp>
#include <NetLobby.h>
#include <NetLobbyServer.h>
#include <NetServerConfig.h>
#include <RadioTraffic.h>
#include <Random.h>
#include <Ship.h>
#include <Shot.h>
#include <Sim.h>
#include <SystemDesign.h>
#include <Types.h>
#include <Utils.h>
#include <WeaponDesign.h>

#include "NetAdminServer.h"


StarServer* StarServer::instance {nullptr};


StarServer*
StarServer::GetInstance()
{
    return instance;
}


StarServer::StarServer() :
    world {nullptr},
    current_mission {nullptr},
    admin_server {nullptr},
    lobby_server {nullptr}
{
    if (instance != nullptr)
        throw "StarServer may have only one instance";
    instance = this;

    DataLoader::Initialize();
    auto loader = DataLoader::GetLoader();
    if (!loader) throw "Could not get DataLoader instance";

    int loadstat = loader->MountDatafile("shatter.dat");
    if (loadstat == DataLoader::FAILED) {
        throw loader->LastError();
    }

    if (loader->FindFile("start.dat"))
        loader->MountDatafile("start.dat");

    loader->EnableMedia(false);
}


StarServer::~StarServer()
{
    delete admin_server;
    delete lobby_server;

    admin_server = nullptr;
    lobby_server = nullptr;

    // delete all the ships and stuff
    // BEFORE getting rid of the system
    // and weapons catalogs!
    if (world) delete world;
    world = nullptr;

    Drive::Close();
    Explosion::Close();
    FlightDeck::Close();
    Campaign::Close();
    CombatRoster::Close();
    Galaxy::Close();
    RadioTraffic::Close();
    Ship::Close();
    WeaponDesign::Close();
    SystemDesign::Close();
    DataLoader::Close();
    NetServerConfig::Close();
    ModConfig::Close();

    if (instance == this)
        instance = nullptr;
}


bool
StarServer::Init()
{
    if (auto loader = DataLoader::GetLoader())
        loader->UseFileSystem(false);
    return Game::Init();
}


bool
StarServer::InitGame()
{
    if (!Game::InitGame())
    return false;

    RandomInit();
    ModConfig::Initialize();
    NetServerConfig::Initialize();
    SystemDesign::Initialize("sys.def");
    WeaponDesign::Initialize("wep.def");
    Ship::Initialize();
    Galaxy::Initialize();
    CombatRoster::Initialize();
    Campaign::Initialize();

    Drive::Initialize();
    Explosion::Initialize();
    FlightDeck::Initialize();
    Ship::Initialize();
    Shot::Initialize();
    RadioTraffic::Initialize();

    NetServerConfig* server_config = NetServerConfig::GetInstance();
    if (!server_config)
    return false;

    ::Print("\n\n\nStarshatter Server Init\n");
    ::Print("-----------------------\n");
    ::Print("Server Name:       %s\n", (const char*) server_config->Name());
    ::Print("Server Type:       %d\n", server_config->GetGameType());

    if (server_config->GetMission().length() > 0)
    ::Print("Server Mission:    %s\n", (const char*) server_config->GetMission());

    ::Print("Lobby Server Port: %d\n", server_config->GetLobbyPort());
    ::Print("Admin Server Port: %d\n", server_config->GetAdminPort());
    ::Print("-----------------------\n");

    NetLobbyServer* nls = new NetLobbyServer;
    NetAdminServer* nas = NetAdminServer::GetInstance(server_config->GetAdminPort());
    nas->SetServerName(server_config->Name());

    lobby_server = nls;
    admin_server = nas;

    std::cout << "Started server listening on port " << server_config->GetLobbyPort() << std::endl;

    return true;
}


void
StarServer::SetGameMode(int m)
{
    if (game_mode == m)
    return;

    if (m == LOAD_MODE) {
        Print("  game_mode = LOAD_MODE\n");
        paused = true;
    }

    else if (m == PLAY_MODE) {
        Print("  game_mode = PLAY_MODE\n");

        if (!world) {
            CreateWorld();
            InstantiateMission();
        }

        // stand alone server should wait for players to connect
        // before unpausing the simulation...
        Clock::GetInstance()->SetTimeCompression(1.0);
        Pause(true);
    }

    else if (m == MENU_MODE) {
        Print("  game_mode = MENU_MODE\n");
        paused = true;

        if (auto sim = dynamic_cast<Sim*>(world))
            sim->UnloadMission();
    }

    game_mode = m;
}


void
StarServer::CreateWorld()
{
    RadioTraffic::Initialize();

    // create world
    if (!world) {
        world = new Sim(nullptr);
        Print("  World Created.\n");
    }
}


void
StarServer::InstantiateMission()
{
    current_mission = nullptr;

    if (Campaign::GetCampaign()) {
        current_mission = Campaign::GetCampaign()->GetMission();
    }

    if (auto sim = dynamic_cast<Sim*>(world)) {
        sim->UnloadMission();

        if (current_mission) {
            sim->LoadMission(current_mission);
            sim->ExecMission();
            sim->SetTestMode(false);

            Print("  Mission Instantiated.\n");
            std::cout << "Loaded mission: " << current_mission->Name() << std::endl;
        }

        else {
            Print("  *** WARNING: StarServer::InstantiateMission() - no mission selected ***\n");
        }
    }
}


void
StarServer::UpdateWorld()
{
    Galaxy* galaxy = Galaxy::GetInstance();
    if (galaxy) galaxy->ExecFrame();

    Campaign* campaign = Campaign::GetCampaign();
    if (campaign) campaign->ExecFrame();

    if (paused) {
        if (world)
        world->ExecFrame(0);
    }

    else {
        Drive::StartFrame();

        if (world)
        world->ExecFrame(Clock::GetInstance()->Delta());
    }
}


void
StarServer::GameState()
{
    if (lobby_server) {
        lobby_server->ExecFrame();

        if (lobby_server->GetStatus() == NetServerInfo::PERSISTENT)
        paused = NetGame::NumPlayers() < 1;
    }

    if (game_mode == MENU_MODE) {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    else if (game_mode == LOAD_MODE) {
        CreateWorld();
        InstantiateMission();

        SetGameMode(PLAY_MODE);
    }
    else if (game_mode == PLAY_MODE) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


bool
StarServer::Server() const
{
    return true;
}
