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


#ifdef PG_MACOSX

#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <pthread.h>
#include <GLUT/glut.h>

#include "pg_low_level_api.h"

#include "pg_sys_all.h" // for pgSysMgr::SysFlag and pgSysMgr::sprintf
#include "pg_key_all.h" // for pgKeyMgr::KeyType


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

static s32 s_argc;
static char** s_argv;
static bool (*s_update_func)(void);

struct KeyCode
{
    unsigned char key;
    pgKeyMgr::KeyType type;
};

struct AlphaKeyCode
{
    unsigned char keyL;
    unsigned char keyC;
    pgKeyMgr::KeyType type;
};

struct SpecialKeyCode
{
    int key;
    pgKeyMgr::KeyType type;
};

static const KeyCode s_key_code[] =
{
    {(unsigned char)'1', pgKeyMgr::KEY_1},
    {(unsigned char)'2', pgKeyMgr::KEY_2},
    {(unsigned char)'3', pgKeyMgr::KEY_3},
    {(unsigned char)'4', pgKeyMgr::KEY_4},
    {(unsigned char)'5', pgKeyMgr::KEY_5},
    {(unsigned char)'6', pgKeyMgr::KEY_6},
    {(unsigned char)'7', pgKeyMgr::KEY_7},
    {(unsigned char)'8', pgKeyMgr::KEY_8},
    {(unsigned char)'9', pgKeyMgr::KEY_9},
    {(unsigned char)'0', pgKeyMgr::KEY_0},
    {(unsigned char)'*', pgKeyMgr::KEY_MULTIPLY},
    {(unsigned char)'+', pgKeyMgr::KEY_ADD},
    {(unsigned char)'-', pgKeyMgr::KEY_SUBTRACT},
    {(unsigned char)'.', pgKeyMgr::KEY_DECIMAL},
    {(unsigned char)'/', pgKeyMgr::KEY_DIVIDE},
    {(unsigned char)' ', pgKeyMgr::KEY_SPACE},
    {(unsigned char)'\n', pgKeyMgr::KEY_ENTER},
    {(unsigned char)'\t', pgKeyMgr::KEY_TAB},
    {0x1b, pgKeyMgr::KEY_ESCAPE},
};

static const size_t s_key_code_count = sizeof(s_key_code) / sizeof(s_key_code[0]);

static const AlphaKeyCode s_alpha_key_code[] =
{
    {'a', 'A', pgKeyMgr::KEY_A},
    {'b', 'B', pgKeyMgr::KEY_B},
    {'c', 'C', pgKeyMgr::KEY_C},
    {'d', 'D', pgKeyMgr::KEY_D},
    {'e', 'E', pgKeyMgr::KEY_E},
    {'f', 'F', pgKeyMgr::KEY_F},
    {'g', 'G', pgKeyMgr::KEY_G},
    {'h', 'H', pgKeyMgr::KEY_H},
    {'i', 'I', pgKeyMgr::KEY_I},
    {'j', 'J', pgKeyMgr::KEY_J},
    {'k', 'K', pgKeyMgr::KEY_K},
    {'l', 'L', pgKeyMgr::KEY_L},
    {'m', 'M', pgKeyMgr::KEY_M},
    {'n', 'N', pgKeyMgr::KEY_N},
    {'o', 'O', pgKeyMgr::KEY_O},
    {'p', 'P', pgKeyMgr::KEY_P},
    {'q', 'Q', pgKeyMgr::KEY_Q},
    {'r', 'R', pgKeyMgr::KEY_R},
    {'s', 'S', pgKeyMgr::KEY_S},
    {'t', 'T', pgKeyMgr::KEY_T},
    {'u', 'U', pgKeyMgr::KEY_U},
    {'v', 'V', pgKeyMgr::KEY_V},
    {'w', 'W', pgKeyMgr::KEY_W},
    {'x', 'X', pgKeyMgr::KEY_X},
    {'y', 'Y', pgKeyMgr::KEY_Y},
    {'z', 'Z', pgKeyMgr::KEY_Z},
};

static const size_t s_alpha_key_code_count = sizeof(s_alpha_key_code) / sizeof(s_alpha_key_code[0]);

