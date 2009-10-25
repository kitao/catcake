/*
    Copyright (c) 2007-2009 Takashi Kitao
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
    `  notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
    `  notice, this list of conditions and the following disclaimer in the
    `  documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products
    `  derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifdef WIN32


#include <windows.h>
#include <zmouse.h>
#include <shlwapi.h>
#include <process.h>

#include "pg_low_level_api.h"

#include "pg_sys_all.h" // for pgSysMgr::SysFlag
#include "pg_key_all.h" // for pgKeyMgr::KeyType


#ifndef PG_MINGW
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "opengl32.lib")
#endif


static pgLowLevelAPI::KeyEventHandler s_key_event_handler = NULL;
static pgLowLevelAPI::MouseEventHandler s_mouse_event_handler = NULL;
static pgLowLevelAPI::ExtraEventHandler s_extra_event_handler = NULL;

static const char* s_app_name;
static u16 s_framebuffer_width;
static u16 s_framebuffer_height;
static u16 s_sys_flag;
static bool s_is_framebuffer_size_changed;
static bool s_is_fullscreen;
static bool s_is_mouse_visible;

static HINSTANCE s_hInstance;
static HWND s_hWnd;
static HDC s_hDC;
static HGLRC s_hRC;


static void callKeyEventHandler(u16 vk, bool is_down)
{
    static const pgKeyMgr::KeyType s_key_table[] =
    {
        pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_LBUTTON, pgKeyMgr::KEY_RBUTTON, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_MBUTTON, // 0-4
        pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_BACKSPACE, pgKeyMgr::KEY_TAB, // 5-9
        pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_ENTER, pgKeyMgr::KEY_NONE, // 10-14
        pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_SHIFT, pgKeyMgr::KEY_CTRL, pgKeyMgr::KEY_ALT, pgKeyMgr::KEY_NONE, // 15-19
        pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, // 20-24
        pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_ESCAPE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, // 25-29
        pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_SPACE, pgKeyMgr::KEY_PAGEUP, pgKeyMgr::KEY_PAGEDOWN, // 30-34
        pgKeyMgr::KEY_END, pgKeyMgr::KEY_HOME, pgKeyMgr::KEY_LEFT, pgKeyMgr::KEY_UP, pgKeyMgr::KEY_RIGHT, // 35-39
        pgKeyMgr::KEY_DOWN, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, // 40-44
        pgKeyMgr::KEY_INSERT, pgKeyMgr::KEY_DELETE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_0, pgKeyMgr::KEY_1, // 45-49
        pgKeyMgr::KEY_2, pgKeyMgr::KEY_3, pgKeyMgr::KEY_4, pgKeyMgr::KEY_5, pgKeyMgr::KEY_6, // 50-54
        pgKeyMgr::KEY_7, pgKeyMgr::KEY_8, pgKeyMgr::KEY_9, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, // 55-59
        pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, // 60-64
        pgKeyMgr::KEY_A, pgKeyMgr::KEY_B, pgKeyMgr::KEY_C, pgKeyMgr::KEY_D, pgKeyMgr::KEY_E, // 65-69
        pgKeyMgr::KEY_F, pgKeyMgr::KEY_G, pgKeyMgr::KEY_H, pgKeyMgr::KEY_I, pgKeyMgr::KEY_J, // 70-74
        pgKeyMgr::KEY_K, pgKeyMgr::KEY_L, pgKeyMgr::KEY_M, pgKeyMgr::KEY_N, pgKeyMgr::KEY_O, // 75-79
        pgKeyMgr::KEY_P, pgKeyMgr::KEY_Q, pgKeyMgr::KEY_R, pgKeyMgr::KEY_S, pgKeyMgr::KEY_T, // 80-84
        pgKeyMgr::KEY_U, pgKeyMgr::KEY_V, pgKeyMgr::KEY_W, pgKeyMgr::KEY_X, pgKeyMgr::KEY_Y, // 85-89
        pgKeyMgr::KEY_Z, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NONE, // 90-94
        pgKeyMgr::KEY_NONE, pgKeyMgr::KEY_NUMPAD0, pgKeyMgr::KEY_NUMPAD1, pgKeyMgr::KEY_NUMPAD2, pgKeyMgr::KEY_NUMPAD3, // 95-99
        pgKeyMgr::KEY_NUMPAD4, pgKeyMgr::KEY_NUMPAD5, pgKeyMgr::KEY_NUMPAD6, pgKeyMgr::KEY_NUMPAD7, pgKeyMgr::KEY_NUMPAD8, // 100-104
        pgKeyMgr::KEY_NUMPAD9, pgKeyMgr::KEY_MULTIPLY, pgKeyMgr::KEY_ADD, pgKeyMgr::KEY_SEPARATOR, pgKeyMgr::KEY_SUBTRACT, // 105-109
        pgKeyMgr::KEY_DECIMAL, pgKeyMgr::KEY_DIVIDE, pgKeyMgr::KEY_F1, pgKeyMgr::KEY_F2, pgKeyMgr::KEY_F3, // 110-114
        pgKeyMgr::KEY_F4, pgKeyMgr::KEY_F5, pgKeyMgr::KEY_F6, pgKeyMgr::KEY_F7, pgKeyMgr::KEY_F8, // 115-119
        pgKeyMgr::KEY_F9, pgKeyMgr::KEY_F10, pgKeyMgr::KEY_F11, pgKeyMgr::KEY_F12, pgKeyMgr::KEY_NONE // 120-124
    };

    static const u32 s_key_table_size = sizeof(s_key_table) / sizeof(pgKeyMgr::KeyType);

    (*s_key_event_handler)((vk < s_key_table_size) ? s_key_table[vk] : pgKeyMgr::KEY_NONE, is_down);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_ENTERSIZEMOVE:
    case WM_EXITSIZEMOVE:
        return 0;

    case WM_SYSCOMMAND:
        switch (wParam)
        {
        case SC_SCREENSAVE:
        case SC_MONITORPOWER:
        case SC_KEYMENU:
            return 0;
        }
        break;

    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        callKeyEventHandler(wParam, true);
        break;

    case WM_KEYUP:
        callKeyEventHandler(wParam, false);
        break;

    case WM_SYSKEYDOWN:
        if (wParam == VK_MENU)
        {
            callKeyEventHandler(VK_MENU, true);
        }
        else if (GetKeyState(VK_F10) < 0)
        {
            callKeyEventHandler(VK_F10, true);
        }
        break;

    case WM_SYSKEYUP:
        if (wParam == VK_MENU)
        {
            callKeyEventHandler(VK_MENU, false);
        }
        else if (GetKeyState(VK_F10) >= 0) // TODO
        {
            callKeyEventHandler(VK_F10, false);
        }
        break;

    case WM_LBUTTONDOWN:
        SetCapture(hWnd);
        callKeyEventHandler(VK_LBUTTON, true);
        break;

    case WM_LBUTTONUP:
        ReleaseCapture();
        callKeyEventHandler(VK_LBUTTON, false);
        break;

    case WM_MBUTTONDOWN:
        SetCapture(hWnd);
        callKeyEventHandler(VK_MBUTTON, true);
        break;

    case WM_MBUTTONUP:
        ReleaseCapture();
        callKeyEventHandler(VK_MBUTTON, false);
        break;

    case WM_RBUTTONDOWN:
        SetCapture(hWnd);
        callKeyEventHandler(VK_RBUTTON, true);
        break;

    case WM_RBUTTONUP:
        ReleaseCapture();
        callKeyEventHandler(VK_RBUTTON, false);
        break;

    case WM_MOUSEWHEEL:
        if (static_cast<short>(HIWORD(wParam)) > 0)
        {
            (*s_key_event_handler)(pgKeyMgr::KEY_WHEELUP, true);
        }
        else
        {
            (*s_key_event_handler)(pgKeyMgr::KEY_WHEELDOWN, true);
        }
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


static void destroyFramebuffer()
{
    if (s_is_fullscreen)
    {
        ChangeDisplaySettings(NULL, 0);
    }

    if (s_hDC)
    {
        ReleaseDC(s_hWnd, s_hDC);
        s_hDC = NULL;
    }

    if (s_hWnd)
    {
        DestroyWindow(s_hWnd);
        s_hWnd = NULL;
    }

    if (s_hInstance)
    {
        UnregisterClass("Pogolyn", s_hInstance);
        s_hInstance = NULL;
    }
}


static bool createFramebuffer(u16 new_width, u16 new_height)
{
    /*
        create window
    */
    s_hInstance = GetModuleHandle(NULL);

    WNDCLASS wc;

    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = static_cast<WNDPROC>(WndProc);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = s_hInstance;
    wc.hIcon = LoadIcon(s_hInstance, MAKEINTRESOURCE(101));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "Pogolyn";

    if (!RegisterClass(&wc))
    {
        return false;
    }

    if (s_is_fullscreen)
    {
        DEVMODE dmScreenSettings;
        pgLowLevelAPI::memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));

        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = new_width;
        dmScreenSettings.dmPelsHeight = new_height;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            s_is_fullscreen = false;

            new_width = s_framebuffer_width;
            new_height = s_framebuffer_height;
        }
    }

    DWORD dwStyle, dwExStyle;

    if (s_is_fullscreen)
    {
        dwStyle = WS_POPUP;
        dwExStyle = WS_EX_APPWINDOW;
    }
    else
    {
        if (s_sys_flag & pgSysMgr::FLAG_VARIABLE_SIZE)
        {
            dwStyle = WS_OVERLAPPEDWINDOW;
        }
        else
        {
            dwStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
        }

        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    }

    RECT rect;

    rect.left = 0;
    rect.right = new_width;
    rect.top = 0;
    rect.bottom = new_height;

    AdjustWindowRectEx(&rect, dwStyle, false, dwExStyle);

    s_hWnd = CreateWindowEx( //
        dwExStyle, // dwExStyle
        "Pogolyn", // lpClassName
        s_app_name, // lpWindowName
        dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, // dwStyle
        CW_USEDEFAULT, // x
        CW_USEDEFAULT, // y
        rect.right - rect.left, // nWidth
        rect.bottom - rect.top, // nHeight
        NULL, // hWndParent
        NULL, // hMenu
        s_hInstance, // hInstance
        NULL); // lpParam

    if (!s_hWnd)
    {
        destroyFramebuffer();
        return false;
    }

    s_hDC = GetDC(s_hWnd);

    if (!s_hDC)
    {
        destroyFramebuffer();
        return false;
    }

    /*
        initialize OpenGL
    */
    static const PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR), // nSize
        1, // nVersion
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // dwFlags
        PFD_TYPE_RGBA, // iPixelType
        24, // cColorBits
        0, 0, 0, 0, 0, 0, // Not used
        8, // cAlphaBits
        0, // Not used
        0, // cAccumBits
        0, 0, 0, 0, // Not used
        16, // cDepthBits
        0, // cStencilBits
        0, // cAuxBuffers
        PFD_MAIN_PLANE, // iLayerType
        0, 0, 0, 0, // Not used
    };

    int PixelFormat = ChoosePixelFormat(s_hDC, &pfd);

    if (!PixelFormat)
    {
        destroyFramebuffer();
        return false;
    }

    if (!SetPixelFormat(s_hDC, PixelFormat, &pfd))
    {
        destroyFramebuffer();
        return false;
    }

    s_hRC = wglCreateContext(s_hDC);

    if (!s_hRC)
    {
        destroyFramebuffer();
        return false;
    }

    if (!wglMakeCurrent(s_hDC, s_hRC))
    {
        destroyFramebuffer();
        return false;
    }

    /*
        wait vsync
    */
    typedef bool (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);

    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));

    if (wglSwapIntervalEXT)
    {
        wglSwapIntervalEXT(1);
    }

    /*
        show window
    */
    ShowWindow(s_hWnd, SW_SHOW);
    SetForegroundWindow(s_hWnd);
    SetFocus(s_hWnd);

    GetClientRect(s_hWnd, &rect);

    s_framebuffer_width = static_cast<u16>(rect.right);
    s_framebuffer_height = static_cast<u16>(rect.bottom);

    return true;
}


