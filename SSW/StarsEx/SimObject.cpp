/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Simulation Object and Observer classes
*/

#include "SimObject.h"

#include <List.h>

#include "Scene.h"

// +--------------------------------------------------------------------+

SimObserver::~SimObserver()
{
    ListIter<SimObject> observed = observe_list;
    while (++observed)
    observed->Unregister(this);
}

void
SimObserver::Observe(SimObject* obj)
{
    if (obj) {
        obj->Register(this);

        if (!observe_list.contains(obj))
        observe_list.append(obj);
    }
}

void
SimObserver::Ignore(SimObject* obj)
{
    if (obj) {
        obj->Unregister(this);
        observe_list.remove(obj);
    }
}

bool
SimObserver::Update(SimObject* obj)
{
    if (obj)
    observe_list.remove(obj);

    return true;
}

const char*
SimObserver::GetObserverName() const
{
    static char name[32];
    sprintf_s(name, "SimObserver 0x%08x", (DWORD) this);
    return name;
}

// +--------------------------------------------------------------------+

SimObject::~SimObject()
{
    Notify();
}

// +--------------------------------------------------------------------+

void
SimObject::Notify()
{
    if (!notifying) {
        notifying = true;

        int nobservers = observers.size();
        int nupdate    = 0;

        if (nobservers > 0) {
            ListIter<SimObserver> iter = observers;
            while (++iter) {
                SimObserver* observer = iter.value();
                observer->Update(this);
                nupdate++;
            }

            observers.clear();
        }

        if (nobservers != nupdate) {
            ::Print("WARNING: incomplete notify sim object '%s' - %d of %d notified\n",
            Name(), nupdate, nobservers);
        }

        notifying = false;
    }
    else {
        ::Print("WARNING: double notify on sim object '%s'\n", Name());
    }
}

// +--------------------------------------------------------------------+

void
SimObject::Register(SimObserver* observer)
{
    if (!notifying && !observers.contains(observer))
    observers.append(observer);
}

// +--------------------------------------------------------------------+

void
SimObject::Unregister(SimObserver* observer)
{
    if (!notifying)
    observers.remove(observer);
}

// +--------------------------------------------------------------------+

void
SimObject::Activate(Scene& scene)
{
    if (rep)
    scene.AddGraphic(rep);
    if (light)
    scene.AddLight(light);

    active = true;
}

void
SimObject::Deactivate(Scene& scene)
{
    if (rep)
    scene.DelGraphic(rep);
    if (light)
    scene.DelLight(light);

    active = false;
}

