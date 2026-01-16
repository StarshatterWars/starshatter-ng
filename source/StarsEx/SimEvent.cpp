/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Simulation Events for mission summary
*/

#include "SimEvent.h"

#include "Clock.h"
#include "ContentBundle.h"
#include "Ship.h"
#include "Sim.h"
#include <algorithm>


// +====================================================================+

List<ShipStats>   records;

// +====================================================================+

SimEvent::SimEvent(int e, const char* t, const char* i)
: event(e), count(0)
{
    Sim* sim = Sim::GetSim();
    if (sim) {
        time = (int) sim->MissionClock();
    }
    else {
        time = (int) (Clock::GetInstance()->GameTime()/1000);
    }

    SetTarget(t);
    SetInfo(i);
}

SimEvent::~SimEvent()
{
}

// +--------------------------------------------------------------------+

void
SimEvent::SetTime(int t)
{
    time = t;
}

void
SimEvent::SetTarget(const char* t)
{
    if (t && t[0])
    target = t;
}

void
SimEvent::SetInfo(const char* i)
{
    if (i && i[0])
    info = i;
}

void
SimEvent::SetCount(int c)
{
    count = c;
}

Text
SimEvent::GetEventDesc() const
{
    switch (event) {
    case LAUNCH:         return ContentBundle::GetInstance()->GetText("sim.event.Launch");
    case DOCK:           return ContentBundle::GetInstance()->GetText("sim.event.Dock");
    case LAND:           return ContentBundle::GetInstance()->GetText("sim.event.Land");
    case EJECT:          return ContentBundle::GetInstance()->GetText("sim.event.Eject");
    case CRASH:          return ContentBundle::GetInstance()->GetText("sim.event.Crash");
    case COLLIDE:        return ContentBundle::GetInstance()->GetText("sim.event.Collision With");
    case DESTROYED:      return ContentBundle::GetInstance()->GetText("sim.event.Destroyed By");
    case MAKE_ORBIT:     return ContentBundle::GetInstance()->GetText("sim.event.Make Orbit");
    case BREAK_ORBIT:    return ContentBundle::GetInstance()->GetText("sim.event.Break Orbit");
    case QUANTUM_JUMP:   return ContentBundle::GetInstance()->GetText("sim.event.Quantum Jump");
    case LAUNCH_SHIP:    return ContentBundle::GetInstance()->GetText("sim.event.Launch Ship");
    case RECOVER_SHIP:   return ContentBundle::GetInstance()->GetText("sim.event.Recover Ship");
    case FIRE_GUNS:      return ContentBundle::GetInstance()->GetText("sim.event.Fire Guns");
    case FIRE_MISSILE:   return ContentBundle::GetInstance()->GetText("sim.event.Fire Missile");
    case DROP_DECOY:     return ContentBundle::GetInstance()->GetText("sim.event.Drop Decoy");
    case GUNS_KILL:      return ContentBundle::GetInstance()->GetText("sim.event.Guns Kill");
    case MISSILE_KILL:   return ContentBundle::GetInstance()->GetText("sim.event.Missile Kill");
    case LAUNCH_PROBE:   return ContentBundle::GetInstance()->GetText("sim.event.Launch Probe");
    case SCAN_TARGET:    return ContentBundle::GetInstance()->GetText("sim.event.Scan Target");
    default:             return ContentBundle::GetInstance()->GetText("sim.event.no event");
    }
}

// +====================================================================+

ShipStats::ShipStats(const char* n, int i)
: name(n), iff(i), kill1(0), kill2(0), lost(0), coll(0), points(0),
cmd_points(0), gun_shots(0), gun_hits(0), missile_shots(0), missile_hits(0),
combat_group(0), combat_unit(0), player(false), ship_class(0), elem_index(-1)
{
    if (!n || !n[0])
    name = ContentBundle::GetInstance()->GetText("[unknown]");
}

ShipStats::~ShipStats()
{
    events.destroy();
}

// +--------------------------------------------------------------------+

void
ShipStats::SetType(const char* t)
{
    if (t && t[0])
    type = t;
}

void
ShipStats::SetRole(const char* r)
{
    if (r && r[0])
    role = r;
}

void
ShipStats::SetRegion(const char* r)
{
    if (r && r[0])
    region = r;
}

void
ShipStats::SetCombatGroup(CombatGroup* g)
{
    combat_group = g;
}

void
ShipStats::SetCombatUnit(CombatUnit* u)
{
    combat_unit = u;
}

void
ShipStats::SetElementIndex(int n)
{
    elem_index = n;
}

void
ShipStats::SetPlayer(bool p)
{
    player = p;
}

// +--------------------------------------------------------------------+

void
ShipStats::Summarize()
{
    kill1 = 0;
    kill2 = 0;
    lost  = 0;
    coll  = 0;

    ListIter<SimEvent> iter = events;
    while (++iter) {
        SimEvent* event = iter.value();
        int       code  = event->GetEvent();

        if (code == SimEvent::GUNS_KILL)
        kill1++;

        else if (code == SimEvent::MISSILE_KILL)
        kill2++;

        else if (code == SimEvent::DESTROYED)
        lost++;

        else if (code == SimEvent::CRASH)
        coll++;

        else if (code == SimEvent::COLLIDE)
        coll++;
    }
}


long double
ShipStats::FlightTime() const
{
    return flight_time;
}


void
ShipStats::SetFlightTime(long double seconds)
{
    flight_time = seconds;
}


static int
FlightTimeBonus(const ShipStats* stats)
{
    static constexpr long double TEN_MINUTES {600.0l};
    return static_cast<int>(stats->FlightTime() / TEN_MINUTES) * 10;
}


static int
DockBonus(const ShipStats* stats)
{
    return stats->HasEvent(SimEvent::DOCK) ? 100 : 0;
}


static int
LossPenalty(const ShipStats* stats)
{
    return 2.5 * Ship::Value(stats->GetShipClass());
}


int ShipStats::EffectivePoints() const
{
    const int whole = GetPoints() + GetCommandPoints();

    if (GetDeaths() > 0 || GetColls() > 0)
        return (std::max)(0, whole - LossPenalty(this));

    return whole + FlightTimeBonus(this) + DockBonus(this);
}

// +--------------------------------------------------------------------+

SimEvent*
ShipStats::AddEvent(SimEvent* e)
{
    events.append(e);
    return e;
}

SimEvent*
ShipStats::AddEvent(int event, const char* tgt, const char* info)
{
    SimEvent* e = new SimEvent(event, tgt, info);
    events.append(e);
    return e;
}

bool
ShipStats::HasEvent(int event) const
{
    for (int i = 0; i < events.size(); i++)
    if (events[i]->GetEvent() == event)
    return true;

    return false;
}

// +--------------------------------------------------------------------+

void ShipStats::Initialize()  { records.destroy(); }
void ShipStats::Close()       { records.destroy(); }

// +--------------------------------------------------------------------+

int
ShipStats::NumStats()
{
    return records.size();
}

ShipStats*
ShipStats::GetStats(int i)
{
    if (i >= 0 && i < records.size())
    return records.at(i);

    return 0;
}

ShipStats*
ShipStats::Find(const char* name)
{
    if (name && name[0]) {
        ListIter<ShipStats> iter = records;
        while (++iter) {
            ShipStats* stats = iter.value();
            if (!strcmp(stats->GetName(), name))
            return stats;
        }

        ShipStats* stats = new ShipStats(name);
        records.append(stats);
        return stats;
    }

    return 0;
}

