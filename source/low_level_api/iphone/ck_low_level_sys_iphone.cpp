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


#ifdef PG_IPHONE


#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <pthread.h>

#include "pg_iphone_bridge.h"

#include "pg_low_level_api.h"

#include "pg_sys_all.h" // for pgSysMgr::SysFlag and pgSysMgr::sprintf
#include "pg_task_all.h" // for pgTaskMgr::getAimFPS and pgTaskMgr::resetFrameSkip
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


static void destroyFramebuffer()
{
    return pgIPhoneBridge::destroyFramebuffer();
}


static bool createFramebuffer(u16 new_width, u16 new_height)
{
    s_framebuffer_width = pgIPhoneBridge::getFramebufferWidth();
    s_framebuffer_height = pgIPhoneBridge::getFramebufferHeight();

    return pgIPhoneBridge::createFramebuffer();
}


bool pgLowLevelAPI::createApplication(const char* title, u16 width, u16 height, u16 sys_flag)
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

    setupShaderAPI((sys_flag & pgSysMgr::FLAG_DISABLE_SHADER) ? false : true);

    return true;
}


void pgLowLevelAPI::destroyApplication()
{
    destroyFramebuffer();
}


static bool (*s_update_func)(void);


void pgLowLevelAPI::startApplication(bool (*update_func)(void))
{
    s_update_func = update_func;

    pgIPhoneBridge::startApplication();
}


void pgIPhoneBridge::updateApplication()
{
    int x, y;

    pgIPhoneBridge::getMousePos(&x, &y);
    (*s_mouse_event_handler)(static_cast<s16>(x), static_cast<s16>(y));

    pgIPhoneBridge::getInnerMousePos(&x, &y);
    (*s_extra_event_handler)(0, static_cast<r32>(x));
    (*s_extra_event_handler)(1, static_cast<r32>(y));

    pgIPhoneBridge::getOuterMousePos(&x, &y);
    (*s_extra_event_handler)(2, static_cast<r32>(x));
    (*s_extra_event_handler)(3, static_cast<r32>(y));

    (*s_extra_event_handler)(4, static_cast<r32>(pgIPhoneBridge::getDeviceOrientation()));

    float ax, ay, az;

    pgIPhoneBridge::getAcceleraion(&ax, &ay, &az);
    (*s_extra_event_handler)(5, static_cast<r32>(ax));
    (*s_extra_event_handler)(6, static_cast<r32>(ay));
    (*s_extra_event_handler)(7, static_cast<r32>(az));

    if (pgTaskMgr::getAimFPS() <= 30)
    {
        pgTaskMgr::resetFrameSkip();
    }

    while (!(*s_update_func)()) {}
}


void pgIPhoneBridge::setMouseState(int button, bool is_on)
{
    if (button == 0)
    {
        (*s_key_event_handler)(pgKeyMgr::KEY_LBUTTON, is_on);
    }
    else
    {
        (*s_key_event_handler)(pgKeyMgr::KEY_EXT_00, is_on);
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


void pgLowLevelAPI::updateFramebufferSize() {}


bool pgLowLevelAPI::isFramebufferSizeChanged()
{
    return s_is_framebuffer_size_changed;
}


void pgLowLevelAPI::swapFramebuffer()
{
    pgIPhoneBridge::swapFramebuffer();
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


void pgLowLevelAPI::setMousePos(s16 mouse_x, s16 mouse_y) {}


bool pgLowLevelAPI::isMouseVisible()
{
    return s_is_mouse_visible;
}


void pgLowLevelAPI::setMouseVisible(bool is_visible) {}


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
    exit(status);
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
    if (argc > 0)
    {
        chdir(dirname(argv[0]));
    }
}


void pgLowLevelAPI::getWindowsFontDirectory(char* buf, u32 buf_size)
{
    // TODO: Error
}


#endif // PG_IPHONE
