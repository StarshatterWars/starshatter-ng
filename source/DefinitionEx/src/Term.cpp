/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Implementation of the Term class
*/

#include <starshatter/definition/Term.h>

#include <Utils.h>

// +-------------------------------------------------------------------+

Term*
error(const char* s1, const char* s2)
{
    Print("ERROR: ");
    if (s1) Print(s1);
    if (s2) Print(s2);
    Print("\n\n");
    return 0;
}

// +-------------------------------------------------------------------+

void TermBool::print(int level)  { if (level > 0) Print(val? "true" : "false"); else Print("..."); }
void TermNumber::print(int level){ if (level > 0) Print("%g", val);     else Print("..."); }
void TermText::print(int level)  { if (level > 0) Print("\"%s\"", val.data()); else Print("..."); }

// +-------------------------------------------------------------------+

TermArray::TermArray(TermList* elist)
{
    elems = elist;
}

TermArray::~TermArray()
{
    if (elems) elems->destroy();
    delete elems;
}

void
TermArray::print(int level)
{
    if (level > 1) {
        Print("(");

        if (elems) {
            for (int i = 0; i < elems->size(); i++) {
                elems->at(i)->print(level-1);
                if (i < elems->size() -1)
                    Print(", ");
            }
        }

        Print(") ");
    }
    else Print("(...) ");
}

// +-------------------------------------------------------------------+

TermStruct::TermStruct(TermList* elist)
{
    elems = elist;
}

TermStruct::~TermStruct()
{
    if (elems) elems->destroy();
    delete elems;
}

void
TermStruct::print(int level)
{
    if (level > 1) {
        Print("{");

        if (elems) {
            for (int i = 0; i < elems->size(); i++) {
                elems->at(i)->print(level-1);
                if (i < elems->size() -1)
                    Print(", ");
            }
        }

        Print("} ");
    }
    else Print("{...} ");
}

// +-------------------------------------------------------------------+

TermDef::~TermDef()
{
    delete mname;
    delete mval;
}

void
TermDef::print(int level)
{
    if (level >= 0) {
        mname->print(level);
        Print(": ");
        mval->print(level-1);
    }
    else Print("...");
}

// +-------------------------------------------------------------------+
