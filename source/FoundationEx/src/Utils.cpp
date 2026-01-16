/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include <Utils.h>

#include <stdarg.h>
#include <stdio.h>


FILE* ErrLog = nullptr;


void AssignErrLog(FILE * out)
{
    ErrLog = out;
}


int CloseErrLog()
{
    if (ErrLog)
        return fclose(ErrLog);
    return 0;
}


void Print(const char* fmt, ...)
{
    if (!ErrLog) return;
    va_list args;
    va_start(args, fmt);
    vfprintf(ErrLog, fmt, args);
    fflush(ErrLog);
    va_end(args);
}
