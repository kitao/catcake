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


#include <stdarg.h>

#include "pg_sys_all.h"

#include "pg_mem_all.h"
#include "pg_key_all.h"
#include "pg_draw_all.h"
#include "pg_low_level_api.h"
#include "pg_private_macro.h"


pgSysMgr* pgSysMgr::m_instance = NULL;


void pgSysMgr::printf(const char* str, ...)
{
    if (!str)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    char buf[256];
    PG_VSPRINTF(buf, 256, str);

    pgLowLevelAPI::printf(buf);
}


void pgSysMgr::wprintf(const wchar_t* str, ...)
{
    if (!str)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    wchar_t buf[256];
    PG_VSWPRINTF(buf, 256, str);

    pgLowLevelAPI::wprintf(buf);
}


void pgSysMgr::sprintf(char* buf, u32 buf_size, const char* str, ...)
{
    if (!buf || buf_size == 0 || !str)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    PG_VSPRINTF(buf, buf_size, str);
}


void pgSysMgr::swprintf(wchar_t* buf, u32 buf_size, const wchar_t* str, ...)
{
    if (!buf || buf_size == 0 || !str)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    PG_VSWPRINTF(buf, buf_size, str);
}


void* pgSysMgr::openFile(const char* filename, FileMode file_mode)
{
    if (!filename)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    void* fh = pgLowLevelAPI::openFile(filename, static_cast<pgLowLevelAPI::FileMode>(file_mode));

    if (!fh)
    {
        pgThrow(ExceptionCannotOpenFile);
    }

    return fh;
}


u32 pgSysMgr::getFileSize(void* file_handler)
{
    if (!file_handler)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    s32 file_size = pgLowLevelAPI::getFileSize(file_handler);

    if (file_size < 0)
    {
        pgThrow(ExceptionCannotReadFile);
    }

    return file_size;
}


void pgSysMgr::readFile(void* buf, u32 offset, u32 size, void* file_handler)
{
    if (!buf || size == 0 || !file_handler)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (!pgLowLevelAPI::readFile(buf, offset, size, file_handler))
    {
        pgThrow(ExceptionCannotReadFile);
    }
}


void pgSysMgr::writeFile(u32 offset, const void* buf, u32 size, void* file_handler)
{
    if (!buf || size == 0 || !file_handler)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (!pgLowLevelAPI::writeFile(offset, buf, size, file_handler))
    {
        pgThrow(ExceptionCannotWriteFile);
    }
}


void pgSysMgr::closeFile(void* file_handler)
{
    if (!file_handler)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgLowLevelAPI::closeFile(file_handler);
}


u64 pgSysMgr::getUsecTime()
{
    return pgLowLevelAPI::getUsecTime();
}


void pgSysMgr::sleepUsec(u64 usec)
{
    pgLowLevelAPI::sleepUsec(usec);
}


void* pgSysMgr::newThread(void (*start_func)(void*), void* user_param)
{
    if (!start_func)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    void* th = pgLowLevelAPI::newThread(start_func, user_param);

    if (!th)
    {
        pgThrow(ExceptionCannotCreateMutex);
    }

    return th;
}


void pgSysMgr::deleteThread(void* thread_handler)
{
    if (!thread_handler)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgLowLevelAPI::deleteThread(thread_handler);
}


void pgSysMgr::joinThread(void* thread_handler)
{
    if (!thread_handler)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgLowLevelAPI::joinThread(thread_handler);
}


void* pgSysMgr::newMutex()
{
    void* mh = pgLowLevelAPI::newMutex();

    if (!mh)
    {
        pgThrow(ExceptionCannotCreateMutex);
    }

    return mh;
}


void pgSysMgr::deleteMutex(void* mutex_handler)
{
    if (!mutex_handler)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgLowLevelAPI::deleteMutex(mutex_handler);
}


void pgSysMgr::lockMutex(void* mutex_handler)
{
    if (!mutex_handler)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgLowLevelAPI::lockMutex(mutex_handler);
}


void pgSysMgr::unlockMutex(void* mutex_handler)
{
    if (!mutex_handler)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgLowLevelAPI::unlockMutex(mutex_handler);
}


PG_DEFINE_MANAGER_IS_CREATED(pgSysMgr)


void pgSysMgr::createAfterMem(const char* title, u16 width, u16 height, u16 sys_flag)
{
    if (!title || width == 0 || height == 0)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    destroyBeforeMem();

    m_instance = pgNew(pgSysMgr)(title, width, height, sys_flag);
}


PG_DEFINE_MANAGER_DESTROY(pgSysMgr, BeforeMem)


u16 pgSysMgr::getFramebufferWidth()
{
    instance();

    u16 width = pgLowLevelAPI::getFramebufferWidth();

    return (width > 0) ? width : 1;
}


u16 pgSysMgr::getFramebufferHeight()
{
    instance();

    u16 height = pgLowLevelAPI::getFramebufferHeight();

    return (height > 0) ? height : 1;
}


bool pgSysMgr::isFramebufferSizeChanged()
{
    instance();

    return pgLowLevelAPI::isFramebufferSizeChanged();
}


bool pgSysMgr::isFullScreen()
{
    instance();

    return pgLowLevelAPI::isFullScreen();
}


void pgSysMgr::toggleFullScreen(u16 width, u16 height)
{
    instance();

    if (width == 0 || height == 0)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (pgDrawMgr::isCreated())
    {
        pgDrawMgr::deleteAllVramObjForSystem();
    }

    if (!pgLowLevelAPI::toggleFullScreen(width, height))
    {
        pgThrow(ExceptionCreateFramebufferFailed);
    }

    pgLowLevelAPI::updateFramebufferSize();
    pgLowLevelAPI::resetDrawState();

    if (pgKeyMgr::isCreated())
    {
        pgKeyMgr::resetKeyStateForSystem();
    }
}


void pgSysMgr::updateForSystem()
{
    instance();

    pgLowLevelAPI::updateFramebufferSize();
}


void pgSysMgr::readLittleEndianForSystem(void* dest, const void* src, u32 size)
{
    pgLowLevelAPI::readLittleEndian(dest, src, size);
}


void pgSysMgr::writeLittleEndianForSystem(void* dest, const void* src, u32 size)
{
    pgLowLevelAPI::writeLittleEndian(dest, src, size);
}


void pgSysMgr::setInitialDirectoryForSystem(s32 argc, char** argv)
{
    if (argc < 0 || (argc > 0 && !argv))
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgLowLevelAPI::setInitialDirectory(argc, argv);
}


pgSysMgr::pgSysMgr(const char* title, u16 width, u16 height, u16 sys_flag)
{
    if (!pgLowLevelAPI::createApplication(title, width, height, sys_flag))
    {
        pgThrow(ExceptionCreateFramebufferFailed);
    }
}


pgSysMgr::~pgSysMgr()
{
    pgLowLevelAPI::destroyApplication();
}


PG_DEFINE_OPERATOR_EQUAL(pgSysMgr)


PG_DEFINE_MANAGER_INSTANCE(pgSysMgr)
