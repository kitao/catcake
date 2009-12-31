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
