/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Asteroid Sprite animation class
*/

#include "Asteroid.h"
#include "Shot.h"
#include "Explosion.h"
#include "Sim.h"

#include "Solid.h"
#include "Bitmap.h"
#include "DataLoader.h"
#include "Game.h"

// +--------------------------------------------------------------------+

static Point   asteroid_velocity = Point(0,0,0);
static Model*  asteroid_model[32];

// +--------------------------------------------------------------------+

Asteroid::Asteroid(int t, const Vec3& pos, double m)
    : Debris(asteroid_model[t%6], pos, asteroid_velocity, m)
{
    life = -1;
}

// +--------------------------------------------------------------------+

void
Asteroid::Initialize()
{
    ZeroMemory(asteroid_model, sizeof(asteroid_model));

    DataLoader* loader = DataLoader::GetLoader();
    Text old_path = loader->GetDataPath();
    loader->SetDataPath("Galaxy/Asteroids/");

    int n = 0;

    Model* a = new Model;
    if (a) {
        a->Load("a1.mag", 100);
        asteroid_model[n++] = a;
    }

    a = new Model;
    if (a) {
        a->Load("a2.mag", 50);
        asteroid_model[n++] = a;
    }

    a = new Model;
    if (a) {
        a->Load("a1.mag",  8);
        asteroid_model[n++] = a;
    }

    a = new Model;
    if (a) {
        a->Load("a2.mag", 10);
        asteroid_model[n++] = a;
    }

    a = new Model;
    if (a) {
        a->Load("a3.mag", 30);
        asteroid_model[n++] = a;
    }

    a = new Model;
    if (a) {
        a->Load("a4.mag", 20);
        asteroid_model[n++] = a;
    }

    List<Text> mod_asteroids;
    loader->SetDataPath("Mods/Galaxy/Asteroids/");
    loader->ListFiles("*.mag", mod_asteroids);

    ListIter<Text> iter = mod_asteroids;
    while (++iter && n < 32) {
        a = new Model;
        if (a) {
            a->Load(*iter.value(), 50);
            asteroid_model[n++] = a;
        }
    }

    loader->SetDataPath(old_path);
}

void
Asteroid::Close()
{
    for (int i = 0; i < 32; i++)
    delete asteroid_model[i];

    ZeroMemory(asteroid_model, sizeof(asteroid_model));
}

// +--------------------------------------------------------------------+



