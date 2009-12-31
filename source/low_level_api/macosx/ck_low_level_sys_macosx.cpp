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


#ifdef CK_MACOSX

#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <pthread.h>
#include <GLUT/glut.h>

#include "ck_low_level_api.h"

#include "ck_sys_all.h" // for ckSysMgr::SysFlag
#include "ck_key_all.h" // for ckKeyMgr::KeyType


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

static s32 s_argc;
static char** s_argv;
static bool (*s_update_func)(void);

struct KeyCode
{
    unsigned char key;
    ckKeyMgr::KeyType type;
};

struct AlphaKeyCode
{
    unsigned char keyL;
    unsigned char keyC;
    ckKeyMgr::KeyType type;
};

struct SpecialKeyCode
{
    int key;
    ckKeyMgr::KeyType type;
};

static const KeyCode s_key_code[] =
{
    {(unsigned char)'1', ckKeyMgr::KEY_1},
    {(unsigned char)'2', ckKeyMgr::KEY_2},
    {(unsigned char)'3', ckKeyMgr::KEY_3},
    {(unsigned char)'4', ckKeyMgr::KEY_4},
    {(unsigned char)'5', ckKeyMgr::KEY_5},
    {(unsigned char)'6', ckKeyMgr::KEY_6},
    {(unsigned char)'7', ckKeyMgr::KEY_7},
    {(unsigned char)'8', ckKeyMgr::KEY_8},
    {(unsigned char)'9', ckKeyMgr::KEY_9},
    {(unsigned char)'0', ckKeyMgr::KEY_0},
    {(unsigned char)'*', ckKeyMgr::KEY_MULTIPLY},
    {(unsigned char)'+', ckKeyMgr::KEY_ADD},
    {(unsigned char)'-', ckKeyMgr::KEY_SUBTRACT},
    {(unsigned char)'.', ckKeyMgr::KEY_DECIMAL},
    {(unsigned char)'/', ckKeyMgr::KEY_DIVIDE},
    {(unsigned char)' ', ckKeyMgr::KEY_SPACE},
    {(unsigned char)'\n', ckKeyMgr::KEY_ENTER},
    {(unsigned char)'\t', ckKeyMgr::KEY_TAB},
    {0x1b, ckKeyMgr::KEY_ESCAPE},
};

static const size_t s_key_code_count = sizeof(s_key_code) / sizeof(s_key_code[0]);

static const AlphaKeyCode s_alpha_key_code[] =
{
    {'a', 'A', ckKeyMgr::KEY_A},
    {'b', 'B', ckKeyMgr::KEY_B},
    {'c', 'C', ckKeyMgr::KEY_C},
    {'d', 'D', ckKeyMgr::KEY_D},
    {'e', 'E', ckKeyMgr::KEY_E},
    {'f', 'F', ckKeyMgr::KEY_F},
    {'g', 'G', ckKeyMgr::KEY_G},
    {'h', 'H', ckKeyMgr::KEY_H},
    {'i', 'I', ckKeyMgr::KEY_I},
    {'j', 'J', ckKeyMgr::KEY_J},
    {'k', 'K', ckKeyMgr::KEY_K},
    {'l', 'L', ckKeyMgr::KEY_L},
    {'m', 'M', ckKeyMgr::KEY_M},
    {'n', 'N', ckKeyMgr::KEY_N},
    {'o', 'O', ckKeyMgr::KEY_O},
    {'p', 'P', ckKeyMgr::KEY_P},
    {'q', 'Q', ckKeyMgr::KEY_Q},
    {'r', 'R', ckKeyMgr::KEY_R},
    {'s', 'S', ckKeyMgr::KEY_S},
    {'t', 'T', ckKeyMgr::KEY_T},
    {'u', 'U', ckKeyMgr::KEY_U},
    {'v', 'V', ckKeyMgr::KEY_V},
    {'w', 'W', ckKeyMgr::KEY_W},
    {'x', 'X', ckKeyMgr::KEY_X},
    {'y', 'Y', ckKeyMgr::KEY_Y},
    {'z', 'Z', ckKeyMgr::KEY_Z},
};

static const size_t s_alpha_key_code_count = sizeof(s_alpha_key_code) / sizeof(s_alpha_key_code[0]);

