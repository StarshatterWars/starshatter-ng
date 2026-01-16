/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#include "GameWinDX9.h"

#include <stdio.h>
#include <string.h>

#include <InfoEx.h>

#include "Bitmap.h"
#include "Clock.h"
#include "Color.h"
#include "DataLoader.h"
#include "Game.h"
#include "Panic.h"
#include "Screen.h"
#include "Types.h"
#include "Utils.h"
#include "VideoFactory.h"
#include "Video.h"
#include "VideoSettings.h"


GameWinDX9* GameWinDX9::instance {nullptr};


GameWinDX9*
GameWinDX9::GetInstance()
{
    return instance;
}


GameWinDX9::GameWinDX9() :
    screen_color {Color::Black},
    hInst {nullptr},
    hwnd {nullptr},
    hmenu {nullptr},
    window_style {0},
    video_factory {nullptr},
    video {nullptr},
    video_settings {nullptr},
    soundcard {nullptr},
    screen {nullptr},
    is_active {false},
    is_device_lost {false},
    ignore_size_change {false},
    is_device_initialized {false},
    is_device_restored {false}
{
    if (instance != nullptr)
        throw "GameWinDX9 may have only one instance";
    instance = this;
    video_settings = new VideoSettings;
}


GameWinDX9::~GameWinDX9()
{
    if (screen) delete screen;
    if (video_factory) delete video_factory;
    if (video) delete video;
    if (soundcard) delete soundcard;
    if (video_settings) delete video_settings;
    if (instance == this)
        instance = nullptr;
}


bool
GameWinDX9::Init(HINSTANCE hi, HINSTANCE hpi, LPSTR cmdline, int nCmdShow)
{
    status = OK;
    hInst  = hi;

    Print("  Initializing Game\n");

    if (!InitApplication(hInst)) {  // Initialize shared things
        Panic::Panic("Could not initialize the application.");
        status = INIT_FAILED;
    }

    if (status == OK && !video_settings) {
        Panic::Panic("No video settings specified");
        status = INIT_FAILED;
    }

    if (status == OK && InfoEx::DirectX() < InfoEx::DXVersion::Dx9) {
        Panic::Panic("  Insufficient DirectX detected (Dx9 IS REQUIRED)");
        status = INIT_FAILED;
    }

    if (status == OK) {
        Print("\n  Initializing instance...\n");
        // Perform initializations that apply to a specific instance
        if (!InitInstance(hInst, nCmdShow)) {
            Panic::Panic("Could not initialize the instance.");
            status = INIT_FAILED;
        }
    }

    if (status != OK)
        return false;
    return Game::Init();
}


bool
GameWinDX9::InitApplication(HINSTANCE hInstance)
{
    WNDCLASS  wc;
    LOGBRUSH  brush = { BS_SOLID, RGB(0,0,0), 0 };

    // Fill in window class structure with parameters that
    // describe the main window.
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC) WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(100));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

    wc.hbrBackground = CreateBrushIndirect(&brush);
    wc.lpszMenuName  = app_name;
    wc.lpszClassName = app_name;

    // Register the window class and return success/failure code.
    if (RegisterClass(&wc) == 0) {
        DWORD err = GetLastError();

        if (err == 1410) // class already exists, this is OK
        return true;

        else
        Print("WARNING: Register Window Class: %08x\n", err);
    }

    return true;
}


