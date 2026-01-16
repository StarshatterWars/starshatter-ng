/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo

    OVERVIEW
    ========
    Network Server Pump for HTTP Server
*/

#include "HttpServlet.h"

#include <stdio.h>

#include "HttpRequest.h"
#include "HttpResponse.h"


HttpServlet::HttpServlet() :
    session(nullptr)
{
}


HttpServlet::~HttpServlet()
{
}


bool
HttpServlet::Service(HttpRequest& request, HttpResponse& response)
{
    bool result = false;

    switch (request.Method()) {
    case HttpRequest::HTTP_GET:
        result = DoGet(request, response);
        break;

    case HttpRequest::HTTP_POST:
        result = DoPost(request, response);
        break;

    case HttpRequest::HTTP_HEAD:
        result = DoHead(request, response);
        break;

    default:
        break;
    }

    return result;
}


bool
HttpServlet::DoGet(HttpRequest& request, HttpResponse& response)
{
    return false;
}


bool
HttpServlet::DoPost(HttpRequest& request, HttpResponse& response)
{
    return DoGet(request, response);
}


bool
HttpServlet::DoHead(HttpRequest& request, HttpResponse& response)
{
    if (DoGet(request, response)) {
        int len = response.Content().length();

        char buffer[256];
        sprintf(buffer, "%d", len);
        response.SetHeader("Content-Length", buffer);
        response.SetContent("");

        return true;
    }

    return false;
}
