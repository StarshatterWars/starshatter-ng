/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#ifndef HttpSession_h
#define HttpSession_h

#include "HttpParam.h"
#include "List.h"
#include "Text.h"


class HttpSession
{
public:
    static const char* TYPENAME() { return "HttpSession"; }

    HttpSession();
    virtual ~HttpSession();

    int operator == (const HttpSession& s) const { return id == s.id; }

    Text              GenerateUniqueID();

    Text              GetID()              const { return id;         }
    void              SetID(const char* i)       { id = i;            }
    int               GetLastAccess()      const { return access_time;}
    void              Access();

    List<HttpParam>&  GetAttributes()            { return attributes; }

    Text              GetAttribute(const char* name);
    void              SetAttribute(const char* name, const char* value);
    void              DelAttribute(const char* name);

    int               GetIntAttribute(const char* name);
    void              SetIntAttribute(const char* name, int value);
    void              DelIntAttribute(const char* name);

protected:
    Text              id;
    int               access_time;
    List<HttpParam>   attributes;
};


#endif  // HttpSession_h
