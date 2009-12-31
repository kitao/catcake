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


#ifdef CK_IPHONE


#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <pthread.h>

#include "ck_iphone_bridge.h"

#include "ck_low_level_api.h"

#include "ck_sys_all.h" // for ckSysMgr::SysFlag
#include "ck_task_all.h" // for ckTaskMgr::getAimFPS and ckTaskMgr::resetFrameSkip
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


static void destroyFramebuffer()
{
    return ckIPhoneBridge::destroyFramebuffer();
}


static bool createFramebuffer(u16 new_width, u16 new_height)
{
    s_framebuffer_width = ckIPhoneBridge::getFramebufferWidth();
    s_framebuffer_height = ckIPhoneBridge::getFramebufferHeight();

    return ckIPhoneBridge::createFramebuffer();
}


bool ckLowLevelAPI::createApplication(const char* title, u16 width, u16 height, u16 sys_flag)
{
    s_app_name = title;
    s_framebuffer_width = width;
    s_framebuffer_height = height;
    s_sys_flag = sys_flag;
    s_is_framebuffer_size_changed = false;
    s_is_fullscreen = true;
    s_is_mouse_visible = false;

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


static bool (*s_update_func)(void);


void ckLowLevelAPI::startApplication(bool (*update_func)(void))
{
    s_update_func = update_func;

    ckIPhoneBridge::startApplication();
}


void ckIPhoneBridge::updateApplication()
{
    int x, y;

    ckIPhoneBridge::getMousePos(&x, &y);
    (*s_mouse_event_handler)(static_cast<s16>(x), static_cast<s16>(y));

    ckIPhoneBridge::getInnerMousePos(&x, &y);
    (*s_extra_event_handler)(0, static_cast<r32>(x));
    (*s_extra_event_handler)(1, static_cast<r32>(y));

    ckIPhoneBridge::getOuterMousePos(&x, &y);
    (*s_extra_event_handler)(2, static_cast<r32>(x));
    (*s_extra_event_handler)(3, static_cast<r32>(y));

    (*s_extra_event_handler)(4, static_cast<r32>(ckIPhoneBridge::getDeviceOrientation()));

    float ax, ay, az;

    ckIPhoneBridge::getAcceleraion(&ax, &ay, &az);
    (*s_extra_event_handler)(5, static_cast<r32>(ax));
    (*s_extra_event_handler)(6, static_cast<r32>(ay));
    (*s_extra_event_handler)(7, static_cast<r32>(az));

    if (ckTaskMgr::getAimFPS() <= 30)
    {
        ckTaskMgr::resetFrameSkip();
    }

    while (!(*s_update_func)()) {}
}


void ckIPhoneBridge::setMouseState(int button, bool is_on)
{
    if (button == 0)
    {
        (*s_key_event_handler)(ckKeyMgr::KEY_LBUTTON, is_on);
    }
    else
    {
        (*s_key_event_handler)(ckKeyMgr::KEY_EXT_00, is_on);
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


void ckLowLevelAPI::updateFramebufferSize() {}


bool ckLowLevelAPI::isFramebufferSizeChanged()
{
    return s_is_framebuffer_size_changed;
}


void ckLowLevelAPI::swapFramebuffer()
{
    ckIPhoneBridge::swapFramebuffer();
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


void ckLowLevelAPI::setMousePos(s16 mouse_x, s16 mouse_y) {}


bool ckLowLevelAPI::isMouseVisible()
{
    return s_is_mouse_visible;
}


void ckLowLevelAPI::setMouseVisible(bool is_visible) {}


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
    exit(status);
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
    if (argc > 0)
    {
        chdir(dirname(argv[0]));
    }
}


void ckLowLevelAPI::getWindowsFontDirectory(char* buf, u32 buf_size) {}


#endif // CK_IPHONE
