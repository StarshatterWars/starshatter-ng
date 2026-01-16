/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2022, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Base class (interface) for command pattern.  A
    command encapsulates a single editing operation
    that may be performed on a document.  Each command
    may be done or undone multiple times.  The specialized
    implementations for each type of operation are re-
    sponsible for providing a means to return the model
    to the prior state.
*/

#include "StdAfx.h"
#include "Command.h"

// +--------------------------------------------------------------------+

void         Print(const char* msg, ...);

// +--------------------------------------------------------------------+

Command::Command(const char* n, MagicDoc* d)
   : name(n), document(d)
{
}

Command::~Command()
{
   document = 0;
}

// +--------------------------------------------------------------------+

void
Command::Do()
{
   Print("WARNING: Command::Do() called for '%s'\n", name.data());
}

void
Command::Undo()
{
   Print("WARNING: Command::Undo() called for '%s'\n", name.data());
}
