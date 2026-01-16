/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "WndProc.h"

#include "GameWinDX9.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Types.h"


#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x20A
#endif


static struct {
	bool maximized = false;
	bool minimized = false;
} window_state;


LRESULT CALLBACK
WndProc(HWND hwnd, UINT message, WPARAM uParam, LPARAM lParam)
{
    auto game = GameWinDX9::GetInstance();
    switch (message) {
    case WM_SYSKEYDOWN:
        if (uParam == VK_TAB || uParam == VK_F4)
            return DefWindowProc(hwnd, message, uParam, lParam);
        return 0;

    case WM_MENUCHAR:
        return MNC_CLOSE << 16;

    case WM_ACTIVATEAPP:
        // Keep track of whether or not the app is in the foreground
        if (game)
            game->Activate(uParam?true:false);
        break;

    case WM_PAINT:
        return DefWindowProc(hwnd, message, uParam, lParam);

    case WM_SETCURSOR:
        // hide the windows mouse cursor
        SetCursor(NULL);
        return 1;

    case WM_ENTERSIZEMOVE:
        // Halt frame movement while the app is sizing or moving
        if (game)
            game->Pause(true);
        break;

    case WM_SIZE:
        // Pick up possible changes to window style due to maximize, etc.
        if (game && game->hwnd != NULL ) {
            game->window_style = GetWindowLong(game->hwnd, GWL_STYLE );
            if (uParam == SIZE_MINIMIZED) {
                game->Pause(true);
                window_state.minimized = true;
                window_state.maximized = false;
            }
            else if (uParam == SIZE_MAXIMIZED) {
                if (window_state.minimized)
                    game->Pause(false);
                window_state.minimized = false;
                window_state.maximized = true;
                game->ResizeVideo();
            }

            else if (uParam == SIZE_RESTORED) {
                if (window_state.maximized) {
                    window_state.maximized = false;
                    game->ResizeVideo();
                }
                else if (window_state.minimized) {
                    game->Pause(false);
                    window_state.minimized = false;
                    game->ResizeVideo();
                }
                else {
                    // If we're neither maximized nor minimized, the window size
                    // is changing by the user dragging the window edges.  In this
                    // case, we don't reset the device yet -- we wait until the
                    // user stops dragging, and a WM_EXITSIZEMOVE message comes.
                }
            }
        }
        break;

    case WM_EXITSIZEMOVE:
        if (game) {
            game->Pause(false);
            game->ResizeVideo();
        }
        break;

    case WM_ENTERMENULOOP:
        if (game)
        game->Pause(true);
        break;

    case WM_EXITMENULOOP:
        if (game)
        game->Pause(false);
        break;

    case WM_KEYDOWN:
        BufferKey(uParam);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_MOUSEMOVE:
        Mouse::x = LOWORD(lParam);
        Mouse::y = HIWORD(lParam);
        break;

    case WM_LBUTTONDOWN:
        Mouse::l = 1;
        break;

    case WM_LBUTTONDBLCLK:
        Mouse::l = 2;
        break;

    case WM_LBUTTONUP:
        Mouse::l = 0;
        break;

    case WM_MBUTTONDOWN:
        Mouse::m = 1;
        break;

    case WM_MBUTTONDBLCLK:
        Mouse::m = 2;
        break;

    case WM_MBUTTONUP:
        Mouse::m = 0;
        break;

    case WM_RBUTTONDOWN:
        Mouse::r = 1;
        break;

    case WM_RBUTTONDBLCLK:
        Mouse::r = 2;
        break;

    case WM_RBUTTONUP:
        Mouse::r = 0;
        break;

    case WM_MOUSEWHEEL:
        {
            int w = (int) (uParam >> 16);
            if (w > 32000) w -= 65536;
            Mouse::w += w;
        }
        break;

    case WM_CLOSE:
        if (game) // && game->Server())
            game->Exit();
        break;

    default:
        return DefWindowProc(hwnd, message, uParam, lParam);
    }

    return 0;
}


const int MAX_KEY_BUF = 512;
static int vkbuf[MAX_KEY_BUF];
static int vkshiftbuf[MAX_KEY_BUF];
static int vkins = 0;
static int vkext = 0;


void
FlushKeys()
{
    Keyboard::FlushKeys();
    vkins = vkext = 0;
}


void
BufferKey(int vkey)
{
    if (vkey < 1) return;

    int shift = 0;

    if (GetAsyncKeyState(VK_SHIFT))
    shift |= 1;

    if (GetAsyncKeyState(VK_CONTROL))
    shift |= 2;

    if (GetAsyncKeyState(VK_MENU))
    shift |= 4;

    vkbuf[vkins] = vkey;
    vkshiftbuf[vkins++] = shift;

    if (vkins >= MAX_KEY_BUF)
    vkins = 0;

    if (vkins == vkext) {
        vkext++;
        if (vkext >= MAX_KEY_BUF)
        vkext = 0;
    }
}


int
GetKey()
{
    if (vkins == vkext) return 0;

    int result = vkbuf[vkext++];
    if (vkext >= MAX_KEY_BUF)
    vkext = 0;

    return result;
}


int
GetKeyPlus(int& key, int& shift)
{
    if (vkins == vkext) return 0;

    key = vkbuf[vkext];
    shift = vkshiftbuf[vkext++];

    if (vkext >= MAX_KEY_BUF)
    vkext = 0;

    return key;
}
