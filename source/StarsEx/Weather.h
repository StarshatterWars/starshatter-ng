/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Manages local weather conditions according to the system stardate
*/

#ifndef Weather_h
#define Weather_h

#include "Types.h"
#include "Text.h"

// +--------------------------------------------------------------------+

class Weather
{
public:
    Weather();
    virtual ~Weather();

    enum STATE  {  CLEAR,
        HIGH_CLOUDS,
        MODERATE_CLOUDS,
        OVERCAST,
        FOG,
        STORM,

        NUM_STATES
    };

    virtual void      Update();

    // accessors:
    STATE             State()           const { return state;            }
    Text              Description()     const;
    double            Period()          const { return period;           }
    double            Chance(STATE s)   const { return chances[(int)s];  }
    double            Ceiling()         const { return ceiling;          }
    double            Visibility()      const { return visibility;       }

    void              SetPeriod(double p)        { period = p;           }
    void              SetChance(int n, double c);

protected:
    void              NormalizeChances();

    STATE             state;
    double            period;
    double            chances[NUM_STATES];
    double            ceiling;
    double            visibility;

    STATE             active_states[NUM_STATES];
    double            thresholds[NUM_STATES];
};


#endif  // Weather_h

