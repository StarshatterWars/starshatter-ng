/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#ifndef HttpRequest_h
#define HttpRequest_h

#include "HttpParam.h"
#include "List.h"
#include "NetAddr.h"


class HttpRequest
{
public:
    static const char* TYPENAME() { return "HttpRequest"; }

    enum METHOD {
        HTTP_OPTIONS,
        HTTP_GET,
        HTTP_HEAD,
        HTTP_POST,
        HTTP_PUT,
        HTTP_DELETE,
        HTTP_TRACE,
        HTTP_CONNECT
    };

    HttpRequest(const char* request=nullptr);
    ~HttpRequest();

    operator Text();

    void              ParseRequest(Text request);
    void              ParseCookie(const char* param);

    int               Method()       const { return method;        }
    Text              URI()          const { return uri;           }
    Text              Content()      const { return content;       }
    Text              RequestLine()  const { return request_line;  }

    List<HttpParam>&  GetQuery()           { return query;         }
    List<HttpParam>&  GetHeaders()         { return headers;       }
    List<HttpParam>&  GetCookies()         { return cookies;       }

    NetAddr           GetClientAddr()            const { return client_addr; }
    void              SetClientAddr(const NetAddr& a)  { client_addr = a;    }

    Text              GetParam(const char* name);

    Text              GetHeader(const char* name);
    void              SetHeader(const char* name, const char* value);
    void              AddHeader(const char* name, const char* value);
    Text              GetCookie(const char* name);
    void              SetCookie(const char* name, const char* value);
    void              AddCookie(const char* name, const char* value);

    Text              DecodeParam(const char* value);
    static Text       EncodeParam(const char* value);

private:
    int               method;
    Text              uri;
    Text              content;
    Text              request_line;
    NetAddr           client_addr;

    List<HttpParam>   query;
    List<HttpParam>   headers;
    List<HttpParam>   cookies;
};


#endif  // HttpRequest_h