static const SpecialKeyCode s_special_key_code[] =
{
    {GLUT_KEY_F1, pgKeyMgr::KEY_F1},
    {GLUT_KEY_F2, pgKeyMgr::KEY_F2},
    {GLUT_KEY_F3, pgKeyMgr::KEY_F3},
    {GLUT_KEY_F4, pgKeyMgr::KEY_F4},
    {GLUT_KEY_F5, pgKeyMgr::KEY_F5},
    {GLUT_KEY_F6, pgKeyMgr::KEY_F6},
    {GLUT_KEY_F7, pgKeyMgr::KEY_F7},
    {GLUT_KEY_F8, pgKeyMgr::KEY_F8},
    {GLUT_KEY_F9, pgKeyMgr::KEY_F9},
    {GLUT_KEY_F10, pgKeyMgr::KEY_F10},
    {GLUT_KEY_F11, pgKeyMgr::KEY_F11},
    {GLUT_KEY_F12, pgKeyMgr::KEY_F12},
    {GLUT_KEY_LEFT, pgKeyMgr::KEY_LEFT},
    {GLUT_KEY_UP, pgKeyMgr::KEY_UP},
    {GLUT_KEY_RIGHT, pgKeyMgr::KEY_RIGHT},
    {GLUT_KEY_DOWN, pgKeyMgr::KEY_DOWN},
    {GLUT_KEY_PAGE_UP, pgKeyMgr::KEY_PAGEUP},
    {GLUT_KEY_PAGE_DOWN, pgKeyMgr::KEY_PAGEDOWN},
    {GLUT_KEY_HOME, pgKeyMgr::KEY_HOME},
    {GLUT_KEY_END, pgKeyMgr::KEY_END},
    {GLUT_KEY_INSERT, pgKeyMgr::KEY_INSERT},
};

static const size_t s_special_key_code_count = sizeof(s_special_key_code) / sizeof(s_special_key_code[0]);

static pgKeyMgr::KeyType s_key = pgKeyMgr::KEY_NONE;
static bool s_last_shift_mask = false;
static bool s_last_alt_mask = false;
static bool s_last_ctrl_mask = false;


static pgKeyMgr::KeyState s_mouse_button_state = pgKeyMgr::STATE_UP;
static pgKeyMgr::KeyType s_mouse_button = pgKeyMgr::KEY_NONE;
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