bool
GameWinDX9::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    // center window on display:
    int   screenx  = GetSystemMetrics(SM_CXSCREEN);
    int   screeny  = GetSystemMetrics(SM_CYSCREEN);
    int   x_offset = 0;
    int   y_offset = 0;
    int   s_width  = 800;
    int   s_height = 600;

    if (video_settings) {
        s_width  = video_settings->window_width;
        s_height = video_settings->window_height;
    }

    if (s_width < screenx)
    x_offset = (screenx - s_width) / 2;

    if (s_height < screeny)
    y_offset = (screeny - s_height) / 2;

    // Create a main window for this application instance
    RECT  rctmp;
    rctmp.left   = x_offset;
    rctmp.top    = y_offset;
    rctmp.right  = x_offset + s_width;
    rctmp.bottom = y_offset + s_height;

    window_style =
        WS_OVERLAPPED  | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME |
        WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE;

    AdjustWindowRect(&rctmp, window_style, 1);

    hwnd = CreateWindow(
        app_name,                  // Class name
        app_name,                  // Caption
        window_style,
        x_offset,                  // Position
        y_offset,
        rctmp.right - rctmp.left,  // Size
        rctmp.bottom - rctmp.top,
        0,                         // Parent window (no parent)
        0,                         // use class menu
        hInst,                     // handle to window instance
        0);                        // no params to pass on

    // If window could not be created, return "failure"
    if (!hwnd) {
        Panic::Panic("Could not create window\n");
        return false;
    }

    Print("  Window created.\n");

    // Make the window visible and draw it
    ShowWindow(hwnd, nCmdShow); // Show the window
    UpdateWindow(hwnd);         // Sends WM_PAINT message

    // Save window properties
    window_style = GetWindowLong(hwnd, GWL_STYLE);
    GetWindowRect(hwnd, &bounds_rect);
    GetClientRect(hwnd, &client_rect);

    // Use client area to set video window size
    video_settings->window_width  = client_rect.right - client_rect.left;
    video_settings->window_height = client_rect.bottom - client_rect.top;

    Print("  Instance initialized.\n");
    return true;
}


bool
GameWinDX9::InitGame()
{
    if (!SetupPalette()) {
        Panic::Panic("Could not set up the palette.");
        return false;
    }
    Print("  Palette laoded\n");
    if (!InitVideo() || !video || video->Status() != Video::VIDEO_OK) {
        Panic::Panic("Could not create the Video Interface");
        return false;
    }
    Print("  Created video object.\n");
    screen = new Screen(video);
    if (!screen) {
        Panic::Panic("Could not create the Screen object.");
        return false;
    }
    Print("  Created screen object.\n");
    if (!screen->SetBackgroundColor(Color::Black))
        Print("  WARNING: Could not set video background color to Black.\n");
    screen->ClearAllFrames(true);
    Print("Established requested video parameters.\n\n");
    return Game::InitGame();
}


bool
GameWinDX9::InitVideo()
{
    // create a video factory, and video object:
    video_factory = new VideoFactory(hwnd);

    if (video_factory) {
        Print("  Init Video...\n");
        Print("  Request %s mode\n", video_settings->GetModeDescription());

        video = video_factory->CreateVideo(video_settings);

        if (video) {
            if (!video->IsHardware()) {
                video_factory->DestroyVideo(video);
                video = 0;

                Panic::Panic("3D Hardware Not Found");
            }

            // save a copy of the device-specific video settings:
            else if (video->GetVideoSettings()) {
                *video_settings = *video->GetVideoSettings();
            }
        }

        soundcard = video_factory->CreateSoundCard();
    }

    return (video && video->Status() == Video::VIDEO_OK);
}


bool
GameWinDX9::ResetVideo()
{
    if (!video_factory)  return InitVideo();

    Print("  Reset Video...\n");
    Print("  Request %s mode\n", video_settings->GetModeDescription());

    delete screen;

    if (video && !video->Reset(video_settings)) {
        video_factory->DestroyVideo(video);
        video = video_factory->CreateVideo(video_settings);
    }

    if (!video || video->Status() != Video::VIDEO_OK) {
        Panic::Panic("Could not re-create Video Interface.");
        return false;
    }

    Print("  Re-created video object.\n");

    // save a copy of the device-specific video settings:
    if (video->GetVideoSettings()) {
        *video_settings = *video->GetVideoSettings();
    }

    screen = new Screen(video);
    if (!screen) {
        Panic::Panic("Could not re-create Screen object.");
        return false;
    }

    Print("  Re-created screen object.\n");

    if (!screen->SetBackgroundColor(Color::Black))
    Print("  WARNING: could not set video background color to Black\n");

    screen->ClearAllFrames(true);

    Print("  Re-established requested video parameters.\n");

    Bitmap::CacheUpdate();
    Print("  Refreshed texture bitmaps.\n\n");
    return true;
}


