/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "Panic.h"

#include <sstream>
#include <string>

#include "Utils.h"

namespace Panic
{


static std::string panic_msg;


void Panic(const char* msg)
{
    if (msg)
        Print("*** PANIC: %s\n", msg);
    else
        Print("*** PANIC! ***\n");

    std::ostringstream full_msg;
    if (msg)
        full_msg << msg;
    else
        full_msg << "Unspecified fatal error";
    full_msg << std::endl << "This game will now terminate.";
    panic_msg = full_msg.str();
}


bool Panicked()
{
    return !panic_msg.empty();
}


const char* Message()
{
    return panic_msg.c_str();
}


}  // namespace Panic