static void onIdle()
{
    if (s_key_event_handler != NULL)
    {
        if (s_last_shift_mask)
            //(*s_key_event_handler)(pgKeyMgr::KEY_SHIFT, pgKeyMgr::STATE_DOWN);
            (*s_key_event_handler)(pgKeyMgr::KEY_SHIFT, true);
        if (s_last_alt_mask)
            //(*s_key_event_handler)(pgKeyMgr::KEY_ALT, pgKeyMgr::STATE_DOWN);
            (*s_key_event_handler)(pgKeyMgr::KEY_ALT, true);
        if (s_last_ctrl_mask)
            //(*s_key_event_handler)(pgKeyMgr::KEY_CTRL, pgKeyMgr::STATE_DOWN);
            (*s_key_event_handler)(pgKeyMgr::KEY_CTRL, true);
        if (s_key != pgKeyMgr::KEY_NONE)
            //(*s_key_event_handler)(s_key, pgKeyMgr::STATE_DOWN);
            (*s_key_event_handler)(s_key, true);
        if (s_mouse_button != pgKeyMgr::KEY_NONE)
        {
            //(*s_key_event_handler)(s_mouse_button, s_mouse_button_state);
            bool is_down = s_mouse_button_state == pgKeyMgr::STATE_DOWN;
            (*s_key_event_handler)(s_mouse_button, is_down);
            s_mouse_button = pgKeyMgr::KEY_NONE;
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
        if (s_key != pgKeyMgr::KEY_NONE)
        {
            //(*s_key_event_handler)(s_key, pgKeyMgr::STATE_UP);
            (*s_key_event_handler)(s_key, false);
            s_key = pgKeyMgr::KEY_NONE;
        }
        if (s_last_ctrl_mask)
        {
            //(*s_key_event_handler)(pgKeyMgr::KEY_CTRL, pgKeyMgr::STATE_UP);
            (*s_key_event_handler)(pgKeyMgr::KEY_CTRL, false);
            s_last_ctrl_mask = false;
        }
        if (s_last_alt_mask)
        {
            //(*s_key_event_handler)(pgKeyMgr::KEY_ALT, pgKeyMgr::STATE_UP);
            (*s_key_event_handler)(pgKeyMgr::KEY_ALT, false);
            s_last_alt_mask = false;
        }
        if (s_last_shift_mask)
        {
            //(*s_key_event_handler)(pgKeyMgr::KEY_SHIFT, pgKeyMgr::STATE_UP);
            (*s_key_event_handler)(pgKeyMgr::KEY_SHIFT, false);
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

    if (s_key == pgKeyMgr::KEY_NONE)
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

    s_mouse_button_state = (state == GLUT_DOWN) ? pgKeyMgr::STATE_DOWN : pgKeyMgr::STATE_UP;

    if (button == GLUT_LEFT_BUTTON)
        s_mouse_button = pgKeyMgr::KEY_LBUTTON;
    else if (button == GLUT_MIDDLE_BUTTON)
        s_mouse_button = pgKeyMgr::KEY_MBUTTON;
    else if (button == GLUT_RIGHT_BUTTON)
        s_mouse_button = pgKeyMgr::KEY_RBUTTON;
    else if (button == 3)
        s_mouse_button = pgKeyMgr::KEY_WHEELUP;
    else if (button == 4)
        s_mouse_button = pgKeyMgr::KEY_WHEELDOWN;
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


void pgLowLevelAPI::startApplication(bool (*update_func)(void))
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
    // TODO
}


bool pgLowLevelAPI::isFramebufferSizeChanged()
{
    return s_is_framebuffer_size_changed;
}


void pgLowLevelAPI::swapFramebuffer()
{
    glutSwapBuffers();
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
    // TODO
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

        // TODO
    }
}


u64 pgLowLevelAPI::getUsecTime()
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


void pgLowLevelAPI::sleepUsec(u64 usec)
{
    usleep(usec);
}


void pgLowLevelAPI::exit(s32 status)
{
    ::exit(status);
}


void pgLowLevelAPI::error(const char* msg)
{
    printf(msg);
    printf("\n");

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


struct ThreadStartFuncAndUserParam
{
    void (*start_func)(void*);
    void* user_param;
};


static void* threadStartFunc(void* user_param)
{
    ThreadStartFuncAndUserParam* func_and_param = static_cast<ThreadStartFuncAndUserParam*>(user_param);

    func_and_param->start_func(func_and_param->user_param);

    pgLowLevelAPI::free(user_param);

    return NULL;
}


void* pgLowLevelAPI::newThread(void (*start_func)(void*), void* user_param)
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


void pgLowLevelAPI::deleteThread(void* thread_handler)
{
    free(thread_handler);
}


void pgLowLevelAPI::joinThread(void* thread_handler)
{
    pthread_join(*static_cast<pthread_t*>(thread_handler), NULL);
}


void* pgLowLevelAPI::newMutex()
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


void pgLowLevelAPI::deleteMutex(void* mutex_handler)
{
    pthread_mutex_destroy(static_cast<pthread_mutex_t*>(mutex_handler));

    free(mutex_handler);
}


void pgLowLevelAPI::lockMutex(void* mutex_handler)
{
    pthread_mutex_lock(static_cast<pthread_mutex_t*>(mutex_handler));
}


void pgLowLevelAPI::unlockMutex(void* mutex_handler)
{
    pthread_mutex_unlock(static_cast<pthread_mutex_t*>(mutex_handler));
}


void pgLowLevelAPI::setInitialDirectory(s32 argc, char** argv)
{
    s_argc = argc;
    s_argv = argv;

    if (argc > 0)
    {
        chdir(dirname(argv[0]));
    }
}


void pgLowLevelAPI::getWindowsFontDirectory(char* buf, u32 buf_size)
{
    // TODO: Error
}


#endif // PG_MACOSX