static const SpecialKeyCode s_special_key_code[] =
{
    {GLUT_KEY_F1, ckKeyMgr::KEY_F1},
    {GLUT_KEY_F2, ckKeyMgr::KEY_F2},
    {GLUT_KEY_F3, ckKeyMgr::KEY_F3},
    {GLUT_KEY_F4, ckKeyMgr::KEY_F4},
    {GLUT_KEY_F5, ckKeyMgr::KEY_F5},
    {GLUT_KEY_F6, ckKeyMgr::KEY_F6},
    {GLUT_KEY_F7, ckKeyMgr::KEY_F7},
    {GLUT_KEY_F8, ckKeyMgr::KEY_F8},
    {GLUT_KEY_F9, ckKeyMgr::KEY_F9},
    {GLUT_KEY_F10, ckKeyMgr::KEY_F10},
    {GLUT_KEY_F11, ckKeyMgr::KEY_F11},
    {GLUT_KEY_F12, ckKeyMgr::KEY_F12},
    {GLUT_KEY_LEFT, ckKeyMgr::KEY_LEFT},
    {GLUT_KEY_UP, ckKeyMgr::KEY_UP},
    {GLUT_KEY_RIGHT, ckKeyMgr::KEY_RIGHT},
    {GLUT_KEY_DOWN, ckKeyMgr::KEY_DOWN},
    {GLUT_KEY_PAGE_UP, ckKeyMgr::KEY_PAGEUP},
    {GLUT_KEY_PAGE_DOWN, ckKeyMgr::KEY_PAGEDOWN},
    {GLUT_KEY_HOME, ckKeyMgr::KEY_HOME},
    {GLUT_KEY_END, ckKeyMgr::KEY_END},
    {GLUT_KEY_INSERT, ckKeyMgr::KEY_INSERT},
};

static const size_t s_special_key_code_count = sizeof(s_special_key_code) / sizeof(s_special_key_code[0]);

static ckKeyMgr::KeyType s_key = ckKeyMgr::KEY_NONE;
static bool s_last_shift_mask = false;
static bool s_last_alt_mask = false;
static bool s_last_ctrl_mask = false;


static ckKeyMgr::KeyState s_mouse_button_state = ckKeyMgr::STATE_UP;
static ckKeyMgr::KeyType s_mouse_button = ckKeyMgr::KEY_NONE;
static s16 s_mouse_x = 0;
static s16 s_mouse_y = 0;
static bool s_is_mouse_moved = false;


static void destroyFramebuffer()
{
    // TODO
}


static void onDisplay()
{
    // TODO
}


