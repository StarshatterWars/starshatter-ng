/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "Clock.h"

#include <chrono>

static constexpr double NO_COMPRESSION {1.0};
static constexpr double STARTING_DELTA {1.0 / 60.0};

Clock* Clock::instance = nullptr;


Clock*
Clock::GetInstance()
{
    return instance;
}


bool
Clock::Init()
{
    if (instance == nullptr) {
        instance = new Clock();
        return instance != nullptr;
    }
    return false;
}


void
Clock::Close()
{
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}


Clock::Clock() :
    m_point {inner_clock::now()},
    m_game_elapsed {inner_clock::duration::zero()},
    m_real_elapsed {inner_clock::duration::zero()},
    m_compression {NO_COMPRESSION},
    m_delta {STARTING_DELTA},
    m_gui_delta {STARTING_DELTA},
    m_rate {0.0}
{
}


void
Clock::Set()
{
    m_point = inner_clock::now();
    m_delta = STARTING_DELTA;
    m_gui_delta = STARTING_DELTA;
}


double
Clock::Step()
{
    const auto next = inner_clock::now();
    const auto delta = next - m_point;
    m_game_elapsed += std::chrono::duration_cast<inner_clock::duration>(delta * m_compression);
    m_real_elapsed += delta;
    const std::chrono::duration<double> seconds = delta;
    m_delta = seconds.count() * m_compression;
    m_gui_delta = seconds.count();
    m_point = next;
    m_rate = 1 / m_gui_delta;
    return m_delta;
}


void
Clock::ResetGameTime(double seconds)
{
    const std::chrono::duration<double> target {seconds};
    m_game_elapsed = std::chrono::duration_cast<inner_clock::duration>(target);
}


void
Clock::SkipGameTime(double seconds)
{
    const std::chrono::duration<double> skip {seconds};
    m_game_elapsed += std::chrono::duration_cast<inner_clock::duration>(skip);
}


double
Clock::Delta() const
{
    return m_delta;
}


double
Clock::GuiDelta() const
{
    return m_gui_delta;
}


double
Clock::Rate() const
{
    return m_rate;
}


double
Clock::TimeCompression() const
{
    return m_compression;
}


void
Clock::SetTimeCompression(double compression)
{
    m_compression = compression;
}
