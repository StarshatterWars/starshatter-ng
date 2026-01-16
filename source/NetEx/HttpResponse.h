/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#ifndef HttpResponse_h
#define HttpResponse_h

#include "HttpParam.h"
#include "List.h"
#include "Text.h"


class HttpResponse
{
public:
    static const char* TYPENAME() { return "HttpResponse"; }

    enum STATUS {
        SC_CONTINUE             = 100,
        SC_SWITCHING_PROTOCOLS  = 101,

        SC_OK                   = 200,
        SC_CREATED              = 201,
        SC_ACCEPTED             = 202,
        SC_NON_AUTHORITATIVE    = 203,
        SC_NO_CONTENT           = 204,
        SC_RESET_CONTENT        = 205,
        SC_PARTIAL_CONTENT      = 206,

        SC_MULTIPLE_CHOICES     = 300,
        SC_MOVED_PERMANENTLY    = 301,
        SC_FOUND                = 302,
        SC_SEE_OTHER            = 303,
        SC_NOT_MODIFIED         = 304,
        SC_USE_PROXY            = 305,
        SC_TEMPORARY_REDIRECT   = 307,

        SC_BAD_REQUEST          = 400,
        SC_UNAUTHORIZED         = 401,
        SC_PAYMENT_REQUIRED     = 402,
        SC_FORBIDDEN            = 403,
        SC_NOT_FOUND            = 404,
        SC_METHOD_NOT_ALLOWED   = 405,
        SC_NOT_ACCEPTABLE       = 406,
        SC_PROXY_AUTH_REQ       = 407,
        SC_REQUEST_TIME_OUT     = 408,
        SC_CONFLICT             = 409,
        SC_GONE                 = 410,
        SC_LENGTH_REQUIRED      = 411,

        SC_SERVER_ERROR         = 500,
        SC_NOT_IMPLEMENTED      = 501,
        SC_BAD_GATEWAY          = 502,
        SC_SERVICE_UNAVAILABLE  = 503,
        SC_GATEWAY_TIMEOUT      = 504,
        SC_VERSION_NOT_SUPPORTED= 505
    };


    HttpResponse(int status=500, const char* content=0);
    HttpResponse(const char* response);
    ~HttpResponse();

    operator Text();

    void              ParseResponse(Text request);
    void              ParseCookie(const char* param);

    int               Status()       const { return status;  }
    void              SetStatus(int s)     { status = s;     }

    Text              Content()      const { return content; }
    void              SetContent(Text t)   { content = t;    }
    void              AddContent(Text t)   { content += t;   }

    List<HttpParam>&  GetHeaders()         { return headers; }
    List<HttpParam>&  GetCookies()         { return cookies; }

    Text              GetHeader(const char* name);
    void              SetHeader(const char* name, const char* value);
    void              AddHeader(const char* name, const char* value);
    Text              GetCookie(const char* name);
    void              SetCookie(const char* name, const char* value);
    void              AddCookie(const char* name, const char* value);

    void              SendRedirect(const char* url);

private:
    int               status;
    Text              content;

    List<HttpParam>   headers;
    List<HttpParam>   cookies;
};


#endif  // HttpResponse_h
