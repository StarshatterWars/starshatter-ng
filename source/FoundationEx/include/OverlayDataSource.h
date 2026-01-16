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

#include "DataSource.h"


class OverlayDataSource : public DataSource
{
public:
    OverlayDataSource();
    ~OverlayDataSource();

    bool Find(const Text& prefix, const char* name) const override;
    int ListFiles(const Text& prefix, Text filter, List<Text>& items, bool recurse=false) const override;
    int Load(const Text& prefix, const char* name, std::uint8_t*& buf, bool null_terminate=false) const override;
    Reader Open(const Text& prefix, const char* name) const override;

    DataSource* Mount(DataSource* source, int pos=-1);
    DataSource* Unmount(DataSource* source);
    DataSource* Unmount(int index);
    DataSource* UnmountId(int ref);
    DataSource* ById(int ref);

protected:
    List<DataSource> m_sources;
};
