/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Manages local weather conditions according to the system stardate
*/

#include "Weather.h"

#include "ContentBundle.h"
#include "Stardate.h"

// +--------------------------------------------------------------------+

Weather::Weather()
{
    state       = CLEAR;
    period      = 7 * 20 * 3600;
    ceiling     = 0;
    visibility  = 1;

    for (int i = 0; i < NUM_STATES; i++)
    chances[i] = 0;

    chances[0]  = 1;
}

// +--------------------------------------------------------------------+

Weather::~Weather()
{ }

// +--------------------------------------------------------------------+

void
Weather::SetChance(int n, double c)
{
    if (n >= 0 && n < NUM_STATES) {
        if (c > 1 && c <= 100)
        chances[n] = c / 100;

        else if (c < 1)
        chances[n] = c;
    }
}

void
Weather::NormalizeChances()
{
    double total = 0;

    for (int i = 1; i < NUM_STATES; i++)
    total += chances[i];

    if (total <= 1) {
        chances[0] = 1 - total;
    }

    else {
        chances[0] = 0;

        for (int i = 1; i < NUM_STATES; i++)
        chances[i] /= total;
    }

    int    index = 0;
    double level = 0;

    for (int i = 0; i < NUM_STATES; i++) {
        if (chances[i] > 0) {
            level += chances[i];

            active_states[index] = (STATE) i;
            thresholds[index]    = level;

            index++;
        }
    }

    while (index < NUM_STATES)
    thresholds[index++] = 10;
}

// +--------------------------------------------------------------------+

void
Weather::Update()
{
    NormalizeChances();

    const auto weather = (sin(starshatter::engine::CurrentTime() * 2.0 * PI / period) + 1.0) / 2.0;

    state = active_states[0];

    for (int i = 1; i < NUM_STATES; i++) {
        if (weather > thresholds[i-1] && weather <= thresholds[i]) {
            state = active_states[i];
            break;
        }
    }

    switch (state) {
    default:
    case CLEAR:
        ceiling    = 0;
        visibility = 1.0;
        break;

    case HIGH_CLOUDS:
        ceiling    = 0;
        visibility = 0.9;
        break;

    case MODERATE_CLOUDS:
        ceiling    = 0;
        visibility = 0.8;
        break;

    case OVERCAST:
        ceiling    = 6000;
        visibility = 0.7;
        break;

    case FOG:
        ceiling    = 3500;
        visibility = 0.6;
        break;

    case STORM:
        ceiling    = 7500;
        visibility = 0.5;
        break;
    }
}

// +--------------------------------------------------------------------+

Text
Weather::Description() const
{
    Text description;

    switch (state) {
    default:
    case CLEAR:
        description = ContentBundle::GetInstance()->GetText("weather.clear");
        break;

    case HIGH_CLOUDS:
        description = ContentBundle::GetInstance()->GetText("weather.high-clouds");
        break;

    case MODERATE_CLOUDS:
        description = ContentBundle::GetInstance()->GetText("weather.partly-cloudy");
        break;

    case OVERCAST:
        description = ContentBundle::GetInstance()->GetText("weather.overcast");
        break;

    case FOG:
        description = ContentBundle::GetInstance()->GetText("weather.fog");
        break;

    case STORM:
        description = ContentBundle::GetInstance()->GetText("weather.storm");
        break;
    }

    return description;
}