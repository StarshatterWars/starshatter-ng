/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo
*/

#include <InfoEx.h>

#include "Starshatter.h"
#include "HUDView.h"
#include "Utils.h"

#include "NetHost.h"
#include "NetAddr.h"
#include "NetLayer.h"
#include "NetBrokerClient.h"
#include "NetClient.h"
#include "HttpClient.h"

#include "Color.h"
#include "DataLoader.h"
#include "DumpMissions.h"
#include "Pcx.h"
#include "Encrypt.h"
#include "FormatUtil.h"
#include "Panic.h"
#include "ParseUtil.h"
#include "Random.h"
#include "VersionInfo.h"


extern int VD3D_describe_things;


static void
PrintLogHeader()
{
    Text sTime = FormatTimeString();
    Print("+====================================================================+\n");
    Print("| STARSHATTER %-25s%29s |\n", versionInfo, sTime.data());
    Print("+====================================================================+\n\n");
    Print("%s\n\n", InfoEx::LongDescription().data());
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    int   result    = 0;
    int   test_mode = 0;

    if (strstr(lpCmdLine, "-server"))
        AssignErrLog(fopen("serverlog.txt", "wb"));
    else
        AssignErrLog(fopen("errlog.txt", "wb"));

    PrintLogHeader();

    if (strstr(lpCmdLine, "-test")) {
        Print("  Request TEST mode\n");
        test_mode = 1;
    }

    if (strstr(lpCmdLine, "-fps")) {
        HUDView::ShowFPS(true);
    }

    if (strstr(lpCmdLine, "-dump")) {
        Print("  Request dump dynamic missions\n");
        dump_missions = 1;
    }

    if (strstr(lpCmdLine, "-lan")) {
        Print("  Request LAN ONLY mode\n");
        NetBrokerClient::Disable();
    }

    char* d3dinfo = strstr(lpCmdLine, "-d3d");
    if (d3dinfo) {
        int n = d3dinfo[4] - '0';

        if (n >= 0 && n <= 5)
        VD3D_describe_things = n;

        Print("  D3D Info Level: %d\n", VD3D_describe_things);
    }
    else {
        VD3D_describe_things = 0;
    }

    try {
        NetLayer net;

        Starshatter* stars = 0;

        stars = new Starshatter;
        stars->SetTestMode(test_mode);

        if (stars->Init(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
        result = stars->Run();

        Print("\n+====================================================================+\n");
        Print("  Begin Shutdown...\n");

        delete stars;

        Token::close();

        if (Panic::Panicked())
        MessageBox(0, Panic::Message(), "Starshatter - Error", MB_OK);
    }

    catch (const char* msg) {
        Print("  FATAL EXCEPTION: '%s'\n", msg);
    }
    /* } */

    Print("+====================================================================+\n");
    Print(" END OF LINE.\n");

    CloseErrLog();

    return result;
}
