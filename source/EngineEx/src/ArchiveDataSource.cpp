/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "ArchiveDataSource.h"

#include <cstdint>
#include <memory>
#include <utility>

#include <starshatter/foundation/reader.h>
#include <Archive.h>
#include <List.h>
#include <Text.h>


using starshatter::foundation::Reader;


ArchiveDataSource::ArchiveDataSource(const char* name) :
    ArchiveDataSource(new ArchiveEx::Archive(name))
{
}


ArchiveDataSource::ArchiveDataSource(ArchiveEx::Archive* archive) :
    DataSource(),
    m_archive {archive}
{
}


ArchiveDataSource::~ArchiveDataSource()
{
    delete m_archive;
}


bool
ArchiveDataSource::Find(const Text& prefix, const char* name) const
{
    const int index = m_archive->Find(prefix.concat(name));
    return index > -1;
}


int
ArchiveDataSource::ListFiles(const Text& prefix, Text filter, List<Text>& items, bool recurse) const
{
    (void) recurse;  // Lookup in Archives was always recursive so far
    filter = filter.replace("*", "");  // Wildcards worked only on boundaries
    const int first = prefix.length();
    m_archive->ForEachEntry([&](const char* path){
        Text name{path};
        name.setSensitive(false);
        if (name.contains(prefix) && name.contains(filter)) {
            const auto without_prefix = name.substring(first, name.length());
            if (!items.contains(&without_prefix))
                items.append(new Text(without_prefix));
        }
    });
    return items.size();
}


int
ArchiveDataSource::Load(const Text& prefix, const char* name, std::uint8_t*& buf, bool null_terminate) const
{
    const int result = m_archive->Extract(prefix.concat(name), buf, null_terminate);
    return -1 == result ? 0 : result;  // -1 would be preferable, but 0 is legacy
}


Reader
ArchiveDataSource::Open(const Text& prefix, const char* name) const
{
    std::uint8_t* buf = nullptr;
    const auto len = Load(prefix, name, buf, true);
    std::unique_ptr<const char[]> managed{reinterpret_cast<const char*>(buf)};
    if (len > 0)
        return {std::move(managed)};
    return {};
}
