/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    HTTP Servlet Engine for Admin Chat
*/

#pragma once

#include <HttpRequest.h>
#include <HttpResponse.h>
#include <Text.h>

#include "NetAdminServer.h"


class NetAdminChat : public NetAdminServlet
{
public:
    NetAdminChat();
    virtual ~NetAdminChat();
    virtual bool DoGet(HttpRequest& request, HttpResponse& response);
    virtual Text GetContent();
};
