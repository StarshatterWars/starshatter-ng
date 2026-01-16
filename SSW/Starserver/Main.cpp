/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include <InfoEx.h>
#include <NetLayer.h>
#include <Panic.h>
#include <starshatter/definition.h>
#include <Utils.h>
#include <VersionInfo.h>

#include "StarServer.h"


int
main(int, char*[])
{
    AssignErrLog(fopen("serverlog.txt", "wb"));
    Print("Starserver %s\n", versionInfo);
    Print("%s\n\n", InfoEx::LongDescription().data());
    int result = 1;
    try {
        NetLayer net;
        StarServer server;
        if (server.Init())
            result = server.Run();
    }
    catch (const char* err) {
        Print("Exception: %s\n", err);
    }
    Token::close();
    if (Panic::Panicked()) {
        Print("PANIC: %s\n", Panic::Message());
        result = 1;
    }
    CloseErrLog();
    return result;
}
