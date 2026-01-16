/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "Stardate.h"

#include <cstdint>

#include <Text.h>

#include "Clock.h"


namespace starshatter
{
namespace engine
{


static constexpr long double EPOCH {0.5e9l};
static constexpr long double MINUTE {60};
static constexpr long double HOUR {60 * MINUTE};
static constexpr long double DAY {24 * HOUR};
static long double operation_start {};
static long double mission_start {};


Stardate::Stardate() :
	value {0}
{
}


Stardate::Stardate(long double value_) :
	value {value_}
{
}


Stardate::operator long double() const
{
	return value;
}


Text
Stardate::Format(bool short_format) const
{
	auto time = value;
	std::int_fast32_t day = 1, hours = 0, minutes = 0, seconds = 0;
	while (time >= DAY) {
		time -= DAY;
		++day;
	}
	while (time >= HOUR) {
		time -= HOUR;
		++hours;
	}
	while (time >= MINUTE) {
		time -= MINUTE;
		++minutes;
	}
	seconds = time;
	return Text::format(
		short_format ? "%02d/%02d:%02d:%02d" : "Day %d, %02d:%02d:%02d",
		day, hours, minutes, seconds);
}


Stardate
CurrentTime()
{
	return Epoch() + OperationStart() + OperationTime();
}


Stardate
OperationTime()
{
	return MissionStart() + MissionTime();
}


Stardate
MissionTime()
{
	if (const auto* clock = Clock::GetInstance())
		return clock->GameTime<long double>() / 1000.0l;
	throw "game clock not initialized";
}


long double
Epoch()
{
	return EPOCH;
}


long double
OperationStart()
{
	return operation_start;
}


long double
MissionStart()
{
	return mission_start;
}


void
SetOperationStart(long double value, bool relative)
{
	if (relative)
		operation_start += value;
	else
		operation_start = value;
}


void
SetMissionStart(long double value, bool relative)
{
	if (relative)
		mission_start += value;
	else
		mission_start = value;
}


}  // namespace engine
}  // namespace starshatter
