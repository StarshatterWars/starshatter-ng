/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Main Menu Dialog Active Window class
*/

#ifndef NetClientDlg_h
#define NetClientDlg_h

#include "Types.h"
#include "NetClientConfig.h"
#include "NetLobby.h"

#include "FormWindow.h"
#include "Bitmap.h"
#include "Button.h"
#include "ComboBox.h"
#include "ListBox.h"
#include "Font.h"

// +--------------------------------------------------------------------+

class MenuScreen;

// +--------------------------------------------------------------------+

class NetClientDlg : public FormWindow
{
public:
    NetClientDlg(Screen* s, FormDef& def, MenuScreen* mgr);
    virtual ~NetClientDlg();

    virtual void      RegisterControls();
    virtual void      Show();
    virtual void      ExecFrame();

    // Operations:
    virtual void      OnSelect(AWEvent* event);
    virtual void      OnAdd(AWEvent* event);
    virtual void      OnEdit(AWEvent* event);
    virtual void      OnDel(AWEvent* event);
    virtual void      OnServer(AWEvent* event);
    virtual void      OnHost(AWEvent* event);
    virtual void      OnJoin(AWEvent* event);
    virtual void      OnCancel(AWEvent* event);

    virtual void      ShowServers();
    virtual void      UpdateServers();
    virtual void      PingServer(int n);
    virtual bool      PingComplete();
    virtual void      StopNetProc();

protected:
    MenuScreen*       manager;
    NetClientConfig*  config;

    Button*           btn_add;
    Button*           btn_edit;
    Button*           btn_del;
    ListBox*          lst_servers;
    ActiveWindow*     lbl_info;
    int               server_index;
    int               ping_index;
    HANDLE            hnet;
    NetServerInfo     ping_info;

    Button*           btn_server;
    Button*           btn_host;
    Button*           btn_join;
    Button*           btn_cancel;
};

#endif  // NetClientDlg_h

