/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#ifndef WndProc_h
#define WndProc_h

#include "Types.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void FlushKeys();
void BufferKey(int vkey);
int GetKey();
int GetKeyPlus(int& key, int& shift);
void ProcessKeyMessage();


#endif  // WndProc_h
