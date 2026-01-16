/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#ifndef Clock_h
#define Clock_h

#include <chrono>
#include <cstdint>


class Clock
{
public:
    using inner_clock = std::chrono::high_resolution_clock;

    static Clock* GetInstance();
    static bool Init();
    static void Close();

    void Set();
    double Step();
    void ResetGameTime(double seconds={});
    void SkipGameTime(double seconds);

    double Delta() const;
    double GuiDelta() const;
    double Rate() const;
    double TimeCompression() const;
    template <typename R = std::uint32_t> R GameTime() const;
    template <typename R = std::uint32_t> R RealTime() const;

    void SetTimeCompression(double compression);

protected:
    Clock();

    inner_clock::time_point m_point;
    inner_clock::duration m_game_elapsed;
    inner_clock::duration m_real_elapsed;
    double m_compression;
    double m_delta;
    double m_gui_delta;
    double m_rate;

private:
    static Clock* instance;
};


#include "Clock.inl.h"

#endif  // Clock_h
