/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "Clock.h"

#include <chrono>
#include <ratio>


template <typename R>
R
Clock::GameTime() const
{
    using target_duration = std::chrono::duration<R, std::milli>;
    return std::chrono::duration_cast<target_duration>(m_game_elapsed).count();
}


template <typename R>
R
Clock::RealTime() const
{
    using target_duration = std::chrono::duration<R, std::milli>;
    return std::chrono::duration_cast<target_duration>(m_real_elapsed).count();
}
