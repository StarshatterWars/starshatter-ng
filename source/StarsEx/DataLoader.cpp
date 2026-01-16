/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo

*/

#include "DataLoader.h"

#include <cstdint>
#include <memory>

#include <List.h>
#include <Text.h>

#include "ArchiveDataSource.h"
#include "Bitmap.h"
#include "D3DXImage.h"
#include "DataSource.h"
#include "FilesystemDataSource.h"
#include "OverlayDataSource.h"
#include "Pcx.h"
#include "Sound.h"
#include "Wave.h"


DataLoader* DataLoader::loader = nullptr;


DataLoader::DataLoader() :
    datapath(""),
    enable_media(true),
    overlay(new OverlayDataSource()),
    work_directory_source(new FileSystemDataSource())
{
}


DataLoader::~DataLoader()
{
    if (overlay)
        delete overlay;
    if (work_directory_source)
        delete work_directory_source;
}


void
DataLoader::Initialize()
{
    if (!loader) {
        loader = new DataLoader;
    }
}


void
DataLoader::Close()
{
    Bitmap::ClearCache();
    if (loader) {
        delete loader;
        loader = nullptr;
    }
}

// +--------------------------------------------------------------------+

const char*
DataLoader::LastError() const
{
    return last_error;
}

// +--------------------------------------------------------------------+

void
DataLoader::UseFileSystem(bool use)
{
    if (work_directory_source) {
        if (!use) {
            delete work_directory_source;  // This is wasteful
            work_directory_source = nullptr;
        }
    }
    else if (use) {
        work_directory_source = new FileSystemDataSource();
    }
}

void
DataLoader::EnableMedia(bool enable)
{
    enable_media = enable;
}

// +--------------------------------------------------------------------+

int
DataLoader::MountDatafile(const char* name, int pos)
{
    FILE* f = fopen(name, "rb");
    if (!f) {
        last_error = Text::format("Could not open datafile '%s'", name);
        return FAILED;
    }
    fclose(f);
    auto archive = std::make_unique<ArchiveEx::Archive>(name);
    if (!archive || archive->NumEntries() < 1) {
        last_error = Text::format("Invalid datafile '%s'", name);
        return FAILED;
    }
    DataSource* source = new ArchiveDataSource(archive.release());
    static_cast<OverlayDataSource*>(overlay)->Mount(source, pos);  // Consider exposing it as such as member?
    return source->Id();
}

int
DataLoader::UnmountSource(int src)
{
    if (auto source = static_cast<OverlayDataSource*>(overlay)->UnmountId(src)) {
        delete source;
        return src;
    }
    last_error = Text::format("Source with id is not mounted: %d", src);
    return FAILED;
}

// +--------------------------------------------------------------------+

void
DataLoader::SetDataPath(const char* path)
{
    if (path)
    datapath = path;
    else
    datapath = "";
}

// +--------------------------------------------------------------------+


DataLoader::Reader
DataLoader::Open(const char* name)
{
    if (work_directory_source) {
        if (work_directory_source->Find(datapath, name))
            return work_directory_source->Open(datapath, name);
    }
    return overlay->Open(datapath, name);
}


bool
DataLoader::FindFile(const char* name)
{
    if (work_directory_source) {
        if (work_directory_source->Find(datapath, name))
            return true;
    }
    return overlay->Find(datapath, name);
}

// +--------------------------------------------------------------------+

int
DataLoader::ListFiles(const char* filter, List<Text>& list, bool recurse)
{
    list.destroy();
    if (work_directory_source)
        work_directory_source->ListFiles(datapath, filter, list, recurse);
    return overlay->ListFiles(datapath, filter, list, recurse);
}

int
DataLoader::ListArchiveFiles(int src, const char* filter, List<Text> &list)
{
    if (src < 0) {
        if (work_directory_source)
            work_directory_source->ListFiles(datapath, filter, list, true);
        return list.size();
    }
    if (auto source = static_cast<OverlayDataSource*>(overlay)->ById(src))
        return source->ListFiles(datapath, filter, list, true);
    return list.size();
}

// +--------------------------------------------------------------------+

