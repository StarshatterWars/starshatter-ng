/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network Server Pump for HTTP Server
*/

#ifndef HttpServletExec_h
#define HttpServletExec_h

#include <cstdint>
#include <thread>

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpServer.h"
#include "HttpServlet.h"
#include "HttpSession.h"
#include "List.h"


class HttpServletExec : public HttpServer
{
public:
    static const char* TYPENAME() { return "HttpServletExec"; }

    HttpServletExec(std::uint16_t port, int poolsize=1);
    virtual ~HttpServletExec();

    int operator == (const HttpServletExec& l) const { return addr == l.addr; }

    virtual bool         DoGet(HttpRequest& request,  HttpResponse& response);

    virtual HttpServlet* GetServlet(HttpRequest& request);
    virtual HttpSession* GetSession(HttpRequest& request);

    virtual void         CheckSessions();

    virtual int          GetSessionTimeout()  const { return session_timeout; }
    virtual void         SetSessionTimeout(int t)   { session_timeout = t;    }

protected:
    virtual void         DoSyncedCheck();

    List<HttpSession>    sessions;
    int                  session_timeout;
    std::thread          hsession;
    bool                 exec_shutdown;
};


#endif  // HttpServletExec_h