bool
GameWinDX9::ResizeVideo()
{
    if (!video || !video_settings)   return false;
    if (!IsWindowed())               return false;
    if (ignore_size_change)          return true;

    HRESULT  hr = S_OK;
    RECT     client_old;

    client_old = client_rect;

    // Update window properties
    GetWindowRect(hwnd, &bounds_rect);
    GetClientRect(hwnd, &client_rect);

    if (client_old.right   - client_old.left  !=
            client_rect.right  - client_rect.left ||
            client_old.bottom  - client_old.top   !=
            client_rect.bottom - client_rect.top) {

        // A new window size will require a new backbuffer
        // size, so the 3D structures must be changed accordingly.
        Pause(true);

        video_settings->is_windowed   = true;
        video_settings->window_width  = client_rect.right - client_rect.left;
        video_settings->window_height = client_rect.bottom - client_rect.top;

        ::Print("ResizeVideo() %d x %d\n", video_settings->window_width, video_settings->window_height);

        if (video) {
            video->Reset(video_settings);
        }

        Pause(false);
    }

    // save a copy of the device-specific video settings:
    if (video->GetVideoSettings()) {
        *video_settings = *video->GetVideoSettings();
    }

    screen->Resize(video_settings->window_width, video_settings->window_height);

    return hr == S_OK;
}


bool
GameWinDX9::ToggleFullscreen()
{
    bool result = false;

    if (video && video_settings) {
        Pause(true);
        ignore_size_change = true;

        // Toggle the windowed state
        video_settings->is_windowed = !video_settings->is_windowed;

        // Prepare window for windowed/fullscreen change
        AdjustWindowForChange();

        // Reset the 3D device
        if (!video->Reset(video_settings)) {
            // reset failed, try to restore...
            ignore_size_change = false;

            if (!IsWindowed()) {
                // Restore window type to windowed mode
                video_settings->is_windowed = !video_settings->is_windowed;

                AdjustWindowForChange();

                SetWindowPos(hwnd,
                HWND_NOTOPMOST,
                bounds_rect.left,
                bounds_rect.top,
                bounds_rect.right  - bounds_rect.left,
                bounds_rect.bottom - bounds_rect.top,
                SWP_SHOWWINDOW);
            }

            ::Print("Unable to toggle %s fullscreen mode.\n", IsWindowed() ? "into" : "out of");
        }

        else {
            ignore_size_change = false;

            // When moving from fullscreen to windowed mode, it is important to
            // adjust the window size after resetting the device rather than
            // beforehand to ensure that you get the window size you want.  For
            // example, when switching from 640x480 fullscreen to windowed with
            // a 1000x600 window on a 1024x768 desktop, it is impossible to set
            // the window size to 1000x600 until after the display mode has
            // changed to 1024x768, because windows cannot be larger than the
            // desktop.

            if (IsWindowed()) {
                SetWindowPos(hwnd,
                HWND_NOTOPMOST,
                bounds_rect.left,
                bounds_rect.top,
                bounds_rect.right  - bounds_rect.left,
                bounds_rect.bottom - bounds_rect.top,
                SWP_SHOWWINDOW);
            }

            GetClientRect(hwnd, &client_rect);  // Update our copy
            Pause(false);

            if (IsWindowed())
            screen->Resize(video_settings->window_width,
            video_settings->window_height);

            else
            screen->Resize(video_settings->fullscreen_mode.width,
            video_settings->fullscreen_mode.height);

            result = true;
        }
    }

    return result;
}


bool
GameWinDX9::AdjustWindowForChange()
{
    if (IsWindowed()) {
        // Set windowed-mode style
        SetWindowLong(hwnd, GWL_STYLE, window_style);
        if (hmenu != NULL) {
            SetMenu(hwnd, hmenu);
            hmenu = NULL;
        }
    }
    else {
        // Set fullscreen-mode style
        SetWindowLong(hwnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE);
        if (hmenu == NULL) {
            hmenu = GetMenu(hwnd);
            SetMenu(hwnd, NULL);
        }
    }

    return true;
}


bool
GameWinDX9::IsWindowed()
{
	if (video_settings)
		return video_settings->IsWindowed();
	return true;
}


