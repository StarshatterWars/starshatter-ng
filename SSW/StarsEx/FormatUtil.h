/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Text formatting utilities
*/

#ifndef FormatUtil_h
#define FormatUtil_h

#include "Types.h"
#include "Geometry.h"
#include "Text.h"

// +--------------------------------------------------------------------+

void FormatNumber(char* txt, double n);
void FormatNumberExp(char* txt, double n);
void FormatTime(char* txt, double seconds);
void FormatTimeOfDay(char* txt, double seconds);
void FormatDayTime(char* txt, double seconds, bool short_format=false);
void FormatDay(char* txt, double seconds);
void FormatPoint(char* txt, const Point& p);
Text FormatTimeString(int utc=0);

const char* SafeString(const char* s);
const char* SafeQuotes(const char* s);

// scan msg and replace all occurrences of tgt with val
// return new result, leave msg unmodified
Text FormatTextReplace(const char* msg, const char* tgt, const char* val);

// scan msg and replace all C-style \x escape sequences
// with their single-character values, leave orig unmodified
Text FormatTextEscape(const char* msg);

// +--------------------------------------------------------------------+

#endif  // FormatUtil_h

