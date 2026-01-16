/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#pragma once

#include <string>


namespace starshatter
{


template <typename Input, typename Output> int convert(const std::string& input, const std::string& output);


}  // namespace starshatter


#include "convert.inl.h"
