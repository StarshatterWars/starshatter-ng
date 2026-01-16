/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Parser utility functions
*/

#pragma once

#include <cstdint>
#include <vector>

#include <starshatter/definition.h>
#include <starshatter/foundation/reader.h>

#include "Types.h"
#include "Geometry.h"
#include "Color.h"


bool GetDefBool(bool& dst, TermDef* def, const char* file);
bool GetDefText(Text& dst, TermDef* def, const char* file);
bool GetDefText(char* dst, TermDef* def, const char* file);
template <typename Number> bool GetDefNumber(Number& dst, TermDef* def, const char* file);
bool GetDefVec(Vec3& dst, TermDef* def, const char* file);
bool GetDefColor(Color& dst, TermDef* def, const char* file);
bool GetDefColor(ColorValue& dst, TermDef* def, const char* file);
bool GetDefRect(Rect& dst, TermDef* def, const char* file);
bool GetDefInsets(Insets& dst, TermDef* def, const char* file);
bool GetDefTime(int& dst, TermDef* def, const char* file);
bool GetDefArray(int*    dst, int size, TermDef* def, const char* file);
bool GetDefArray(float*  dst, int size, TermDef* def, const char* file);
bool GetDefArray(double* dst, int size, TermDef* def, const char* file);
bool GetDefArray(std::vector<DWORD>& array,      TermDef* def, const char* file);
bool GetDefArray(std::vector<float>& array,      TermDef* def, const char* file);

#include "ParseUtil.inl.h"
