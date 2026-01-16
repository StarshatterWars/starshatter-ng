/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#pragma once

#include <cstdint>

#include <starshatter/foundation/reader.h>
#include <List.h>
#include <Text.h>


class DataSource
{
public:
    using Reader = starshatter::foundation::Reader;

    static const char* TYPENAME() { return "DataSource"; }

    DataSource();
    virtual ~DataSource();

    int Id() const;

    virtual bool Find(const Text& prefix, const char* name) const = 0;
    virtual int ListFiles(const Text& prefix, Text filter, List<Text>& items, bool recurse=false) const = 0;
    virtual int Load(const Text& prefix, const char* name, std::uint8_t*& buf, bool null_terminate=false) const = 0;
    virtual Reader Open(const Text& prefix, const char* name) const = 0;

protected:
    const int m_id;
    static int s_next_id;
};
