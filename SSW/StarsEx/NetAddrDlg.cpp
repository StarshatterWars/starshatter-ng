/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Mission Select Dialog Active Window class
*/

#include "NetAddrDlg.h"

#include <Text.h>

#include "Button.h"
#include "EditBox.h"
#include "FormatUtil.h"
#include "Keyboard.h"
#include "ListBox.h"
#include "MenuScreen.h"
#include "Mouse.h"
#include "NetClientConfig.h"
#include "NetLobby.h"
#include "ParseUtil.h"
#include "Slider.h"

// +--------------------------------------------------------------------+
// DECLARE MAPPING FUNCTIONS:

DEF_MAP_CLIENT(NetAddrDlg, OnSave);
DEF_MAP_CLIENT(NetAddrDlg, OnCancel);

// +--------------------------------------------------------------------+

NetAddrDlg::NetAddrDlg(Screen* s, FormDef& def, MenuScreen* mgr) :
    FormWindow(s, 0, 0, s->Width(), s->Height()),
    manager {mgr},
    btn_save {nullptr},
    btn_cancel {nullptr},
    edt_name {nullptr},
    edt_addr {nullptr},
    edt_port {nullptr},
    edt_pass {nullptr},
    selected_config {NEW}
{
    Init(def);
}

NetAddrDlg::~NetAddrDlg()
{
}

// +--------------------------------------------------------------------+

void
NetAddrDlg::RegisterControls()
{
    btn_save    = (Button*) FindControl(1);
    btn_cancel  = (Button*) FindControl(2);

    REGISTER_CLIENT(EID_CLICK, btn_save,   NetAddrDlg, OnSave);
    REGISTER_CLIENT(EID_CLICK, btn_cancel, NetAddrDlg, OnCancel);

    edt_name    = (EditBox*) FindControl(200);
    edt_addr    = (EditBox*) FindControl(201);
    edt_port    = (EditBox*) FindControl(202);
    edt_pass    = (EditBox*) FindControl(203);

    if (edt_name) edt_name->SetText("");
    if (edt_addr) edt_addr->SetText("");
    if (edt_port) edt_port->SetText("");
    if (edt_pass) edt_pass->SetText("");
}

// +--------------------------------------------------------------------+

void
NetAddrDlg::Show()
{
    auto* config = NetClientConfig::GetInstance();
    if (selected_config != NEW && config != nullptr) {
        auto* info = config->GetServerInfo(selected_config);
        if (edt_name)  edt_name->SetText(info->name);
        if (edt_addr)  edt_addr->SetText(info->hostname);
        if (edt_port)  edt_port->SetText(Text::format("%d", info->port));
        if (edt_pass)  edt_pass->SetText(info->password);
    }
    else {
        if (edt_name)  edt_name->SetText("");
        if (edt_addr)  edt_addr->SetText("");
        if (edt_port)  edt_port->SetText("");
        if (edt_pass)  edt_pass->SetText("");
    }
    if (!IsShown()) {
        FormWindow::Show();
        if (edt_name)  edt_name->SetFocus();
    }
}

// +--------------------------------------------------------------------+

static bool tab_latch = false;

void
NetAddrDlg::ExecFrame()
{
    if (Keyboard::KeyDown(VK_RETURN)) {
        OnSave(0);
    }
}

// +--------------------------------------------------------------------+

void
NetAddrDlg::OnSave(AWEvent* event)
{
    NetClientConfig* config = NetClientConfig::GetInstance();

    if (config && edt_addr && edt_addr->GetText().length() > 0 && edt_port && edt_port->GetText().length() > 0) {
        Text name;
        Text addr;
        Text pass;
        int  port;

        sscanf_s(edt_port->GetText().data(), "%d", &port);

        if (edt_name && edt_name->GetText().length() < 250) {
            char buffer[256];
            strcpy_s(buffer, edt_name->GetText().data());
            char* p = strpbrk(buffer, "\n\r\t");
            if (p) *p = 0;

            name = SafeQuotes(buffer);
        }

        if (edt_pass && edt_pass->GetText().length() < 250) {
            char buffer[256];
            strcpy_s(buffer, edt_pass->GetText().data());
            char* p = strpbrk(buffer, "\n\r\t");
            if (p) *p = 0;

            pass = SafeQuotes(buffer);
        }

        if (edt_addr && edt_addr->GetText().length() < 250) {
            char buffer[256];
            strcpy_s(buffer, edt_addr->GetText().data());
            char* p = strpbrk(buffer, "\n\r\t");
            if (p) *p = 0;

            addr = SafeQuotes(buffer);
        }

        if (selected_config == NEW)
            config->AddServer(name, addr, port, pass, true);
        else
            config->UpdateServer(selected_config, name, addr, port, pass);
        config->Save();
    }

    if (manager)
    manager->ShowNetClientDlg();
}

void
NetAddrDlg::OnCancel(AWEvent* event)
{
    if (manager)
    manager->ShowNetClientDlg();
}


void
NetAddrDlg::SelectConfig(int index)
{
    if (auto* config = NetClientConfig::GetInstance()) {
        const auto list = config->GetServerList();
        const bool exists = (0 <= index && index < list.size());
        selected_config = exists ? index : NEW;
    }
}
