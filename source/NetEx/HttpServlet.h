/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network Server Pump for HTTP Server
*/

#ifndef HttpServlet_h
#define HttpServlet_h

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpSession.h"


class HttpServlet
{
public:
    static const char* TYPENAME() { return "HttpServlet"; }

    HttpServlet();
    virtual ~HttpServlet();

    virtual bool      Service(HttpRequest& request, HttpResponse& response);

    virtual bool      DoGet(HttpRequest& request,  HttpResponse& response);
    virtual bool      DoPost(HttpRequest& request, HttpResponse& response);
    virtual bool      DoHead(HttpRequest& request, HttpResponse& response);

    virtual HttpSession* GetSession()               { return session; }
    virtual void         SetSession(HttpSession* s) { session = s; }

protected:
    HttpSession*      session;
};


#endif  // HttpServlet_h