int
DataLoader::LoadBuffer(const char* name, std::uint8_t*& buf, bool null_terminate, bool optional)
{
    buf = nullptr;
    if (work_directory_source) {
        int len = work_directory_source->Load(datapath, name, buf, null_terminate);
        if (len > 0)
            return len;
    }
    const auto len = overlay->Load(datapath, name, buf, null_terminate);
    if (len > 0)
        return len;
    if (!optional)
        Print("WARNING - DataLoader could not load buffer '%s' '%s'\n", datapath.data(), name);
    return 0;
}

// +--------------------------------------------------------------------+

int
DataLoader::LoadPartialFile(const char* name, std::uint8_t*& buf, int max_load, bool optional)
{
    buf = 0;

    // assemble file name:
    char filename[1024];
    strcpy_s(filename, datapath);
    strcat_s(filename, name);

    // first check current directory:
    FILE* f;
    ::fopen_s(&f, filename, "rb");

    if (f) {
        ::fseek(f, 0, SEEK_END);
        int len = ftell(f);
        ::fseek(f, 0, SEEK_SET);

        if (len > max_load) {
            len = max_load;
        }

        buf = new std::uint8_t[len];

        if (buf)
        ::fread(buf, len, 1, f);

        ::fclose(f);

        return len;
    }

    if (!optional)
    Print("WARNING - DataLoader could not load partial file '%s'\n", filename);
    return 0;
}

int
DataLoader::fread(void* buffer, size_t size, size_t count, std::uint8_t*& stream)
{
    CopyMemory(buffer, stream, size*count);
    stream += size*count;

    return size*count;
}

// +--------------------------------------------------------------------+

void
DataLoader::ReleaseBuffer(std::uint8_t*& buf)
{
    delete [] buf;
    buf = 0;
}

// +--------------------------------------------------------------------+

int
DataLoader::CacheBitmap(const char* name, Bitmap*& bitmap, int type, bool optional)
{
    int      result = 0;

    // search cache:
    bitmap = Bitmap::CheckCache(name);
    if (bitmap) return 1;

    // not in cache yet:
    bitmap = new Bitmap;

    if (bitmap)
    result = LoadBitmap(name, *bitmap, type, optional);

    if (result && bitmap)
    Bitmap::AddToCache(bitmap);

    return result;
}

// +--------------------------------------------------------------------+

int
DataLoader::LoadBitmap(const char* name, Bitmap& bitmap, int type, bool optional)
{
    if (!enable_media)
    return 0;

    int result = LoadIndexed(name, bitmap, type);

    // check for a matching high color bitmap:
    if (result == 1) {
        int hi_result = LoadHiColor(name, bitmap, type);

        if (hi_result == 2)
        result = 3;
    }

    bitmap.SetFilename(name);

    if (!result && !optional)
    Print("WARNING - DataLoader could not load bitmap '%s%s'\n", datapath.data(), name);

    return result;
}

// +--------------------------------------------------------------------+

int
DataLoader::LoadTexture(const char* name, Bitmap*& bitmap, int type, bool preload_cache, bool optional)
{
    if (!enable_media)
    return 0;

    int      result = 0;

    // assemble file name:
    char filename[256];
    strcpy_s(filename, datapath);
    strcat_s(filename, name);

    // search cache:
    bitmap = Bitmap::CheckCache(filename);
    if (bitmap) return 1;

    // not in cache yet:
    bitmap = new Bitmap;

    if (bitmap) {
        result = LoadHiColor(name, *bitmap, type);

        if (!result) {
            result = LoadIndexed(name, *bitmap, type);
        }

        bitmap->SetFilename(filename);

        if (result) {
            bitmap->MakeTexture();
            Bitmap::AddToCache(bitmap);
        }
        else {
            delete bitmap;
            bitmap = 0;

            if (!optional)
            Print("WARNING - DataLoader could not load texture '%s%s'\n", datapath.data(), name);
        }
    }
    else if (!optional) {
        Print("WARNING - DataLoader could not allocate texture '%s%s'\n", datapath.data(), name);
    }

    return result;
}

// +--------------------------------------------------------------------+

