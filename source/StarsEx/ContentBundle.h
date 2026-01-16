/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Chained collection of localized strings
*/

#ifndef ContentBundle_h
#define ContentBundle_h

#include "Types.h"
#include "Dictionary.h"
#include "Text.h"
#include "Locale_ss.h"

// +--------------------------------------------------------------------+

class ContentBundle
{
public:
    static const char* TYPENAME() { return "ContentBundle"; }

    static ContentBundle* GetInstance();

    explicit ContentBundle(Locale* locale=nullptr);
    virtual ~ContentBundle();

    bool Init();
    void UseLocale(Locale* locale);
    void Load(const char* bundle, Locale* locale);

    ContentBundle(ContentBundle&& that)         = delete;
    ContentBundle(const ContentBundle& that)    = delete;
    void operator = (const ContentBundle& that) = delete;

    const Text&       GetName()                  const { return name; }
    Text              GetText(const char* key)   const;
    bool              IsLoaded()                 const { return !values.isEmpty(); }

protected:
    void LoadBundle(const char* filename);
    Text FindFile(const char* bundle, Locale* locale);

    Text              name;
    Dictionary<Text>  values;
};

#endif  // ContentBundle_h

