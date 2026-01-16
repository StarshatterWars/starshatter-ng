/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network Server Pump for HTTP Server
*/

#ifndef HttpServer_h
#define HttpServer_h

#include <cstdint>

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "NetAddr.h"
#include "NetServer.h"
#include "Text.h"


class HttpServer : public NetServer
{
public:
    static const char* TYPENAME() { return "HttpServer"; }

    HttpServer(std::uint16_t port, int poolsize=1);
    virtual ~HttpServer();

    int operator == (const HttpServer& l) const { return addr == l.addr; }

    virtual Text      ProcessRequest(Text request, const NetAddr& addr);
    virtual Text      DefaultResponse();
    virtual Text      ErrorResponse();

    virtual bool      DoGet(HttpRequest& request,  HttpResponse& response);
    virtual bool      DoPost(HttpRequest& request, HttpResponse& response);
    virtual bool      DoHead(HttpRequest& request, HttpResponse& response);

    virtual Text      GetServerName();
    virtual void      SetServerName(const char* name);

protected:
    Text              http_server_name;
};


#endif  // HttpServer_h
