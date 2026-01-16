/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#ifndef Panic_h
#define Panic_h


namespace Panic
{
void Panic(const char* msg = nullptr);
bool Panicked();
const char* Message();
}


#endif  // Panic_h