bool pgLowLevelAPI::createApplication(const char* title, u16 width, u16 height, u16 sys_flag)
{
    s_app_name = title;
    s_framebuffer_width = width;
    s_framebuffer_height = height;
    s_sys_flag = sys_flag;
    s_is_framebuffer_size_changed = false;
    s_is_fullscreen = (sys_flag & pgSysMgr::FLAG_FULLSCREEN_START) ? true : false;
    s_is_mouse_visible = true;

    if (!createFramebuffer(width, height))
    {
        return false;
    }

    setupShaderAPI((sys_flag & pgSysMgr::FLAG_DISABLE_SHADER) ? false : true);

    return true;
}


void pgLowLevelAPI::destroyApplication()
{
    destroyFramebuffer();
}


void pgLowLevelAPI::startApplication(bool (*update_func)(void))
{
    while (true)
    {
        MSG msg;
        msg.wParam = 0;

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                return;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        POINT pos;

        GetCursorPos(&pos);
        ScreenToClient(s_hWnd, &pos);
        (*s_mouse_event_handler)(static_cast<s16>(pos.x), static_cast<s16>(pos.y));

        (*update_func)();
    }
}


u16 pgLowLevelAPI::getFramebufferWidth()
{
    return s_framebuffer_width;
}


u16 pgLowLevelAPI::getFramebufferHeight()
{
    return s_framebuffer_height;
}