int
DataLoader::LoadIndexed(const char* name, Bitmap& bitmap, int type)
{
    if (!enable_media)
    return 0;

    int         result = 0;
    PcxImage    pcx;
    D3DXImage   d3dx;
    bool        pcx_file = strstr(name, ".pcx") || strstr(name, ".PCX");

    // assemble file name:
    char filename[256];
    strcpy_s(filename, datapath);
    strcat_s(filename, name);

    // first try to load from current directory:
    bool loaded = false;

    if (work_directory_source) {
        if (pcx_file)
        loaded = pcx.Load(filename) == PCX_OK;

        else
        loaded = d3dx.Load(filename);
    }

    if (!loaded) {
        std::uint8_t* tmp_buf = nullptr;
        const int len = overlay->Load(datapath, name, tmp_buf);
        if (len > 0) {
            if (pcx_file)
                pcx.LoadBuffer(tmp_buf, len);
            else
                d3dx.LoadBuffer(tmp_buf, len);
        }
        if (tmp_buf)
            ReleaseBuffer(tmp_buf);
    }

    // now copy the image into the bitmap:
    if (pcx_file) {
        if (pcx.bitmap) {
            bitmap.CopyImage(pcx.width, pcx.height, pcx.bitmap, type);
            result = 1;
        }

        else if (pcx.himap) {
            bitmap.CopyHighColorImage(pcx.width, pcx.height, pcx.himap, type);
            result = 2;
        }

        if (result == 2)
        LoadAlpha(name, bitmap, type);
    }

    else {
        if (d3dx.image) {
            bitmap.CopyHighColorImage(d3dx.width, d3dx.height, d3dx.image, type);
            result = 2;
        }

        if (result == 2) {
            LoadAlpha(name, bitmap, type);
        }
    }

    return result;
}

int
DataLoader::LoadHiColor(const char* name, Bitmap& bitmap, int type)
{
    if (!enable_media)
    return 0;

    int         result = 0;
    PcxImage    pcx;
    D3DXImage   d3dx;
    bool        pcx_file = strstr(name, ".pcx") || strstr(name, ".PCX");
    bool        bmp_file = strstr(name, ".bmp") || strstr(name, ".BMP");
    bool        png_file = strstr(name, ".png") || strstr(name, ".PNG");

    // check for a matching high color bitmap:
    char filename[256];
    char name2[256];
    strcpy_s(name2, name);

    char* dot = strrchr(name2, '.');
    if (dot && pcx_file)
    strcpy(dot, "+.pcx");
    else if (dot && bmp_file)
    strcpy(dot, "+.bmp");
    else if (dot && png_file)
    strcpy(dot, "+.png");
    else
    return result;

    strcpy_s(filename, datapath);
    strcat_s(filename, name2);

    // first try to load from current directory:
    bool loaded = false;

    if (work_directory_source) {
        if (pcx_file)
        loaded = pcx.Load(filename) == PCX_OK;

        else
        loaded = d3dx.Load(filename);
    }

    if (!loaded) {
        std::uint8_t* tmp_buf = nullptr;
        const int len = overlay->Load(datapath, name2, tmp_buf);
        if (len > 0) {
            if (pcx_file)
                pcx.LoadBuffer(tmp_buf, len);
            else
                d3dx.LoadBuffer(tmp_buf, len);
        }
        if (tmp_buf)
            ReleaseBuffer(tmp_buf);
    }

    // now copy the image into the bitmap:
    if (pcx_file && pcx.himap) {
        bitmap.CopyHighColorImage(pcx.width, pcx.height, pcx.himap, type);
        result = 2;
    }

    else if (d3dx.image) {
        bitmap.CopyHighColorImage(d3dx.width, d3dx.height, d3dx.image, type);
        result = 2;
    }

    if (result == 2)
    LoadAlpha(name, bitmap, type);

    return result;
}

