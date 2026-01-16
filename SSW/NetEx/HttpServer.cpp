/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Network Server Pump for HTTP Server
*/

#include "HttpServer.h"

#include <cstdint>
#include <cstdio>

#include "HttpParam.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "List.h"
#include "NetAddr.h"
#include "NetServer.h"
#include "Text.h"


HttpServer::HttpServer(std::uint16_t port, int poolsize) :
    NetServer(port, poolsize)
{
    http_server_name = "Generic HttpServer 1.0";
}


HttpServer::~HttpServer()
{
}


Text
HttpServer::ProcessRequest(Text msg, const NetAddr& addr)
{
    HttpRequest    request(msg);
    HttpResponse   response;

    request.SetClientAddr(addr);

    switch (request.Method()) {
    case HttpRequest::HTTP_GET:
        if (DoGet(request, response))
            return response;

    case HttpRequest::HTTP_POST:
        if (DoPost(request, response))
            return response;

    case HttpRequest::HTTP_HEAD:
        if (DoHead(request, response))
            return response;
    }

    return ErrorResponse();
}


Text
HttpServer::GetServerName()
{
    return http_server_name;
}


void
HttpServer::SetServerName(const char* name)
{
    http_server_name = name;
}


Text
HttpServer::DefaultResponse()
{
    Text response = "HTTP/1.1 200 OK\nServer: ";
    response += http_server_name;
    response += "\nMIME-Version: 1.0\nContent-Type: text/html\nConnection: close\n\n";

    return response;
}


Text
HttpServer::ErrorResponse()
{
    Text response = "HTTP/1.1 500 Internal Server Error\nServer:";
    response += http_server_name;
    response += "\nMIME-Version: 1.0\nContent-Type: text/html\nConnection: close\n\n";

    response += "<html><head><title>";
    response += http_server_name;
    response += " Error</title></head>\n";
    response += "<body bgcolor=\"black\" text=\"white\">\n<h1>";
    response += http_server_name;
    response += "</h1>\n<p>Veruca... sweetheart... angel...  I'm not a magician!\n";
    response += "</body></html>\n\n";

    return response;
}


bool
HttpServer::DoGet(HttpRequest& request, HttpResponse& response)
{
    char buffer[1024];
    Text content;

    content  = "<html><head><title>";
    content += http_server_name;
    content += "</title></head>\n";
    content += "<body bgcolor=\"white\" text=\"black\">\n<h1>";
    content += http_server_name;
    content += "</h1>\n";
    content += "<br><h3>Client Address:</h3><p>\n";

    std::sprintf(buffer, "%d.%d.%d.%d:%d<br><br>\n",
                 client_addr.B1(),
                 client_addr.B2(),
                 client_addr.B3(),
                 client_addr.B4(),
                 client_addr.Port());

    content += buffer;
    content += "<h3>Request Method:</h3><p>\n";

    switch (request.Method()) {
    case HttpRequest::HTTP_GET:
        content += "GET";
        break;

    case HttpRequest::HTTP_POST:
        content += "POST";
        break;

    case HttpRequest::HTTP_HEAD:
        content += "HEAD";
        break;

    default:
        content += "(unsupported?)";
        break;
    }

    content += "<br>\n";
    content += "<br><h3>URI Requested:</h3><p>\n";
    content += request.URI();
    content += "<br>\n";

    if (request.GetQuery().size() > 0) {
        content += "<br><h3>Query Parameters:</h3>\n";

        ListIter<HttpParam> q_iter = request.GetQuery();
        while (++q_iter) {
            HttpParam* q = q_iter.value();
            std::printf(buffer, "<b>%s:</b> <i>%s</i><br>\n", q->name.data(), q->value.data());
            content += buffer;
        }
    }

    content += "<br><h3>Request Headers:</h3>\n";
    ListIter<HttpParam> h_iter = request.GetHeaders();
    while (++h_iter) {
        HttpParam* h = h_iter.value();
        std::sprintf(buffer, "<b>%s:</b> <i>%s</i><br>\n", h->name.data(), h->value.data());
        content += buffer;
    }

    content += "</body></html>\n\n";

    response.SetStatus(HttpResponse::SC_OK);
    response.AddHeader("Server",        http_server_name);
    response.AddHeader("MIME-Version",  "1.0");
    response.AddHeader("Content-Type",  "text/html");
    response.SetContent(content);

    return true;
}


bool
HttpServer::DoPost(HttpRequest& request, HttpResponse& response)
{
    return DoGet(request, response);
}


bool
HttpServer::DoHead(HttpRequest& request, HttpResponse& response)
{
    if (DoGet(request, response)) {
        int len = response.Content().length();

        char buffer[256];
        std::sprintf(buffer, "%d", len);
        response.SetHeader("Content-Length", buffer);
        response.SetContent("");

        return true;
    }

    return false;
}