void pgLowLevelAPI::updateFramebufferSize()
{
    RECT rect;
    GetClientRect(s_hWnd, &rect);

    if (rect.right != s_framebuffer_width || rect.bottom != s_framebuffer_height)
    {
        s_framebuffer_width = static_cast<u16>(rect.right);
        s_framebuffer_height = static_cast<u16>(rect.bottom);
        s_is_framebuffer_size_changed = true;
    }
    else
    {
        s_is_framebuffer_size_changed = false;
    }
}


bool pgLowLevelAPI::isFramebufferSizeChanged()
{
    return s_is_framebuffer_size_changed;
}


void pgLowLevelAPI::swapFramebuffer()
{
    ::SwapBuffers(s_hDC);
}


bool pgLowLevelAPI::isFullScreen()
{
    return s_is_fullscreen;
}


bool pgLowLevelAPI::toggleFullScreen(u16 width, u16 height)
{
    destroyFramebuffer();

    s_is_fullscreen = !s_is_fullscreen;

    return createFramebuffer(width, height);
}


void pgLowLevelAPI::setKeyEventHandler(KeyEventHandler handler)
{
    s_key_event_handler = handler;
}


void pgLowLevelAPI::setMouseEventHandler(MouseEventHandler handler)
{
    s_mouse_event_handler = handler;
}