int
DataLoader::LoadAlpha(const char* name, Bitmap& bitmap, int type)
{
    if (!enable_media)
    return 0;

    PcxImage    pcx;
    D3DXImage   d3dx;
    bool        pcx_file = strstr(name, ".pcx") || strstr(name, ".PCX");
    bool        bmp_file = strstr(name, ".bmp") || strstr(name, ".BMP");
    bool        png_file = strstr(name, ".png") || strstr(name, ".PNG");
    bool        tga_file = strstr(name, ".tga") || strstr(name, ".TGA");

    // check for an associated alpha-only (grayscale) bitmap:
    char filename[256];
    char name2[256];
    strcpy_s(name2, name);
    char* dot = strrchr(name2, '.');
    if (dot && pcx_file)
    strcpy(dot, "@.pcx");
    else if (dot && bmp_file)
    strcpy(dot, "@.bmp");
    else if (dot && png_file)
    strcpy(dot, "@.png");
    else if (dot && tga_file)
    strcpy(dot, "@.tga");
    else
    return 0;

    strcpy_s(filename, datapath);
    strcat_s(filename, name2);

    // first try to load from current directory:
    bool loaded = false;

    if (work_directory_source) {
        if (pcx_file)
        loaded = pcx.Load(filename) == PCX_OK;

        else
        loaded = d3dx.Load(filename);
    }

    if (!loaded) {
        std::uint8_t* tmp_buf = nullptr;
        const int len = overlay->Load(datapath, name2, tmp_buf);
        if (len > 0) {
            if (pcx_file)
                pcx.LoadBuffer(tmp_buf, len);
            else
                d3dx.LoadBuffer(tmp_buf, len);
        }
        if (tmp_buf)
            ReleaseBuffer(tmp_buf);
    }

    // now copy the alpha values into the bitmap:
    if (pcx_file && pcx.bitmap) {
        bitmap.CopyAlphaImage(pcx.width, pcx.height, pcx.bitmap);
    }
    else if (pcx_file && pcx.himap) {
        bitmap.CopyAlphaRedChannel(pcx.width, pcx.height, pcx.himap);
    }
    else if (d3dx.image) {
        bitmap.CopyAlphaRedChannel(d3dx.width, d3dx.height, d3dx.image);
    }

    return 0;
}

// +--------------------------------------------------------------------+

int
DataLoader::LoadSound(const char* name, Sound*& snd, std::uint32_t flags, bool optional)
{
    if (!enable_media)
    return 0;

    if (strstr(name, ".ogg"))
    return LoadStream(name, snd, optional);

    int result = 0;

    WAVE_HEADER    head;
    WAVE_FMT       fmt;
    WAVE_FACT      fact;
    WAVE_DATA      data;
    WAVEFORMATEX   wfex;
    LPBYTE         wave;

    LPBYTE         buf;
    LPBYTE         p;
    int            len;

    ZeroMemory(&head, sizeof(head));
    ZeroMemory(&fmt,  sizeof(fmt));
    ZeroMemory(&fact, sizeof(fact));
    ZeroMemory(&data, sizeof(data));

    len = LoadBuffer(name, buf, false, optional);

    if (len > sizeof(head)) {
        CopyMemory(&head, buf, sizeof(head));

        if (head.RIFF == MAKEFOURCC('R', 'I', 'F', 'F') &&
                head.WAVE == MAKEFOURCC('W', 'A', 'V', 'E')) {

            p = buf + sizeof(WAVE_HEADER);

            do {
                DWORD chunk_id = *((LPDWORD) p);

                switch (chunk_id) {
                case MAKEFOURCC('f', 'm', 't', ' '):
                    CopyMemory(&fmt, p, sizeof(fmt));
                    p += fmt.chunk_size + 8;
                    break;

                case MAKEFOURCC('f', 'a', 'c', 't'):
                    CopyMemory(&fact, p, sizeof(fact));
                    p += fact.chunk_size + 8;
                    break;

                case MAKEFOURCC('s', 'm', 'p', 'l'):
                    CopyMemory(&fact, p, sizeof(fact));
                    p += fact.chunk_size + 8;
                    break;

                case MAKEFOURCC('d', 'a', 't', 'a'):
                    CopyMemory(&data, p, sizeof(data));
                    p += 8;
                    break;

                default:
                    ReleaseBuffer(buf);
                    return result;
                }
            }
            while (data.chunk_size == 0);

            wfex.wFormatTag      = fmt.wFormatTag;
            wfex.nChannels       = fmt.nChannels;
            wfex.nSamplesPerSec  = fmt.nSamplesPerSec;
            wfex.nAvgBytesPerSec = fmt.nAvgBytesPerSec;
            wfex.nBlockAlign     = fmt.nBlockAlign;
            wfex.wBitsPerSample  = fmt.wBitsPerSample;
            wfex.cbSize          = 0;
            wave                 = p;

            snd = Sound::Create(flags, &wfex, data.chunk_size, wave);

            if (snd)
            result = data.chunk_size;
        }
    }

    ReleaseBuffer(buf);
    return result;
}

