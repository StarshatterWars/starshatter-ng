/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.
*/

#ifndef GameWinDX9_h
#define GameWinDX9_h

#include "Color.h"
#include "Game.h"
#include "Screen.h"
#include "SoundCard.h"
#include "Types.h"
#include "VideoFactory.h"
#include "Video.h"
#include "VideoSettings.h"
#include "WndProc.h"


class GameWinDX9 : public Game
{
public:
    static GameWinDX9* GetInstance();

    GameWinDX9();
    virtual ~GameWinDX9();

    virtual bool Init(HINSTANCE hi, HINSTANCE hpi, LPSTR cmdline, int nCmdShow);
    virtual bool InitApplication(HINSTANCE hInstance);
    virtual bool InitInstance(HINSTANCE hInstance, int nCmdShow);
    virtual bool InitGame();

    virtual bool InitVideo();
    virtual bool ResizeVideo();
    virtual bool ResetVideo();
    virtual bool ToggleFullscreen();
    virtual bool AdjustWindowForChange();
    virtual bool IsWindowed();

    virtual bool SetupPalette();
    virtual bool LoadPalette(PALETTEENTRY* pal, BYTE* inv);

    virtual void PumpEvents() override;
    virtual void UpdateScreen() override;
    virtual void Activate(bool f) override;
    virtual void Pause(bool f) override;
    void ShowStats();

    Color GetScreenColor() const;
    int GetScreenWidth() const;
    int GetScreenHeight() const;

    HINSTANCE GetHINST();
    HWND GetHWND();

    void SetScreenColor(Color c);

protected:
    friend  LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM uParam, LPARAM lParam);

    PALETTEENTRY      standard_palette[256];
    BYTE              inverse_palette[32768];

    Color             screen_color;

    HINSTANCE         hInst;
    HWND              hwnd;
    HMENU             hmenu;

    // Internal variables for the state of the app
    DWORD             window_style;        // Saved window style for mode switches
    RECT              bounds_rect;         // Saved window bounds for mode switches
    RECT              client_rect;         // Saved client area size for mode switches

    const char*       app_name;
    const char*       title_text;
    const char*       palette_name;

    VideoFactory*     video_factory;
    Video*            video;
    VideoSettings*    video_settings;
    SoundCard*        soundcard;
    Screen*           screen;

    bool              is_active;
    bool              is_device_lost;
    bool              ignore_size_change;
    bool              is_device_initialized;
    bool              is_device_restored;

private:
    static GameWinDX9* instance;
};


#endif  // GameWinDX9_h
