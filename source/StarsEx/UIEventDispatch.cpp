/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo

*/

#include "UIEventDispatch.h"

#include "Mouse.h"
#include "Keyboard.h"

// +--------------------------------------------------------------------+

int GetKeyPlus(int& key, int& shift);

// +--------------------------------------------------------------------+

UIEventDispatch* UIEventDispatch::dispatcher = 0;

// +--------------------------------------------------------------------+

UIEventDispatch::UIEventDispatch()
    : capture(0), current(0), focus(0), click_tgt(0),
      mouse_x(0), mouse_y(0), mouse_l(0), mouse_r(0)
{ }

UIEventDispatch::~UIEventDispatch()
{ }

// +--------------------------------------------------------------------+

void
UIEventDispatch::Create()
{
    dispatcher = new UIEventDispatch;
}

// +--------------------------------------------------------------------+

void
UIEventDispatch::Close()
{
    delete dispatcher;
    dispatcher = 0;
}

// +--------------------------------------------------------------------+

void
UIEventDispatch::Dispatch()
{
    int ml = Mouse::LButton();
    int mr = Mouse::RButton();
    int mx = Mouse::X();
    int my = Mouse::Y();
    int mw = Mouse::Wheel();

    UIEventTarget* mouse_tgt = capture;
    UIEventTarget* key_tgt   = focus;
    UIEventTarget* do_click  = 0;

    if (!mouse_tgt) {
        ListIter<UIEventTarget> iter = clients;
        while (++iter) {
            UIEventTarget* test = iter.value();
            if (test->IsFormActive()) {
                if (test->TargetRect().Contains(mx,my))
                mouse_tgt = test;

                if (test->HasFocus())
                key_tgt = test;
            }
        }
    }

    // Mouse Events:

    if (mouse_tgt != current) {
        if (current && current->IsEnabled() && current->IsVisible())
        current->OnMouseExit(mx,my);

        current = mouse_tgt;

        if (current && current->IsEnabled() && current->IsVisible())
        current->OnMouseEnter(mx,my);
    }

    if (mouse_tgt && mouse_tgt->IsEnabled()) {
        if (mx != mouse_x || my != mouse_y)
        mouse_tgt->OnMouseMove(mx,my);

        if (mw != 0)
        mouse_tgt->OnMouseWheel(mw);

        if (ml != mouse_l) {
            if (ml) {
                mouse_tgt->OnLButtonDown(mx,my);
                click_tgt = mouse_tgt;
            }
            else {
                mouse_tgt->OnLButtonUp(mx,my);

                if (click_tgt == mouse_tgt) {
                    if (click_tgt->TargetRect().Contains(mx,my))
                    do_click = click_tgt;
                    click_tgt = 0;
                }
            }
        }

        if (mr != mouse_r) {
            if (mr)
            mouse_tgt->OnRButtonDown(mx,my);
            else
            mouse_tgt->OnRButtonUp(mx,my);
        }
    }

    mouse_l = ml;
    mouse_r = mr;
    mouse_x = mx;
    mouse_y = my;

    // Keyboard Events:

    if (click_tgt && click_tgt != key_tgt) {
        if (key_tgt) key_tgt->KillFocus();
        key_tgt = click_tgt;

        if (key_tgt != focus) {
            if (focus) focus->KillFocus();

            if (key_tgt && key_tgt->IsEnabled() && key_tgt->IsVisible())
            focus = key_tgt;
            else
            key_tgt = 0;

            if (focus) focus->SetFocus();
        }
    }

    if (key_tgt && key_tgt->IsEnabled()) {
        int key = 0;
        int shift = 0;

        while (GetKeyPlus(key, shift)) {
            if (key == VK_ESCAPE) {
                key_tgt->KillFocus();
                focus = 0;
                break;
            }

            else if (key == VK_TAB && focus) {
                int key_index = clients.index(focus) + 1;

                if (shift & 1) key_index -= 2;

                if (key_index >= clients.size())
                key_index = 0;
                else if (key_index < 0)
                key_index = clients.size()-1;

                if (focus) focus->KillFocus();
                focus = clients[key_index];
                if (focus) focus->SetFocus();

                break;
            }

            key_tgt->OnKeyDown(key, shift);
        }
    }

    if (do_click)
    do_click->OnClick();
}

// +--------------------------------------------------------------------+

void
UIEventDispatch::MouseEnter(UIEventTarget* mouse_tgt)
{
    if (mouse_tgt != current) {
        if (current) current->OnMouseExit(0,0);
        current = mouse_tgt;
        if (current) current->OnMouseEnter(0,0);
    }
}

// +--------------------------------------------------------------------+

void
UIEventDispatch::Register(UIEventTarget* tgt)
{
    if (!clients.contains(tgt))
    clients.append(tgt);
}

// +--------------------------------------------------------------------+

void
UIEventDispatch::Unregister(UIEventTarget* tgt)
{
    clients.remove(tgt);

    if (capture   == tgt)   capture   = 0;
    if (current   == tgt)   current   = 0;
    if (focus     == tgt)   focus     = 0;
    if (click_tgt == tgt)   click_tgt = 0;
}

// +--------------------------------------------------------------------+

UIEventTarget*
UIEventDispatch::GetCapture()
{
    return capture;
}

int
UIEventDispatch::CaptureMouse(UIEventTarget* tgt)
{
    if (tgt) {
        capture = tgt;
        return 1;
    }

    return 0;
}

int
UIEventDispatch::ReleaseMouse(UIEventTarget* tgt)
{
    if (capture == tgt) {
        capture = 0;
        return 1;
    }

    return 0;
}

// +--------------------------------------------------------------------+

UIEventTarget*
UIEventDispatch::GetFocus()
{
    return focus;
}

void
UIEventDispatch::SetFocus(UIEventTarget* tgt)
{
    if (focus != tgt) {
        if (focus)
        focus->KillFocus();

        focus = tgt;

        if (focus && focus->IsEnabled() && focus->IsVisible())
        focus->SetFocus();
    }
}

void
UIEventDispatch::KillFocus(UIEventTarget* tgt)
{
    if (focus && focus == tgt) {
        focus = 0;
        tgt->KillFocus();
    }
}