static bool createFramebuffer(u16 new_width, u16 new_height)
{
    glutInit(&s_argc, s_argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(s_framebuffer_width, s_framebuffer_height);
    glutCreateWindow(s_app_name);
    glutDisplayFunc(onDisplay);

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


static void onIdle()
{
    if (s_key_event_handler != NULL)
    {
        if (s_last_shift_mask)
            //(*s_key_event_handler)(ckKeyMgr::KEY_SHIFT, ckKeyMgr::STATE_DOWN);
            (*s_key_event_handler)(ckKeyMgr::KEY_SHIFT, true);
        if (s_last_alt_mask)
            //(*s_key_event_handler)(ckKeyMgr::KEY_ALT, ckKeyMgr::STATE_DOWN);
            (*s_key_event_handler)(ckKeyMgr::KEY_ALT, true);
        if (s_last_ctrl_mask)
            //(*s_key_event_handler)(ckKeyMgr::KEY_CTRL, ckKeyMgr::STATE_DOWN);
            (*s_key_event_handler)(ckKeyMgr::KEY_CTRL, true);
        if (s_key != ckKeyMgr::KEY_NONE)
            //(*s_key_event_handler)(s_key, ckKeyMgr::STATE_DOWN);
            (*s_key_event_handler)(s_key, true);
        if (s_mouse_button != ckKeyMgr::KEY_NONE)
        {
            //(*s_key_event_handler)(s_mouse_button, s_mouse_button_state);
            bool is_down = s_mouse_button_state == ckKeyMgr::STATE_DOWN;
            (*s_key_event_handler)(s_mouse_button, is_down);
            s_mouse_button = ckKeyMgr::KEY_NONE;
        }
    }
    if (s_mouse_event_handler != NULL && s_is_mouse_moved)
    {
        (*s_mouse_event_handler)(s_mouse_x, s_mouse_y);
        s_is_mouse_moved = false;
    }

    s_update_func();

    if (s_key_event_handler != NULL)
    {
        if (s_key != ckKeyMgr::KEY_NONE)
        {
            //(*s_key_event_handler)(s_key, ckKeyMgr::STATE_UP);
            (*s_key_event_handler)(s_key, false);
            s_key = ckKeyMgr::KEY_NONE;
        }
        if (s_last_ctrl_mask)
        {
            //(*s_key_event_handler)(ckKeyMgr::KEY_CTRL, ckKeyMgr::STATE_UP);
            (*s_key_event_handler)(ckKeyMgr::KEY_CTRL, false);
            s_last_ctrl_mask = false;
        }
        if (s_last_alt_mask)
        {
            //(*s_key_event_handler)(ckKeyMgr::KEY_ALT, ckKeyMgr::STATE_UP);
            (*s_key_event_handler)(ckKeyMgr::KEY_ALT, false);
            s_last_alt_mask = false;
        }
        if (s_last_shift_mask)
        {
            //(*s_key_event_handler)(ckKeyMgr::KEY_SHIFT, ckKeyMgr::STATE_UP);
            (*s_key_event_handler)(ckKeyMgr::KEY_SHIFT, false);
            s_last_shift_mask = false;
        }
    }
}


static void onKeyboard(unsigned char key, int x, int y)
{
    int m = glutGetModifiers();
    s_last_shift_mask = (m & GLUT_ACTIVE_SHIFT) != 0;
    s_last_ctrl_mask = (m & GLUT_ACTIVE_CTRL) != 0;
    s_last_alt_mask = (m & GLUT_ACTIVE_ALT) != 0;

    for (size_t i = 0; i < s_key_code_count; i++)
        if (s_key_code[i].key == key)
        {
            s_key = s_key_code[i].type;
            break;
        }

    if (s_key == ckKeyMgr::KEY_NONE)
    {
        for (size_t i = 0; i < s_alpha_key_code_count; i++)
            if (s_alpha_key_code[i].keyL == key || s_alpha_key_code[i].keyC == key)
            {
                s_key = s_alpha_key_code[i].type;
                break;
            }
    }
}


static void onSpecial(int key, int x, int y)
{
    int m = glutGetModifiers();
    s_last_shift_mask = (m & GLUT_ACTIVE_SHIFT) != 0;
    s_last_ctrl_mask = (m & GLUT_ACTIVE_CTRL) != 0;
    s_last_alt_mask = (m & GLUT_ACTIVE_ALT) != 0;

    for (size_t i = 0; i < s_special_key_code_count; i++)
        if (s_special_key_code[i].key == key)
        {
            s_key = s_special_key_code[i].type;
            break;
        }
}


static void onMouse(int button, int state, int x, int y)
{
    int m = glutGetModifiers();
    s_last_shift_mask = (m & GLUT_ACTIVE_SHIFT) != 0;
    s_last_ctrl_mask = (m & GLUT_ACTIVE_CTRL) != 0;
    s_last_alt_mask = (m & GLUT_ACTIVE_ALT) != 0;

    s_mouse_button_state = (state == GLUT_DOWN) ? ckKeyMgr::STATE_DOWN : ckKeyMgr::STATE_UP;

    if (button == GLUT_LEFT_BUTTON)
        s_mouse_button = ckKeyMgr::KEY_LBUTTON;
    else if (button == GLUT_MIDDLE_BUTTON)
        s_mouse_button = ckKeyMgr::KEY_MBUTTON;
    else if (button == GLUT_RIGHT_BUTTON)
        s_mouse_button = ckKeyMgr::KEY_RBUTTON;
    else if (button == 3)
        s_mouse_button = ckKeyMgr::KEY_WHEELUP;
    else if (button == 4)
        s_mouse_button = ckKeyMgr::KEY_WHEELDOWN;
}


static void onMotion(int x, int y)
{
    s_is_mouse_moved = true;
    s_mouse_x = (s16)x;
    s_mouse_y = (s16)y;
}


static void onPassiveMotion(int x, int y)
{
    s_is_mouse_moved = true;
    s_mouse_x = (s16)x;
    s_mouse_y = (s16)y;
}


void ckLowLevelAPI::startApplication(bool (*update_func)(void))
{
    s_update_func = update_func;

    glutKeyboardFunc(onKeyboard);
    glutSpecialFunc(onSpecial);
    glutMouseFunc(onMouse);
    glutMotionFunc(onMotion);
    glutPassiveMotionFunc(onPassiveMotion);
    glutIdleFunc(onIdle);
    glutMainLoop();
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
    // TODO
}


bool ckLowLevelAPI::isFramebufferSizeChanged()
{
    return s_is_framebuffer_size_changed;
}


void ckLowLevelAPI::swapFramebuffer()
{
    glutSwapBuffers();
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
    // TODO
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

        // TODO
    }
}


u64 ckLowLevelAPI::getUsecTime()
{
    static u64 s_start_time;
    static bool s_is_first = true;

    struct timeval time;
    struct timezone tz;

    if (s_is_first)
    {
        s_is_first = false;

        gettimeofday(&time, &tz);
        s_start_time = time.tv_sec * 1000000 + time.tv_usec;
    }

    gettimeofday(&time, &tz);
    u64 cur_time = time.tv_sec * 1000000 + time.tv_usec;

    return cur_time - s_start_time;
}


void ckLowLevelAPI::sleepUsec(u64 usec)
{
    usleep(usec);
}


void ckLowLevelAPI::exit(s32 status)
{
    ::exit(status);
}


void ckLowLevelAPI::error(const char* msg)
{
    printf(msg);
    printf("\n");

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


struct ThreadStartFuncAndUserParam
{
    void (*start_func)(void*);
    void* user_param;
};


static void* threadStartFunc(void* user_param)
{
    ThreadStartFuncAndUserParam* func_and_param = static_cast<ThreadStartFuncAndUserParam*>(user_param);

    func_and_param->start_func(func_and_param->user_param);

    ckLowLevelAPI::free(user_param);

    return NULL;
}


void* ckLowLevelAPI::newThread(void (*start_func)(void*), void* user_param)
{
    static pthread_mutex_t s_mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&s_mutex);

    void* thread_handler = malloc(sizeof(pthread_t));

    ThreadStartFuncAndUserParam* func_and_param = static_cast<ThreadStartFuncAndUserParam*>(malloc(sizeof(ThreadStartFuncAndUserParam)));
    func_and_param->start_func = start_func;
    func_and_param->user_param = user_param;

    if (pthread_create(static_cast<pthread_t*>(thread_handler), NULL, threadStartFunc, func_and_param))
    {
        free(thread_handler);
        free(func_and_param);

        thread_handler = NULL;
    }

    pthread_mutex_unlock(&s_mutex);

    return thread_handler;
}


void ckLowLevelAPI::deleteThread(void* thread_handler)
{
    free(thread_handler);
}


void ckLowLevelAPI::joinThread(void* thread_handler)
{
    pthread_join(*static_cast<pthread_t*>(thread_handler), NULL);
}


void* ckLowLevelAPI::newMutex()
{
    void* mutex_handler = malloc(sizeof(pthread_mutex_t));

    if (pthread_mutex_init(static_cast<pthread_mutex_t*>(mutex_handler), NULL))
    {
        free(mutex_handler);

        return NULL;
    }
    else
    {
        return mutex_handler;
    }
}


void ckLowLevelAPI::deleteMutex(void* mutex_handler)
{
    pthread_mutex_destroy(static_cast<pthread_mutex_t*>(mutex_handler));

    free(mutex_handler);
}


void ckLowLevelAPI::lockMutex(void* mutex_handler)
{
    pthread_mutex_lock(static_cast<pthread_mutex_t*>(mutex_handler));
}


void ckLowLevelAPI::unlockMutex(void* mutex_handler)
{
    pthread_mutex_unlock(static_cast<pthread_mutex_t*>(mutex_handler));
}


void ckLowLevelAPI::setInitialDirectory(s32 argc, char** argv)
{
    s_argc = argc;
    s_argv = argv;

    if (argc > 0)
    {
        chdir(dirname(argv[0]));
    }
}


void ckLowLevelAPI::getWindowsFontDirectory(char* buf, u32 buf_size) {}


#endif // CK_MACOSX
