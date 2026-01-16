/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#ifndef HttpParam_h
#define HttpParam_h

#include "Text.h"


class HttpParam
{
public:
    static const char* TYPENAME() { return "HttpParam"; }

    HttpParam(const char* n, const char* v);

    int operator == (const HttpParam& p) const;

    Text  name;
    Text  value;
};


#endif  // HttpParam_h
