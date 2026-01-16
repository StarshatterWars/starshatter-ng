/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "FilesystemDataSource.h"

#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <memory>

#include <starshatter/foundation/reader.h>
#include <List.h>
#include <Text.h>
#include <Utils.h>

#include "reader/file.h"


using starshatter::foundation::Reader;


FileSystemDataSource::FileSystemDataSource(const char* path) :
    DataSource(),
    m_path {path}
{
}


FileSystemDataSource::~FileSystemDataSource()
{
}


bool
FileSystemDataSource::Find(const Text& prefix, const char* name) const
{
    std::filesystem::path full_path {m_path};
    full_path.append(prefix.data());
    full_path.append(name);
    return std::filesystem::is_regular_file(full_path);
}


int
FileSystemDataSource::ListFiles(const Text& prefix, Text filter, List<Text>& items, bool recurse) const
{
    std::filesystem::path full_path {m_path};
    full_path.append(prefix.data());
    filter = filter.replace("*", "");
    const auto first = full_path.string().length();
    const auto check = [&](const std::filesystem::directory_entry& entry){
        const auto filename = entry.path().filename().string();  // more of a reason to switch to string soon
        const auto index = filename.find(filter.data());
        if (index != decltype(filename)::npos) {
            Text path = entry.path().string().c_str();
            items.append(new Text(path.substring(first, path.length())));
        }
    };
    try {
        if (recurse) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(full_path))
                check(entry);
        }
        else {
            for (const auto& entry : std::filesystem::directory_iterator(full_path))
                check(entry);
        }
    }
    catch (const std::filesystem::filesystem_error& err) {
        Print("FS::ListFiles: %s\n", err.what());
    }
    return items.size();
}


int
FileSystemDataSource::Load(const Text& prefix, const char* name, std::uint8_t*& buf, bool null_terminate) const
{
    std::filesystem::path full_path {m_path};
    full_path.append(prefix.data());
    full_path.append(name);
    auto f = std::fopen(full_path.string().c_str(), "rb");
    if (!f)
        return 0;  // Again, -1 would be better to differentiate an error from an empty file
    std::fseek(f, 0, SEEK_END);
    int length = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);
    if (null_terminate) {
        buf = new std::uint8_t[length + 1];
        if (buf)
            buf[length] = 0;
    }
    else {
        buf = new std::uint8_t[length];
    }
    int bytes = 0;
    if (buf)
        bytes = std::fread(buf, length, 1, f);
    std::fclose(f);
    return bytes;
}


Reader
FileSystemDataSource::Open(const Text& prefix, const char* name) const
{
    std::filesystem::path full_path {m_path};
    full_path.append(prefix.data());
    full_path.append(name);
    return {std::make_unique<starshatter::foundation::FileReader>(
        std::fstream(full_path, std::ios::binary | std::ios::in)
    )};
}
