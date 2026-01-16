/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo

*/

#pragma once

#include <cstdint>

#include <starshatter/foundation/reader.h>
#include <List.h>
#include <Text.h>

#include "Bitmap.h"
#include "DataSource.h"
#include "Sound.h"


class DataLoader
{
public:
    using Reader = starshatter::foundation::Reader;

    static constexpr int FAILED {-1};
    static const char* TYPENAME() { return "DataLoader"; }

    DataLoader();
    virtual ~DataLoader();

    static DataLoader*   GetLoader() { return loader; }
    static void          Initialize();
    static void          Close();

    const char* LastError() const;

    void        UseFileSystem(bool use=true);
    void        EnableMedia(bool enable=true);

    int         MountDatafile(const char* name, int pos=-1);
    int         UnmountSource(int src);

    void        SetDataPath(const char* path);
    const char* GetDataPath() const { return datapath; }

    bool  IsFileSystemEnabled() const { return work_directory_source != nullptr; }
    bool  IsMediaLoadEnabled()  const { return enable_media;   }

    Reader Open(const char* name);
    bool  FindFile(const char* fname);
    int   ListFiles(const char* filter, List<Text>& list, bool recurse=false);
    int   ListArchiveFiles(int src, const char* filter, List<Text>& list);
    int   LoadBuffer(const char* name, std::uint8_t*&  buf, bool null_terminate=false, bool optional=false);
    int   LoadBitmap(const char* name, Bitmap& bmp, int type=0, bool optional=false);
    int   CacheBitmap(const char* name, Bitmap*& bmp, int type=0, bool optional=false);
    int   LoadTexture(const char* name, Bitmap*& bmp, int type=0, bool preload_cache=false, bool optional=false);
    int   LoadSound(const char* fname, Sound*& snd, std::uint32_t flags=0, bool optional=false);
    int   LoadStream(const char* fname, Sound*& snd, bool optional=false);

    void  ReleaseBuffer(std::uint8_t*& buf);
    int   fread(void* buffer, size_t size, size_t count, std::uint8_t*& stream);

private:
    int   LoadIndexed(const char* name, Bitmap& bmp, int type);
    int   LoadHiColor(const char* name, Bitmap& bmp, int type);
    int   LoadAlpha(  const char* name, Bitmap& bmp, int type);

    int   LoadPartialFile(const char* fname, std::uint8_t*& buf, int max_load, bool optional=false);
    int   LoadOggStream(const char* fname, Sound*& snd);

    Text        datapath;
    bool        enable_media;

    Text last_error;

    DataSource* overlay;
    DataSource* work_directory_source;
    static DataLoader* loader;
};
