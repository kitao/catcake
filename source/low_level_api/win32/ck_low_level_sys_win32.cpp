/*
    Copyright (c) 2007-2010 Takashi Kitao

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/


#ifdef WIN32


#include <windows.h>
#include <zmouse.h>
#include <shlwapi.h>
#include <process.h>

#include "ck_low_level_api.h"

#include "ck_sys_all.h" // for ckSysMgr::SysFlag
#include "ck_key_all.h" // for ckKeyMgr::KeyType


#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "opengl32.lib")


static ckLowLevelAPI::KeyEventHandler s_key_event_handler = NULL;
static ckLowLevelAPI::MouseEventHandler s_mouse_event_handler = NULL;
static ckLowLevelAPI::ExtraEventHandler s_extra_event_handler = NULL;

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
    static const ckKeyMgr::KeyType s_key_table[] =
    {
        ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_LBUTTON, ckKeyMgr::KEY_RBUTTON, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_MBUTTON, // 0-4
        ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_BACKSPACE, ckKeyMgr::KEY_TAB, // 5-9
        ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_ENTER, ckKeyMgr::KEY_NONE, // 10-14
        ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_SHIFT, ckKeyMgr::KEY_CTRL, ckKeyMgr::KEY_ALT, ckKeyMgr::KEY_NONE, // 15-19
        ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, // 20-24
        ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_ESCAPE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, // 25-29
        ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_SPACE, ckKeyMgr::KEY_PAGEUP, ckKeyMgr::KEY_PAGEDOWN, // 30-34
        ckKeyMgr::KEY_END, ckKeyMgr::KEY_HOME, ckKeyMgr::KEY_LEFT, ckKeyMgr::KEY_UP, ckKeyMgr::KEY_RIGHT, // 35-39
        ckKeyMgr::KEY_DOWN, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, // 40-44
        ckKeyMgr::KEY_INSERT, ckKeyMgr::KEY_DELETE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_0, ckKeyMgr::KEY_1, // 45-49
        ckKeyMgr::KEY_2, ckKeyMgr::KEY_3, ckKeyMgr::KEY_4, ckKeyMgr::KEY_5, ckKeyMgr::KEY_6, // 50-54
        ckKeyMgr::KEY_7, ckKeyMgr::KEY_8, ckKeyMgr::KEY_9, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, // 55-59
        ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, // 60-64
        ckKeyMgr::KEY_A, ckKeyMgr::KEY_B, ckKeyMgr::KEY_C, ckKeyMgr::KEY_D, ckKeyMgr::KEY_E, // 65-69
        ckKeyMgr::KEY_F, ckKeyMgr::KEY_G, ckKeyMgr::KEY_H, ckKeyMgr::KEY_I, ckKeyMgr::KEY_J, // 70-74
        ckKeyMgr::KEY_K, ckKeyMgr::KEY_L, ckKeyMgr::KEY_M, ckKeyMgr::KEY_N, ckKeyMgr::KEY_O, // 75-79
        ckKeyMgr::KEY_P, ckKeyMgr::KEY_Q, ckKeyMgr::KEY_R, ckKeyMgr::KEY_S, ckKeyMgr::KEY_T, // 80-84
        ckKeyMgr::KEY_U, ckKeyMgr::KEY_V, ckKeyMgr::KEY_W, ckKeyMgr::KEY_X, ckKeyMgr::KEY_Y, // 85-89
        ckKeyMgr::KEY_Z, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NONE, // 90-94
        ckKeyMgr::KEY_NONE, ckKeyMgr::KEY_NUMPAD0, ckKeyMgr::KEY_NUMPAD1, ckKeyMgr::KEY_NUMPAD2, ckKeyMgr::KEY_NUMPAD3, // 95-99
        ckKeyMgr::KEY_NUMPAD4, ckKeyMgr::KEY_NUMPAD5, ckKeyMgr::KEY_NUMPAD6, ckKeyMgr::KEY_NUMPAD7, ckKeyMgr::KEY_NUMPAD8, // 100-104
        ckKeyMgr::KEY_NUMPAD9, ckKeyMgr::KEY_MULTIPLY, ckKeyMgr::KEY_ADD, ckKeyMgr::KEY_SEPARATOR, ckKeyMgr::KEY_SUBTRACT, // 105-109
        ckKeyMgr::KEY_DECIMAL, ckKeyMgr::KEY_DIVIDE, ckKeyMgr::KEY_F1, ckKeyMgr::KEY_F2, ckKeyMgr::KEY_F3, // 110-114
        ckKeyMgr::KEY_F4, ckKeyMgr::KEY_F5, ckKeyMgr::KEY_F6, ckKeyMgr::KEY_F7, ckKeyMgr::KEY_F8, // 115-119
        ckKeyMgr::KEY_F9, ckKeyMgr::KEY_F10, ckKeyMgr::KEY_F11, ckKeyMgr::KEY_F12, ckKeyMgr::KEY_NONE // 120-124
    };

    static const u32 s_key_table_size = sizeof(s_key_table) / sizeof(ckKeyMgr::KeyType);

    (*s_key_event_handler)((vk < s_key_table_size) ? s_key_table[vk] : ckKeyMgr::KEY_NONE, is_down);
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
            (*s_key_event_handler)(ckKeyMgr::KEY_WHEELUP, true);
        }
        else
        {
            (*s_key_event_handler)(ckKeyMgr::KEY_WHEELDOWN, true);
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
        UnregisterClass("Catcake", s_hInstance);
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
    wc.lpszClassName = "Catcake";

    if (!RegisterClass(&wc))
    {
        return false;
    }

    if (s_is_fullscreen)
    {
        DEVMODE dmScreenSettings;
        ckLowLevelAPI::memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));

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
        if (s_sys_flag & ckSysMgr::FLAG_VARIABLE_SIZE)
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
        "Catcake", // lpClassName
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


bool ckLowLevelAPI::createApplication(const char* title, u16 width, u16 height, u16 sys_flag)
{
    s_app_name = title;
    s_framebuffer_width = width;
    s_framebuffer_height = height;
    s_sys_flag = sys_flag;
    s_is_framebuffer_size_changed = false;
    s_is_fullscreen = (sys_flag & ckSysMgr::FLAG_FULLSCREEN_START) ? true : false;
    s_is_mouse_visible = true;

    if (!createFramebuffer(width, height))
    {
        return false;
    }

    setupShaderAPI((sys_flag & ckSysMgr::FLAG_DISABLE_SHADER) ? false : true);

    return true;
}


void ckLowLevelAPI::destroyApplication()
{
    destroyFramebuffer();
}


void ckLowLevelAPI::startApplication(bool (*update_func)(void))
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


u16 ckLowLevelAPI::getFramebufferWidth()
{
    return s_framebuffer_width;
}


u16 ckLowLevelAPI::getFramebufferHeight()
{
    return s_framebuffer_height;
}


void ckLowLevelAPI::updateFramebufferSize()
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


bool ckLowLevelAPI::isFramebufferSizeChanged()
{
    return s_is_framebuffer_size_changed;
}


void ckLowLevelAPI::swapFramebuffer()
{
    ::SwapBuffers(s_hDC);
}


bool ckLowLevelAPI::isFullScreen()
{
    return s_is_fullscreen;
}


bool ckLowLevelAPI::toggleFullScreen(u16 width, u16 height)
{
    destroyFramebuffer();

    s_is_fullscreen = !s_is_fullscreen;

    return createFramebuffer(width, height);
}


void ckLowLevelAPI::setKeyEventHandler(KeyEventHandler handler)
{
    s_key_event_handler = handler;
}


void ckLowLevelAPI::setMouseEventHandler(MouseEventHandler handler)
{
    s_mouse_event_handler = handler;
}


void ckLowLevelAPI::setExtraEventHandler(ExtraEventHandler handler)
{
    s_extra_event_handler = handler;
}


void ckLowLevelAPI::setMousePos(s16 mouse_x, s16 mouse_y)
{
    POINT pos;

    pos.x = mouse_x;
    pos.y = mouse_y;

    ClientToScreen(s_hWnd, &pos);
    SetCursorPos(pos.x, pos.y);
}


bool ckLowLevelAPI::isMouseVisible()
{
    return s_is_mouse_visible;
}


void ckLowLevelAPI::setMouseVisible(bool is_visible)
{
    if (is_visible != s_is_mouse_visible)
    {
        s_is_mouse_visible = is_visible;

        ShowCursor(s_is_mouse_visible);
    }
}


u64 ckLowLevelAPI::getUsecTime()
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


void ckLowLevelAPI::sleepUsec(u64 usec)
{
    Sleep(static_cast<u32>(usec / 1000));
}


void ckLowLevelAPI::exit(s32 status)
{
    ::exit(status);
}


void ckLowLevelAPI::error(const char* msg)
{
    printf(msg);
    MessageBoxA(NULL, msg, "Catcake Error", MB_OK | MB_ICONERROR);

    exit(1);
}


void ckLowLevelAPI::readLittleEndian(void* dest, const void* src, u32 size)
{
    memcpy(dest, src, size);
}


void ckLowLevelAPI::writeLittleEndian(void* dest, const void* src, u32 size)
{
    memcpy(dest, src, size);
}


void* ckLowLevelAPI::newThread(void (*start_func)(void*), void* user_param)
{
    return reinterpret_cast<void*>(_beginthread(start_func, 0, user_param));
}


void ckLowLevelAPI::deleteThread(void* thread_handler) {}


void ckLowLevelAPI::joinThread(void* thread_handler)
{
    WaitForSingleObject(thread_handler, INFINITE);
}


void* ckLowLevelAPI::newMutex()
{
    return CreateMutex(NULL, false, NULL);
}


void ckLowLevelAPI::deleteMutex(void* mutex_handler)
{
    CloseHandle(mutex_handler);
}


void ckLowLevelAPI::lockMutex(void* mutex_handler)
{
    WaitForSingleObject(mutex_handler, INFINITE);
}


void ckLowLevelAPI::unlockMutex(void* mutex_handler)
{
    ReleaseMutex(mutex_handler);
}


void ckLowLevelAPI::setInitialDirectory(s32 argc, char** argv)
{
    if (argc > 0)
    {
        PathRemoveFileSpec(argv[0]);
        SetCurrentDirectory(argv[0]);
    }
}


void ckLowLevelAPI::getWindowsFontDirectory(char* buf, u32 buf_size)
{
    GetWindowsDirectory(buf, buf_size);
    strcat_s(buf, buf_size, "\\Fonts\\");
}


#endif // WIN32