void pgLowLevelAPI::setExtraEventHandler(ExtraEventHandler handler)
{
    s_extra_event_handler = handler;
}


void pgLowLevelAPI::setMousePos(s16 mouse_x, s16 mouse_y)
{
    POINT pos;

    pos.x = mouse_x;
    pos.y = mouse_y;

    ClientToScreen(s_hWnd, &pos);
    SetCursorPos(pos.x, pos.y);
}


bool pgLowLevelAPI::isMouseVisible()
{
    return s_is_mouse_visible;
}


void pgLowLevelAPI::setMouseVisible(bool is_visible)
{
    if (is_visible != s_is_mouse_visible)
    {
        s_is_mouse_visible = is_visible;

        ShowCursor(s_is_mouse_visible);
    }
}


u64 pgLowLevelAPI::getUsecTime()
{
    static bool is_first = true;
    static LARGE_INTEGER freq;
    static LARGE_INTEGER now;

    if (is_first)
    {
        QueryPerformanceFrequency(&freq);
        is_first = false;
    }

    QueryPerformanceCounter(&now);
    return static_cast<u64>(static_cast<r64>(now.QuadPart) * 1000.0 * 1000.0 / freq.QuadPart);
}


void pgLowLevelAPI::sleepUsec(u64 usec)
{
    Sleep(static_cast<u32>(usec / 1000));
}


void pgLowLevelAPI::exit(s32 status)
{
    ::exit(status);
}


void pgLowLevelAPI::error(const char* msg)
{
    printf(msg);
    MessageBoxA(NULL, msg, "Pogolyn Error", MB_OK | MB_ICONERROR);

    exit(1);
}


void pgLowLevelAPI::readLittleEndian(void* dest, const void* src, u32 size)
{
    memcpy(dest, src, size);
}


void pgLowLevelAPI::writeLittleEndian(void* dest, const void* src, u32 size)
{
    memcpy(dest, src, size);
}


void* pgLowLevelAPI::newThread(void (*start_func)(void*), void* user_param)
{
    return reinterpret_cast<void*>(_beginthread(start_func, 0, user_param));
}


void pgLowLevelAPI::deleteThread(void* thread_handler) {}


void pgLowLevelAPI::joinThread(void* thread_handler)
{
    WaitForSingleObject(thread_handler, INFINITE);
}


void* pgLowLevelAPI::newMutex()
{
    return CreateMutex(NULL, false, NULL);
}


void pgLowLevelAPI::deleteMutex(void* mutex_handler)
{
    CloseHandle(mutex_handler);
}


void pgLowLevelAPI::lockMutex(void* mutex_handler)
{
    WaitForSingleObject(mutex_handler, INFINITE);
}


void pgLowLevelAPI::unlockMutex(void* mutex_handler)
{
    ReleaseMutex(mutex_handler);
}


void pgLowLevelAPI::setInitialDirectory(s32 argc, char** argv)
{
    if (argc > 0)
    {
        PathRemoveFileSpec(argv[0]);
        SetCurrentDirectory(argv[0]);
    }
}


void pgLowLevelAPI::getWindowsFontDirectory(char* buf, u32 buf_size)
{
    GetWindowsDirectory(buf, buf_size);

#ifdef PG_MINGW
    strcat(buf, "\\Fonts\\");
#else
    strcat_s(buf, buf_size, "\\Fonts\\");
#endif
}


#endif // WIN32