bool
GameWinDX9::SetupPalette()
{
    if (LoadPalette(standard_palette, inverse_palette)) {
        Color::SetPalette(standard_palette, 256, inverse_palette);
        return true;
    }

    return false;
}


bool
GameWinDX9::LoadPalette(PALETTEENTRY* pal, BYTE* inv)
{
    char palheader[32];
    struct {
        WORD Version;
        WORD NumberOfEntries;
        PALETTEENTRY Entries[256];
    } Palette = { 0x300, 256 };

    DataLoader* loader = DataLoader::GetLoader();
    BYTE* block;
    char fname[256];
    sprintf_s(fname, "%s.pal", palette_name);

    if (!loader->LoadBuffer(fname, block)) {
        Print("  Could not open file '%s'\n", fname);
        return false;
    }

    memcpy(&palheader, block, 24);
    memcpy((char*) Palette.Entries, (block+24), 256*4);

    for (int i = 0; i < 256; i++) {
        *pal++ = Palette.Entries[i];
    }

    loader->ReleaseBuffer(block);

    sprintf_s(fname, "%s.ipl", palette_name);
    int size = loader->LoadBuffer(fname, block);
    if (size < 32768) {
        Print("  Could not open file '%s'\n", fname);
        return false;
    }

    memcpy(inv, block, 32768);
    loader->ReleaseBuffer(block);

    return true;
}


void
GameWinDX9::PumpEvents()
{
    MSG msg;
    while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            if (!exit_code)
                exit_code = msg.wParam;
            Exit();
            break;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}


void
GameWinDX9::UpdateScreen()
{
    if (!screen || !video) return;

    if (screen->Refresh()) {
        video->Present();
    }
    else {
        Panic::Panic("Screen refresh failed.");
    }
}


void
GameWinDX9::Activate(bool f)
{
    Game::Activate(f);
    if (f && video)
        video->InvalidateCache();
}


void
GameWinDX9::Pause(bool f)
{
    Game::Pause(f);
    if (soundcard) {
        if (f)
            soundcard->Pause();
        else
            soundcard->Resume();
    }
}


void
GameWinDX9::ShowStats()
{
    auto total_time = Clock::GetInstance()->RealTime();
    auto stats = Video::GetInstance()->GetStats();

    Print("\n");
    Print("Performance Data:\n");
    Print("-----------------\n");

    Print("   Time:            %d msec\n", total_time);
    Print("   Frames:          %d\n", stats.nframe);
    Print("   Polys Rendered:  %d\n", stats.total_polys);
    Print("   Lines Rendered:  %d\n", stats.total_lines);
    Print("   Verts Rendered:  %d\n", stats.total_verts);
    Print("   Render Calls:    %d\n", stats.ncalls);
    Print("\n");

    Print("Performance Statistics:\n");
    Print("-----------------------\n");

    Print("   Frames/Second:   %.2f\n", (stats.nframe * 1000.0)      / total_time);
    Print("   Polys/Frame:     %.2f\n", (double) stats.total_polys   / (double) stats.nframe);
    Print("   Polys/Call:      %.2f\n", (double) stats.total_polys   / (double) stats.ncalls);
    Print("   Polys/Second:    %.2f\n", (stats.total_polys * 1000.0) / total_time);
    Print("   Lines/Second:    %.2f\n", (stats.total_lines * 1000.0) / total_time);
    Print("   Verts/Second:    %.2f\n", (stats.total_verts * 1000.0) / total_time);

    Print("\n");
}


Color
GameWinDX9::GetScreenColor() const
{
    return screen_color;
}


int
GameWinDX9::GetScreenWidth() const
{
    if (video)
        return video->Width();
    return 0;
}


int
GameWinDX9::GetScreenHeight() const
{
    if (video)
        return video->Height();
    return 0;
}


HINSTANCE
GameWinDX9::GetHINST()
{
    return hInst;
}


HWND
GameWinDX9::GetHWND()
{
    return hwnd;
}


void
GameWinDX9::SetScreenColor(Color c)
{
    screen_color = c;
    if (screen)
        screen->SetBackgroundColor(c);
}
