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

#include "pg_mem_all.h"
#include "pg_sys_all.h"
#include "pg_task_all.h"
#include "pg_key_all.h"
#include "pg_res_all.h"
#include "pg_conf_all.h"
#include "pg_draw_all.h"
#include "pg_snd_all.h"
#include "pg_dbg_all.h"

#include "pg_low_level_api.h"


static u64 s_one_frame_time;
static u64 s_next_update_time = 0;
static u64 s_last_render_time = 0;


void pgCreatePogolyn(const char* title, u16 width, u16 height, u16 aim_fps, u16 sys_flag)
{
    pgMemMgr::createFirst();
    pgSysMgr::createAfterMem(title, width, height, sys_flag);
    pgTaskMgr::createAfterSys(aim_fps);
    pgKeyMgr::createAfterTask();
    pgResMgr::createAfterTask();
    pgConfMgr::createAfterRes();
    pgDrawMgr::createAfterRes();
    pgSndMgr::createAfterRes();
    pgDbgMgr::createLast();
}


void pgDestroyPogolyn()
{
    pgTaskMgr::destroyFirst();
    pgDbgMgr::destroySecond();
    pgSndMgr::destroyBeforeRes();
    pgDrawMgr::destroyBeforeRes();
    pgConfMgr::destroyBeforeRes();
    pgResMgr::destroyBeforeSys();
    pgKeyMgr::destroyBeforeSys();
    pgSysMgr::destroyBeforeMem();
    pgMemMgr::destroyLast();
}


static bool updatePogolyn()
{
    u64 cur_time = pgSysMgr::getUsecTime();

    if ((pgTaskMgr::isFrameSkipResetForSystem() && cur_time > s_next_update_time) || //
        cur_time - s_last_render_time >= pgDrawMgr::MAX_RENDER_INTERVAL_MSEC_TIME * 1000)
    {
        s_next_update_time = cur_time;
    }

    if (cur_time <= s_next_update_time + s_one_frame_time)
    {
        while (cur_time < s_next_update_time)
        {
            pgLowLevelAPI::sleepUsec((s_next_update_time - cur_time) / 4);

            cur_time = pgSysMgr::getUsecTime();
        }

        pgSysMgr::updateForSystem();
        pgTaskMgr::updateForSystem();

        pgTaskMgr::measureRenderTimeForSystem(pgDrawMgr::renderForSystem);
        pgLowLevelAPI::swapFramebuffer();

        s_last_render_time = cur_time;
        s_next_update_time += s_one_frame_time;

        return true;
    }
    else
    {
        pgSysMgr::updateForSystem();
        pgTaskMgr::updateForSystem();

        s_next_update_time += s_one_frame_time;

        return false;
    }
}


void pgStartPogolyn()
{
    pgTry
    {
        s_one_frame_time = 1000000 / pgTaskMgr::getAimFPS();

        pgLowLevelAPI::startApplication(updatePogolyn);
    }
    pgCatch(pgSysMgr::ExceptionEndPogolyn) {}
    pgCatch(pgException e)
    {
#ifndef PG_NO_THROW_EXCEPTION
        pgError("Unhandled Exception: %s in %s(%d)\n", e.getException(), e.getFile(), e.getLine());
#endif
    }
}


void pgEndPogolyn()
{
#ifdef PG_NO_THROW_EXCEPTION
    pgLowLevelAPI::exit(0);
#else
    pgThrow(pgSysMgr::ExceptionEndPogolyn);
#endif
}


void pgError(const char* msg, ...)
{
    char buf[256];
    va_list ap;

    va_start(ap, msg);
    pgLowLevelAPI::vsprintf(buf, 256, msg, &ap);
    va_end(ap);

    pgLowLevelAPI::error(buf);
}


#ifdef PG_NO_THROW_EXCEPTION
void pgSubstituteThrow(const char* exception, const char* file, u32 line)
{
    pgError("Exception: %s in %s(%d)\n", exception, file, line);
}
#endif
