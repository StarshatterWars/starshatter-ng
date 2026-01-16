/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network Server Pump for HTTP Server
*/

#include "HttpClient.h"

#include "HttpParam.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "List.h"
#include "NetAddr.h"
#include "NetClient.h"

// +-------------------------------------------------------------------+

HttpClient::HttpClient(const NetAddr& server_addr)
    : NetClient(server_addr)
{
}

HttpClient::~HttpClient()
{
    cookies.destroy();
}

HttpResponse*
HttpClient::DoRequest(HttpRequest& request)
{
    // add existing cookies to request before sending:
    CombineCookies(request.GetCookies(), cookies);

    Text req = request.operator Text();
    Text msg = SendRecv(req);
    HttpResponse*  response = new HttpResponse(msg);

    if (response) {
        // save cookies returned in response:
        CombineCookies(cookies, response->GetCookies());
    }

    return response;
}

void
HttpClient::CombineCookies(List<HttpParam>& dst, List<HttpParam>& src)
{
    for (int i = 0; i < src.size(); i++) {
        HttpParam* s = src[i];
        HttpParam* d = dst.find(s);

        if (d) {
            d->value = s->value;
        }
        else {
            HttpParam* cookie = new HttpParam(s->name, s->value);
            if (cookie)
                dst.append(cookie);
        }
    }
}