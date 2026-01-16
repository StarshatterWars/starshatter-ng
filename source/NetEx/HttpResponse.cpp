/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "HttpResponse.h"

#ifndef _WIN32
#include <strings.h>
#endif

#include <cctype>
#include <cstdio>
#include <cstring>

#include "HttpParam.h"
#include "List.h"
#include "Text.h"


HttpResponse::HttpResponse(int stat, const char* data) :
    status(stat),
    content(data)
{
}


HttpResponse::HttpResponse(const char* r) :
    status(0),
    content(r)
{
    if (r && *r)
        ParseResponse(r);
}


HttpResponse::~HttpResponse()
{
    headers.destroy();
    cookies.destroy();
}


HttpResponse::operator Text()
{
    Text response;

    switch (status) {
    case SC_CONTINUE             : response = "HTTP/1.1 100 Continue\n";                break;
    case SC_SWITCHING_PROTOCOLS  : response = "HTTP/1.1 101 Switching Protocols\n";     break;

    case SC_OK                   : response = "HTTP/1.1 200 OK\n";                      break;
    case SC_CREATED              : response = "HTTP/1.1 201 Created\n";                 break;
    case SC_ACCEPTED             : response = "HTTP/1.1 202 Accepted\n";                break;
    case SC_NON_AUTHORITATIVE    : response = "HTTP/1.1 203 Non Authoritative\n";       break;
    case SC_NO_CONTENT           : response = "HTTP/1.1 204 No Content\n";              break;
    case SC_RESET_CONTENT        : response = "HTTP/1.1 205 Reset Content\n";           break;
    case SC_PARTIAL_CONTENT      : response = "HTTP/1.1 206 Partial Content\n";         break;

    case SC_MULTIPLE_CHOICES     : response = "HTTP/1.1 300 Multiple Choices\n";        break;
    case SC_MOVED_PERMANENTLY    : response = "HTTP/1.1 301 Moved Permanently\n";       break;
    case SC_FOUND                : response = "HTTP/1.1 302 Found\n";                   break;
    case SC_SEE_OTHER            : response = "HTTP/1.1 303 See Other\n";               break;
    case SC_NOT_MODIFIED         : response = "HTTP/1.1 304 Not Modified\n";            break;
    case SC_USE_PROXY            : response = "HTTP/1.1 305 Use Proxy\n";               break;
    case SC_TEMPORARY_REDIRECT   : response = "HTTP/1.1 307 Temporary Redirect\n";      break;

    case SC_BAD_REQUEST          : response = "HTTP/1.1 400 Bad Request\n";             break;
    case SC_UNAUTHORIZED         : response = "HTTP/1.1 401 Unauthorized\n";            break;
    case SC_PAYMENT_REQUIRED     : response = "HTTP/1.1 402 Payment Required\n";        break;
    case SC_FORBIDDEN            : response = "HTTP/1.1 403 Forbidden\n";               break;
    case SC_NOT_FOUND            : response = "HTTP/1.1 404 Not Found\n";               break;
    case SC_METHOD_NOT_ALLOWED   : response = "HTTP/1.1 405 Method Not Allowed\n";      break;
    case SC_NOT_ACCEPTABLE       : response = "HTTP/1.1 406 Not Acceptable\n";          break;
    case SC_PROXY_AUTH_REQ       : response = "HTTP/1.1 407 Proxy Authorization Req\n"; break;
    case SC_REQUEST_TIME_OUT     : response = "HTTP/1.1 408 Request Timeout\n";         break;
    case SC_CONFLICT             : response = "HTTP/1.1 409 Conflict\n";                break;
    case SC_GONE                 : response = "HTTP/1.1 410 Gone\n";                    break;
    case SC_LENGTH_REQUIRED      : response = "HTTP/1.1 411 Length Required\n";         break;

    default:
    case SC_SERVER_ERROR         : response = "HTTP/1.1 500 Internal Server Error\n";   break;
    case SC_NOT_IMPLEMENTED      : response = "HTTP/1.1 501 Not Implemented\n";         break;
    case SC_BAD_GATEWAY          : response = "HTTP/1.1 502 Bad Gateway\n";             break;
    case SC_SERVICE_UNAVAILABLE  : response = "HTTP/1.1 503 Service Unavailable\n";     break;
    case SC_GATEWAY_TIMEOUT      : response = "HTTP/1.1 504 Gateway Timeout\n";         break;
    case SC_VERSION_NOT_SUPPORTED: response = "HTTP/1.1 505 HTTP Version Not Supported\n"; break;
    }

    SetHeader("Connection", "close");

    char buffer[256];

    if (content.length()) {
        std::sprintf(buffer, "%d", content.length());
        SetHeader("Content-Length", buffer);
    }

    for (int i = 0; i < cookies.size(); i++) {
        HttpParam* cookie = cookies.at(i);
        std::sprintf(buffer, "%s=\"%s\"; Version=\"1\"", cookie->name.data(), cookie->value.data());

        AddHeader("Set-Cookie", buffer);
    }

    for (int i = 0; i < headers.size(); i++) {
        const HttpParam* p = headers.at(i);
        std::sprintf(buffer, "%s: %s\n", p->name.data(), p->value.data());
        response += buffer;
    }

    response += "\n";
    response += content;

    return response;
}


