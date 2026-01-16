/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Generic ship System Design class
*/

#ifndef SystemDesign_h
#define SystemDesign_h

#include "Types.h"
#include "List.h"
#include "Text.h"

// +--------------------------------------------------------------------+

class ComponentDesign;

// +--------------------------------------------------------------------+

class SystemDesign
{
public:
    static const char* TYPENAME() { return "SystemDesign"; }

    SystemDesign();
    ~SystemDesign();
    int operator == (const SystemDesign& rhs) const { return name == rhs.name; }

    static void          Initialize(const char* filename);
    static void          Close();
    static SystemDesign* Find(const char* name);

    // Unique ID:
    Text              name;

    // Sub-components:
    List<ComponentDesign> components;

    static List<SystemDesign>  catalog;
};

// +--------------------------------------------------------------------+


#endif  // SystemDesign_h

