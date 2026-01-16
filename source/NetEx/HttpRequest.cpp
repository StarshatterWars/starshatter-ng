/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "HttpRequest.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <strings.h>
#endif

#include "HttpParam.h"
#include "List.h"
#include "Text.h"


HttpRequest::HttpRequest(const char* r) :
    method(0)
{
    if (r && *r)
        ParseRequest(r);
}


HttpRequest::~HttpRequest()
{
    query.destroy();
    headers.destroy();
    cookies.destroy();
}


void
HttpRequest::ParseRequest(Text request)
{
    if (request.length() <= 8)
        return;

    const char* pReq    = 0;
    const char* pURI    = 0;
    const char* pQuery  = 0;

    switch (request[0]) {
    case 'G':
        if (request.indexOf("GET") == 0)
            method = HTTP_GET;
        break;

    case 'P':
        if (request.indexOf("POST") == 0)
            method = HTTP_POST;
        break;

    case 'H':
        if (request.indexOf("HEAD") == 0)
            method = HTTP_HEAD;
        break;

    default:
        break;
    }

    if (!method) return;

    char buffer[1024];
    int  i = 0;

    // save the request line:
    pReq = request.data();
    while (*pReq && *pReq != '\n')
        buffer[i++] = *pReq++;
    buffer[i] = 0;

    request_line = buffer;
    i = 0;

    // find the URI:
    pURI = request.data();
    while (*pURI && !isspace(*pURI))
        pURI++;

    while (*pURI && isspace(*pURI))
        pURI++;

    // copy the URI and find the query string:
    while (*pURI && *pURI != '?' && !isspace(*pURI)) {
        buffer[i++] = *pURI++;
    }

    buffer[i] = 0;
    uri = buffer;
    pQuery = pURI;

    // parse the query string:
    if (*pQuery == '?') {
        pQuery++;

        while (*pQuery && !isspace(*pQuery)) {
            char name_buf[1024];
            char value_buf[1024];

            i = 0;
            while (*pQuery && *pQuery != '=' && !isspace(*pQuery))
                name_buf[i++] = *pQuery++;
            name_buf[i] = 0;

            if (*pQuery == '=')
                pQuery++;

            i = 0;
            while (*pQuery && *pQuery != '&' && !isspace(*pQuery))
                value_buf[i++] = *pQuery++;
            value_buf[i] = 0;

            if (*pQuery == '&')
                pQuery++;

            HttpParam* param = new HttpParam(name_buf, DecodeParam(value_buf));
            if (param)
                query.append(param);
        }
    }

    // get the headers:
    const char* p = request.data();
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
        while (isspace(*p)) p++;   // skip spaces

        i = 0;
        while (*p && *p != '\r' && *p != '\n') // read to end of header line
            value_buf[i++] = *p++;
        value_buf[i] = 0;

#ifdef _WIN32
        if (!_stricmp(name_buf, "Cookie")) {
#else
        if (!strcasecmp(name_buf, "Cookie")) {
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

    if (method == HTTP_POST && *p) {
        while (*p == '\n' || *p == '\r')
            p++;

        content = *p;
        pQuery  = p;

        while (*pQuery && !isspace(*pQuery)) {
            char name_buf[1024];
            char value_buf[1024];

            i = 0;
            while (*pQuery && *pQuery != '=' && !isspace(*pQuery))
                name_buf[i++] = *pQuery++;
            name_buf[i] = 0;

            if (*pQuery == '=')
                pQuery++;

            i = 0;
            while (*pQuery && *pQuery != '&' && !isspace(*pQuery))
                value_buf[i++] = *pQuery++;
            value_buf[i] = 0;

            if (*pQuery == '&')
                pQuery++;

            HttpParam* param = new HttpParam(name_buf, DecodeParam(value_buf));
            if (param)
                query.append(param);
        }
    }
}


void
HttpRequest::ParseCookie(const char* param)
{
    const char* p = param;

    while (p && *p) {
        while (isspace(*p)) p++;

        // just ignore reserved attributes
        if (*p == '$') {
            while (*p && !isspace(*p) && *p != ';') p++;

            if (*p == ';')
                p++;
        }

        // found a cookie!
        else if (isalpha(*p)) {
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

            HttpParam* param = new HttpParam(name, data);
            if (param)
                cookies.append(param);
        }

        // this shouldn't happen - abandon the parse
        else {
            return;
        }
    }
}


Text
HttpRequest::GetParam(const char* name)
{
    ListIter<HttpParam> iter = query;
    while (++iter) {
        HttpParam* p = iter.value();

        if (p->name == name)
            return p->value;
    }

    return Text();
}


Text
HttpRequest::GetHeader(const char* name)
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
HttpRequest::SetHeader(const char* name, const char* value)
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
HttpRequest::AddHeader(const char* name, const char* value)
{
    HttpParam* param = new HttpParam(name, value);
    if (param)
        headers.append(param);
}


Text
HttpRequest::GetCookie(const char* name)
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
HttpRequest::SetCookie(const char* name, const char* value)
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
HttpRequest::AddCookie(const char* name, const char* value)
{
    HttpParam* param = new HttpParam(name, value);
    if (param)
        cookies.append(param);
}


Text
HttpRequest::DecodeParam(const char* value)
{
    if (!value || !*value) return "";

    int   size   = strlen(value);
    char  val    = 0;
    char  code[4];
    char  sbuf[256];
    char* lbuf = 0;

    char* dst    = sbuf;
    char* p      = sbuf;

    if (size > 255) {
        lbuf   = new char[size+1];
        dst    = lbuf;
        p      = lbuf;
    }

    if (p) {
        while (*value) {
            switch (*value) {
            default:    *p++ = *value; break;
            case '+':   *p++ = ' ';    break;

            case '%':
                value++;
                code[0] = *value++;
                code[1] = *value;
                code[2] = 0;

                val = (char) strtol(code, 0, 16);
                *p++ = val;
                break;
            }

            value++;
        }

        *p = 0;
    }

    Text  result = dst;

    if (lbuf)
        delete [] lbuf;

    return result;
}


Text
HttpRequest::EncodeParam(const char* value)
{
    if (!value || !*value) return "";

    int   size   = strlen(value);
    char  hex1   = 0;
    char  hex2   = 0;

    char  sbuf[1024];
    char* lbuf = 0;

    char* dst    = sbuf;
    char* p      = sbuf;

    if (size > 255) {
        lbuf   = new char[4*size+1];
        dst    = lbuf;
        p      = lbuf;
    }

    if (p) {
        while (*value) {
            switch (*value) {
            default:    *p++ = *value; break;
            case ' ':   *p++ = '+';    break;

            case '?':   *p++ = '%'; *p++ = '3'; *p++ = 'F'; break;
            case '&':   *p++ = '%'; *p++ = '2'; *p++ = '6'; break;
            case ':':   *p++ = '%'; *p++ = '3'; *p++ = 'A'; break;
            case '/':   *p++ = '%'; *p++ = '2'; *p++ = 'F'; break;
            case '\\':  *p++ = '%'; *p++ = '5'; *p++ = 'C'; break;
            case '%':   *p++ = '%'; *p++ = '2'; *p++ = '5'; break;
            case '|':   *p++ = '%'; *p++ = '7'; *p++ = 'C'; break;
            case '<':   *p++ = '%'; *p++ = '3'; *p++ = 'C'; break;
            case '>':   *p++ = '%'; *p++ = '3'; *p++ = 'E'; break;
            case '[':   *p++ = '%'; *p++ = '5'; *p++ = 'B'; break;
            case ']':   *p++ = '%'; *p++ = '5'; *p++ = 'D'; break;
            case '{':   *p++ = '%'; *p++ = '7'; *p++ = 'B'; break;
            case '}':   *p++ = '%'; *p++ = '7'; *p++ = 'D'; break;
            case '"':   *p++ = '%'; *p++ = '2'; *p++ = '2'; break;
            case '^':   *p++ = '%'; *p++ = '5'; *p++ = 'E'; break;
            case '`':   *p++ = '%'; *p++ = '6'; *p++ = '0'; break;
            case '\n':  break;
            case '\r':  break;
            case '\t':  break;
            }

            value++;
        }

        *p = 0;
    }

    Text  result = dst;

    if (lbuf)
        delete [] lbuf;

    return result;
}


HttpRequest::operator Text()
{
    Text response = request_line.data();
    response += "\n";

    for (int i = 0; i < headers.size(); i++) {
        HttpParam* h = headers[i];
        response += h->name;
        response += ": ";
        response += h->value;
        response += "\n";
    }

    for (int i = 0; i < cookies.size(); i++) {
        HttpParam* c = cookies[i];
        response += "Cookie: ";
        response += c->name;
        response += "=\"";
        response += c->value;
        response += "\"\n";
    }

    response += "Connection: close\n\n";
    response += content;

    return response;
}
