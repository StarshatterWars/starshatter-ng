/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    HTTP/1.1 client class
*/

#ifndef HttpClient_h
#define HttpClient_h

#include "HttpParam.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "List.h"
#include "NetAddr.h"
#include "NetClient.h"

// +-------------------------------------------------------------------+

class HttpClient : public NetClient
{
public:
    static const char* TYPENAME() { return "HttpClient"; }

    HttpClient(const NetAddr& server_addr);
    virtual ~HttpClient();

    int operator == (const HttpClient& c)   const { return this == &c; }

    HttpResponse*     DoRequest(HttpRequest& request);

protected:
    void              CombineCookies(List<HttpParam>& dst, List<HttpParam>& src);

    List<HttpParam>   cookies;
};


#endif  // HttpClient_h
