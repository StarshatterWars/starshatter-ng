/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Declaration of the generic Parser class
*/

#pragma once

#include <starshatter/foundation/reader.h>
#include <Text.h>

#include "Term.h"

// +-------------------------------------------------------------------+

class Scanner;

// +-------------------------------------------------------------------+

class Parser
{
public:
    Parser();
    Parser(starshatter::foundation::Reader r);
    ~Parser();

    Term*          ParseTerm();
    Term*          ParseTermBase();
    Term*          ParseTermRest(Term* base);
    TermList*      ParseTermList(int for_struct);
    TermArray*     ParseArray();
    TermStruct*    ParseStruct();

private:
    Scanner* lexer;
};
