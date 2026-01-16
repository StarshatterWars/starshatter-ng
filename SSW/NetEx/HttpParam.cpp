/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "HttpParam.h"


HttpParam::HttpParam(const char* n, const char* v) :
    name(n),
    value(v)
{
}


int
HttpParam::operator == (const HttpParam& p) const
{
    return name == p.name;
}
