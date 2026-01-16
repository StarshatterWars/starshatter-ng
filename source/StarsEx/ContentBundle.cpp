/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Chained collection of localized strings
*/

#include "ContentBundle.h"
#include "DataLoader.h"
#include "Utils.h"

// +--------------------------------------------------------------------+

ContentBundle* ContentBundle::GetInstance()
{
    static ContentBundle instance{};
    return &instance;
}

// +--------------------------------------------------------------------+

ContentBundle::ContentBundle(Locale* locale)
{
    UseLocale(locale);
}

// +--------------------------------------------------------------------+

ContentBundle::~ContentBundle()
{
}

// +--------------------------------------------------------------------+

bool
ContentBundle::Init()
{
    DataLoader* loader = DataLoader::GetLoader();
    List<Text> bundles;

    loader->SetDataPath("Content/");
    loader->ListFiles("content*", bundles);

    ListIter<Text> iter = bundles;
    while (++iter) {
        Text* filename = iter.value();
        int n = filename->indexOf('_');
        if (n > 0) {
            Locale::ParseLocale(filename->data() + n);  // unused?
        }
    }

    loader->SetDataPath(0);
    return true;
}

// +--------------------------------------------------------------------+

void
ContentBundle::UseLocale(Locale* locale)
{
    DataLoader* loader = DataLoader::GetLoader();
    loader->SetDataPath("Content/");
    Load("content", locale);
    loader->SetDataPath(nullptr);
}

// +--------------------------------------------------------------------+

void
ContentBundle::Load(const char* bundle, Locale* locale)
{
    if (values.size() > 0) {
        values.clear();
    }
    Text file = FindFile(bundle, locale);
    if (file.length() > 0) {
        LoadBundle(file);
    }
}

// +--------------------------------------------------------------------+

Text
ContentBundle::GetText(const char* key) const
{
    return values.find(key, Text(key));
}

// +--------------------------------------------------------------------+

Text
ContentBundle::FindFile(const char* bundle, Locale* locale)
{
    Text        result;
    Text        basename = Text(bundle);
    DataLoader* loader = DataLoader::GetLoader();

    if (loader && bundle) {
        if (locale) {
            result = basename + locale->GetFullCode() + ".txt";

            if (loader->FindFile(result))
            return result;

            result = basename + "_" + locale->GetLanguage() + ".txt";

            if (loader->FindFile(result))
            return result;
        }

        result = basename + ".txt";

        if (loader->FindFile(result))
        return result;
    }

    return Text();
}

// +--------------------------------------------------------------------+

void
ContentBundle::LoadBundle(const char* filename)
{
    DataLoader* loader = DataLoader::GetLoader();
    if (loader && filename && *filename) {
        BYTE* buffer = 0;
        loader->LoadBuffer(filename, buffer, true, true);
        if (buffer && *buffer) {
            char  key[1024];
            char  val[2048];
            char* p = (char*) buffer;
            int   s = 0, ik = 0, iv = 0;

            key[0] = 0;
            val[0] = 0;

            while (*p) {
                if (*p == '=') {
                    s = 1;
                }
                else if (*p == '\n' || *p == '\r') {
                    if (key[0] && val[0])
                    values.insert(Text(key).trim(), Text(val).trim());

                    ZeroMemory(key, 1024);
                    ZeroMemory(val, 2048);
                    s   = 0;
                    ik  = 0;
                    iv  = 0;
                }
                else if (s == 0) {
                    if (!key[0]) {
                        if (*p == '#') {
                            s = -1; // comment
                        }
                        else if (!isspace(*p)) {
                            key[ik++] = *p;
                        }
                    }
                    else {
                        key[ik++] = *p;
                    }
                }
                else if (s == 1) {
                    if (!isspace(*p)) {
                        s = 2;
                        val[iv++] = *p;
                    }
                }
                else if (s == 2) {
                    val[iv++] = *p;
                }

                p++;
            }

            loader->ReleaseBuffer(buffer);
        }
    }
}
