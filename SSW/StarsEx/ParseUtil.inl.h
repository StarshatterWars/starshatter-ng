/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include <type_traits>

#include <starshatter/definition.h>
#include <Utils.h>


template <typename Number>
bool
GetDefNumber(Number& dst, TermDef* def, const char* file)
{
    static_assert(std::is_convertible<long double, Number>::value);
    if (!def || !def->term()) {
        Print("WARNING: missing NUMBER TermDef in '%s'\n", file);
        return false;
    }
    TermNumber* tr = def->term()->isNumber();
    if (tr) {
        dst = static_cast<Number>(tr->value());
        return true;
    }
    else
        Print("WARNING: invalid NUMBER %s in '%s'\n", def->name()->value().data(), file);
    return false;
}
