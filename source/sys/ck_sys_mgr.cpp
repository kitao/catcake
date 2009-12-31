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


#include <stdarg.h>

#include "ck_sys_all.h"

#include "ck_mem_all.h"
#include "ck_key_all.h"
#include "ck_draw_all.h"
#include "ck_low_level_api.h"
#include "ck_private_macro.h"


ckSysMgr* ckSysMgr::m_instance = NULL;


void ckSysMgr::printf(const char* str, ...)
{
    if (!str)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    char buf[256];
    CK_VSPRINTF(buf, 256, str);

    ckLowLevelAPI::printf(buf);
}


void ckSysMgr::wprintf(const wchar_t* str, ...)
{
    if (!str)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    wchar_t buf[256];
    CK_VSWPRINTF(buf, 256, str);

    ckLowLevelAPI::wprintf(buf);
}


void ckSysMgr::sprintf(char* buf, u32 buf_size, const char* str, ...)
{
    if (!buf || buf_size == 0 || !str)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    CK_VSPRINTF(buf, buf_size, str);
}


void ckSysMgr::swprintf(wchar_t* buf, u32 buf_size, const wchar_t* str, ...)
{
    if (!buf || buf_size == 0 || !str)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    CK_VSWPRINTF(buf, buf_size, str);
}


void* ckSysMgr::openFile(const char* filename, FileMode file_mode)
{
    if (!filename)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    void* fh = ckLowLevelAPI::openFile(filename, static_cast<ckLowLevelAPI::FileMode>(file_mode));

    if (!fh)
    {
        ckThrow(ExceptionCannotOpenFile);
    }

    return fh;
}


u32 ckSysMgr::getFileSize(void* file_handler)
{
    if (!file_handler)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    s32 file_size = ckLowLevelAPI::getFileSize(file_handler);

    if (file_size < 0)
    {
        ckThrow(ExceptionCannotReadFile);
    }

    return file_size;
}


void ckSysMgr::readFile(void* buf, u32 offset, u32 size, void* file_handler)
{
    if (!buf || size == 0 || !file_handler)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (!ckLowLevelAPI::readFile(buf, offset, size, file_handler))
    {
        ckThrow(ExceptionCannotReadFile);
    }
}


void ckSysMgr::writeFile(u32 offset, const void* buf, u32 size, void* file_handler)
{
    if (!buf || size == 0 || !file_handler)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (!ckLowLevelAPI::writeFile(offset, buf, size, file_handler))
    {
        ckThrow(ExceptionCannotWriteFile);
    }
}


void ckSysMgr::closeFile(void* file_handler)
{
    if (!file_handler)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckLowLevelAPI::closeFile(file_handler);
}


u64 ckSysMgr::getUsecTime()
{
    return ckLowLevelAPI::getUsecTime();
}


void ckSysMgr::sleepUsec(u64 usec)
{
    ckLowLevelAPI::sleepUsec(usec);
}


void* ckSysMgr::newThread(void (*start_func)(void*), void* user_param)
{
    if (!start_func)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    void* th = ckLowLevelAPI::newThread(start_func, user_param);

    if (!th)
    {
        ckThrow(ExceptionCannotCreateMutex);
    }

    return th;
}


void ckSysMgr::deleteThread(void* thread_handler)
{
    if (!thread_handler)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckLowLevelAPI::deleteThread(thread_handler);
}


void ckSysMgr::joinThread(void* thread_handler)
{
    if (!thread_handler)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckLowLevelAPI::joinThread(thread_handler);
}


void* ckSysMgr::newMutex()
{
    void* mh = ckLowLevelAPI::newMutex();

    if (!mh)
    {
        ckThrow(ExceptionCannotCreateMutex);
    }

    return mh;
}


void ckSysMgr::deleteMutex(void* mutex_handler)
{
    if (!mutex_handler)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckLowLevelAPI::deleteMutex(mutex_handler);
}


void ckSysMgr::lockMutex(void* mutex_handler)
{
    if (!mutex_handler)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckLowLevelAPI::lockMutex(mutex_handler);
}


void ckSysMgr::unlockMutex(void* mutex_handler)
{
    if (!mutex_handler)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckLowLevelAPI::unlockMutex(mutex_handler);
}


CK_DEFINE_MANAGER_IS_CREATED(ckSysMgr)


void ckSysMgr::createAfterMem(const char* title, u16 width, u16 height, u16 sys_flag)
{
    if (!title || width == 0 || height == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    destroyBeforeMem();

    m_instance = ckNew(ckSysMgr)(title, width, height, sys_flag);
}


CK_DEFINE_MANAGER_DESTROY(ckSysMgr, BeforeMem)


u16 ckSysMgr::getFramebufferWidth()
{
    instance();

    u16 width = ckLowLevelAPI::getFramebufferWidth();

    return (width > 0) ? width : 1;
}


u16 ckSysMgr::getFramebufferHeight()
{
    instance();

    u16 height = ckLowLevelAPI::getFramebufferHeight();

    return (height > 0) ? height : 1;
}


bool ckSysMgr::isFramebufferSizeChanged()
{
    instance();

    return ckLowLevelAPI::isFramebufferSizeChanged();
}


bool ckSysMgr::isFullScreen()
{
    instance();

    return ckLowLevelAPI::isFullScreen();
}


void ckSysMgr::toggleFullScreen(u16 width, u16 height)
{
    instance();

    if (width == 0 || height == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (ckDrawMgr::isCreated())
    {
        ckDrawMgr::deleteAllVramObjForSystem();
    }

    if (!ckLowLevelAPI::toggleFullScreen(width, height))
    {
        ckThrow(ExceptionCreateFramebufferFailed);
    }

    ckLowLevelAPI::updateFramebufferSize();
    ckLowLevelAPI::resetDrawState();

    if (ckKeyMgr::isCreated())
    {
        ckKeyMgr::resetKeyStateForSystem();
    }
}


void ckSysMgr::updateForSystem()
{
    instance();

    ckLowLevelAPI::updateFramebufferSize();
}


void ckSysMgr::readLittleEndianForSystem(void* dest, const void* src, u32 size)
{
    ckLowLevelAPI::readLittleEndian(dest, src, size);
}


void ckSysMgr::writeLittleEndianForSystem(void* dest, const void* src, u32 size)
{
    ckLowLevelAPI::writeLittleEndian(dest, src, size);
}


void ckSysMgr::setInitialDirectoryForSystem(s32 argc, char** argv)
{
    if (argc < 0 || (argc > 0 && !argv))
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckLowLevelAPI::setInitialDirectory(argc, argv);
}


ckSysMgr::ckSysMgr(const char* title, u16 width, u16 height, u16 sys_flag)
{
    if (!ckLowLevelAPI::createApplication(title, width, height, sys_flag))
    {
        ckThrow(ExceptionCreateFramebufferFailed);
    }
}


ckSysMgr::~ckSysMgr()
{
    ckLowLevelAPI::destroyApplication();
}


CK_DEFINE_OPERATOR_EQUAL(ckSysMgr)


CK_DEFINE_MANAGER_INSTANCE(ckSysMgr)
