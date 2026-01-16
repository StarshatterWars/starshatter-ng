/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#pragma once

#include <Text.h>


namespace starshatter
{
namespace engine
{


struct Stardate
{
	long double value = {};

	Stardate();
	Stardate(long double value_);

	operator long double() const;

	Text Format(bool short_format=false) const;
};


Stardate CurrentTime();
Stardate OperationTime();
Stardate MissionTime();

long double Epoch();
long double OperationStart();
long double MissionStart();
void SetOperationStart(long double value={}, bool relative=false);
void SetMissionStart(long double value={}, bool relative=false);


}  // namespace engine
}  // namespace starshatter
