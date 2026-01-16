/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    HTTP Servlet for File Transfer
*/

#pragma once

#include <HttpRequest.h>
#include <HttpResponse.h>
#include <NetUser.h>

#include "NetAdminServer.h"


class NetFileServlet : public NetAdminServlet
{
public:
    NetFileServlet();
    virtual ~NetFileServlet();
    virtual bool DoGet(HttpRequest& request,  HttpResponse& response);
};


class NetWebServlet : public NetAdminServlet
{
public:
    NetWebServlet();
    virtual ~NetWebServlet();
    virtual bool DoGet(HttpRequest& request,  HttpResponse& response);
};