Text
HttpResponse::GetHeader(const char* name)
{
    ListIter<HttpParam> iter = headers;
    while (++iter) {
        HttpParam* p = iter.value();

        if (p->name == name)
            return p->value;
    }

    return Text();
}


void
HttpResponse::SetHeader(const char* name, const char* value)
{
    ListIter<HttpParam> iter = headers;
    while (++iter) {
        HttpParam* p = iter.value();

        if (p->name == name) {
            p->value = value;
            return;
        }
    }

    HttpParam* param = new HttpParam(name, value);
    if (param)
        headers.append(param);
}

void
HttpResponse::AddHeader(const char* name, const char* value)
{
    HttpParam* param = new HttpParam(name, value);
    if (param)
        headers.append(param);
}


Text
HttpResponse::GetCookie(const char* name)
{
    ListIter<HttpParam> iter = cookies;
    while (++iter) {
        HttpParam* p = iter.value();

        if (p->name == name)
            return p->value;
    }

    return Text();
}

void
HttpResponse::SetCookie(const char* name, const char* value)
{
    ListIter<HttpParam> iter = cookies;
    while (++iter) {
        HttpParam* p = iter.value();

        if (p->name == name) {
            p->value = value;
            return;
        }
    }

    HttpParam* param = new HttpParam(name, value);
    if (param)
        cookies.append(param);
}

void
HttpResponse::AddCookie(const char* name, const char* value)
{
    HttpParam* param = new HttpParam(name, value);
    if (param)
        cookies.append(param);
}


void
HttpResponse::SendRedirect(const char* url)
{
    status = SC_TEMPORARY_REDIRECT;
    SetHeader("Location", url);
}


void
HttpResponse::ParseResponse(Text response)
{
    if (response.length() <= 12 || response.indexOf("HTTP/1.") != 0)
        return;

    const char* pStatus = response.data() + 9;

    std::sscanf(pStatus, "%d", &status);
    if (!status) return;

    int  i = 0;

    // get the headers:
    const char* p = response.data();
    while (*p && *p != '\n')
        p++;

    if (*p == '\n') p++;

    while (*p && *p != '\r' && *p != '\n') {
        char name_buf[1024];
        char value_buf[1024];

        i = 0;
        while (*p && *p != ':')
            name_buf[i++] = *p++;
        name_buf[i] = 0;

        p++;                       // skip ':'
        while (std::isspace(*p)) p++;   // skip spaces

        i = 0;
        while (*p && *p != '\r' && *p != '\n') // read to end of header line
            value_buf[i++] = *p++;
        value_buf[i] = 0;

#ifdef _WIN32
        if (!_stricmp(name_buf, "Set-Cookie")) {
#else
        if (!strcasecmp(name_buf, "Set-Cookie")) {
#endif
            ParseCookie(value_buf);
        }
        else {
            HttpParam* param = new HttpParam(name_buf, value_buf);
            if (param)
                headers.append(param);
        }

        while (*p && *p != '\n')
            p++;

        if (*p == '\n') p++;
    }

    if (*p == '\n') p++;
    content = p;
}


void
HttpResponse::ParseCookie(const char* param)
{
    const char* p = param;

    while (p && *p) {
        while (std::isspace(*p)) p++;

        // just ignore reserved attributes
        if (*p == '$') {
            while (*p && !std::isspace(*p) && *p != ';') p++;

            if (*p == ';')
                p++;
        }

        // found a cookie!
        else if (std::isalpha(*p)) {
            char name[1024];
            char data[1024];

            char* d = name;
            while (*p && *p != '=')
                *d++ = *p++;
            *d = 0;

            if (*p == '=')
                p++;

            if (*p == '"')
                p++;

            d = data;
            while (*p && *p != '"' && *p != ';')
                *d++ = *p++;
            *d = 0;

            if (*p == '"')
                p++;

            if (*p == ';')
                p++;

            // ignore the version attribute
#ifdef _WIN32
            if (_stricmp(name, "version")) {
#else
            if (strcasecmp(name, "version")) {
#endif
                HttpParam* param = new HttpParam(name, data);
                if (param)
                    cookies.append(param);
            }
        }

        // this shouldn't happen - abandon the parse
        else {
            return;
        }
    }
}
