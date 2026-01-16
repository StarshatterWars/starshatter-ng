/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "HttpSession.h"

#include <stdio.h>
#include <stdlib.h>

#include "HttpParam.h"
#include "List.h"
#include "NetLayer.h"
#include "Text.h"


HttpSession::HttpSession()
{
    id          = GenerateUniqueID();
    access_time = NetLayer::GetUTC();
}


HttpSession::~HttpSession()
{
    attributes.destroy();
}


Text
HttpSession::GetAttribute(const char* name)
{
    ListIter<HttpParam> iter = attributes;
    while (++iter) {
        HttpParam* p = iter.value();

        if (p->name == name)
            return p->value;
    }

    return Text();
}


void
HttpSession::SetAttribute(const char* name, const char* value)
{
    ListIter<HttpParam> iter = attributes;
    while (++iter) {
        HttpParam* p = iter.value();

        if (p->name == name) {
            p->value = value;
            return;
        }
    }

    HttpParam* param = new HttpParam(name, value);
    if (param)
        attributes.append(param);
}


void
HttpSession::DelAttribute(const char* name)
{
    ListIter<HttpParam> iter = attributes;
    while (++iter) {
        HttpParam* p = iter.value();

        if (p->name == name) {
            delete iter.removeItem();
            return;
        }
    }
}


int
HttpSession::GetIntAttribute(const char* name)
{
    ListIter<HttpParam> iter = attributes;
    while (++iter) {
        HttpParam* p = iter.value();

        if (p->name == name) {
            int result = ::atoi(p->value.data());
            return result;
        }
    }

    return 0;
}


void
HttpSession::SetIntAttribute(const char* name, int value)
{
    char buf[32];
    sprintf(buf, "%d", value);

    ListIter<HttpParam> iter = attributes;
    while (++iter) {
        HttpParam* p = iter.value();

        if (p->name == name) {
            p->value = buf;
            return;
        }
    }

    HttpParam* param = new HttpParam(name, buf);
    if (param)
        attributes.append(param);
}


void
HttpSession::DelIntAttribute(const char* name)
{
    DelAttribute(name);
}


Text
HttpSession::GenerateUniqueID()
{
    char unique[17];

    for (int i = 0; i < 16; i++) {
        char c = rand() % 25 + 'a';
        unique[i] = c;
    }

    unique[16] = 0;
    return unique;
}


void
HttpSession::Access()
{
    access_time = NetLayer::GetUTC();
}
