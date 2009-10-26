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

#include "ck_mem_all.h"
#include "ck_sys_all.h"
#include "ck_task_all.h"
#include "ck_key_all.h"
#include "ck_res_all.h"
#include "ck_conf_all.h"
#include "ck_draw_all.h"
#include "ck_snd_all.h"
#include "ck_dbg_all.h"

#include "ck_low_level_api.h"


static u64 s_one_frame_time;
static u64 s_next_update_time = 0;
static u64 s_last_render_time = 0;


void ckCreateCatcake(const char* title, u16 width, u16 height, u16 aim_fps, u16 sys_flag)
{
    ckMemMgr::createFirst();
    ckSysMgr::createAfterMem(title, width, height, sys_flag);
    ckTaskMgr::createAfterSys(aim_fps);
    ckKeyMgr::createAfterTask();
    ckResMgr::createAfterTask();
    ckConfMgr::createAfterRes();
    ckDrawMgr::createAfterRes();
    ckSndMgr::createAfterRes();
    ckDbgMgr::createLast();
}


void ckDestroyCatcake()
{
    ckTaskMgr::destroyFirst();
    ckDbgMgr::destroySecond();
    ckSndMgr::destroyBeforeRes();
    ckDrawMgr::destroyBeforeRes();
    ckConfMgr::destroyBeforeRes();
    ckResMgr::destroyBeforeSys();
    ckKeyMgr::destroyBeforeSys();
    ckSysMgr::destroyBeforeMem();
    ckMemMgr::destroyLast();
}


static bool updateCatcake()
{
    u64 cur_time = ckSysMgr::getUsecTime();

    if ((ckTaskMgr::isFrameSkipResetForSystem() && cur_time > s_next_update_time) || //
        cur_time - s_last_render_time >= ckDrawMgr::MAX_RENDER_INTERVAL_MSEC_TIME * 1000)
    {
        s_next_update_time = cur_time;
    }

    if (cur_time <= s_next_update_time + s_one_frame_time)
    {
        while (cur_time < s_next_update_time)
        {
            ckLowLevelAPI::sleepUsec((s_next_update_time - cur_time) / 4);

            cur_time = ckSysMgr::getUsecTime();
        }

        ckSysMgr::updateForSystem();
        ckTaskMgr::updateForSystem();

        ckTaskMgr::measureRenderTimeForSystem(ckDrawMgr::renderForSystem);
        ckLowLevelAPI::swapFramebuffer();

        s_last_render_time = cur_time;
        s_next_update_time += s_one_frame_time;

        return true;
    }
    else
    {
        ckSysMgr::updateForSystem();
        ckTaskMgr::updateForSystem();

        s_next_update_time += s_one_frame_time;

        return false;
    }
}


void ckStartCatcake()
{
    ckTry
    {
        s_one_frame_time = 1000000 / ckTaskMgr::getAimFPS();

        ckLowLevelAPI::startApplication(updateCatcake);
    }
    ckCatch(ckSysMgr::ExceptionEndCatcake) {}
    ckCatch(ckException e)
    {
#ifndef CK_NO_THROW_EXCEPTION
        ckError("Unhandled Exception: %s in %s(%d)\n", e.getException(), e.getFile(), e.getLine());
#endif
    }
}


void ckEndCatcake()
{
#ifdef CK_NO_THROW_EXCEPTION
    ckLowLevelAPI::exit(0);
#else
    ckThrow(ckSysMgr::ExceptionEndCatcake);
#endif
}


void ckError(const char* msg, ...)
{
    char buf[256];
    va_list ap;

    va_start(ap, msg);
    ckLowLevelAPI::vsprintf(buf, 256, msg, &ap);
    va_end(ap);

    ckLowLevelAPI::error(buf);
}


#ifdef CK_NO_THROW_EXCEPTION
void ckSubstituteThrow(const char* exception, const char* file, u32 line)
{
    ckError("Exception: %s in %s(%d)\n", exception, file, line);
}
#endif