int
DataLoader::LoadStream(const char* name, Sound*& snd, bool optional)
{
    if (!enable_media)
    return 0;

    if (!name)
    return 0;

    int namelen = strlen(name);

    if (namelen < 5)
    return 0;

    if ((name[namelen-3] == 'o' || name[namelen-3] == 'O') &&
            (name[namelen-2] == 'g' || name[namelen-2] == 'G') &&
            (name[namelen-1] == 'g' || name[namelen-1] == 'G')) {

        return LoadOggStream(name, snd);
    }

    int result = 0;

    WAVE_HEADER    head;
    WAVE_FMT       fmt;
    WAVE_FACT      fact;
    WAVE_DATA      data;
    WAVEFORMATEX   wfex;

    LPBYTE         buf;
    LPBYTE         p;
    int            len;

    ZeroMemory(&head, sizeof(head));
    ZeroMemory(&fmt,  sizeof(fmt));
    ZeroMemory(&fact, sizeof(fact));
    ZeroMemory(&data, sizeof(data));

    len = LoadPartialFile(name, buf, 4096, optional);

    if (len > sizeof(head)) {
        CopyMemory(&head, buf, sizeof(head));

        if (head.RIFF == MAKEFOURCC('R', 'I', 'F', 'F') &&
                head.WAVE == MAKEFOURCC('W', 'A', 'V', 'E')) {

            p = buf + sizeof(WAVE_HEADER);

            do {
                DWORD chunk_id = *((LPDWORD) p);

                switch (chunk_id) {
                case MAKEFOURCC('f', 'm', 't', ' '):
                    CopyMemory(&fmt, p, sizeof(fmt));
                    p += fmt.chunk_size + 8;
                    break;

                case MAKEFOURCC('f', 'a', 'c', 't'):
                    CopyMemory(&fact, p, sizeof(fact));
                    p += fact.chunk_size + 8;
                    break;

                case MAKEFOURCC('s', 'm', 'p', 'l'):
                    CopyMemory(&fact, p, sizeof(fact));
                    p += fact.chunk_size + 8;
                    break;

                case MAKEFOURCC('d', 'a', 't', 'a'):
                    CopyMemory(&data, p, sizeof(data));
                    p += 8;
                    break;

                default:
                    ReleaseBuffer(buf);
                    return result;
                }
            }
            while (data.chunk_size == 0);

            wfex.wFormatTag      = fmt.wFormatTag;
            wfex.nChannels       = fmt.nChannels;
            wfex.nSamplesPerSec  = fmt.nSamplesPerSec;
            wfex.nAvgBytesPerSec = fmt.nAvgBytesPerSec;
            wfex.nBlockAlign     = fmt.nBlockAlign;
            wfex.wBitsPerSample  = fmt.wBitsPerSample;
            wfex.cbSize          = 0;

            snd = Sound::Create(Sound::STREAMED, &wfex);

            if (snd) {
                // assemble file name:
                char filename[1024];
                strcpy_s(filename, datapath);
                strcat_s(filename, name);

                snd->StreamFile(filename, p - buf);

                result = data.chunk_size;
            }
        }
    }

    ReleaseBuffer(buf);
    return result;
}

int
DataLoader::LoadOggStream(const char* name, Sound*& snd)
{
    if (!enable_media)
    return 0;

    snd = Sound::CreateOggStream(name);

    return snd != 0;
}
