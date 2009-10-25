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


#include "pg_task_all.h"

#include "pg_sys_all.h"
#include "pg_low_level_api.h"
#include "pg_private_macro.h"


pgTaskMgr* pgTaskMgr::m_instance = NULL;


PG_DEFINE_MANAGER_IS_CREATED(pgTaskMgr)


void pgTaskMgr::createAfterSys(u16 aim_fps)
{
    if (aim_fps == 0)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    destroyFirst();

    m_instance = pgNew(pgTaskMgr)(aim_fps);
}


PG_DEFINE_MANAGER_DESTROY(pgTaskMgr, First)


u16 pgTaskMgr::getAimFPS()
{
    return instance()->m_aim_fps;
}


r32 pgTaskMgr::getCurFPS()
{
    return instance()->m_cur_fps;
}


u64 pgTaskMgr::getExecuteUsecTime()
{
    pgTaskMgr* ins = instance();

    return ins->m_execute_time + ins->m_render_time;
}


u64 pgTaskMgr::getRenderUsecTime()
{
    return instance()->m_render_time;
}


u32 pgTaskMgr::getFrameCount()
{
    return instance()->m_frame_count;
}


pgTask* pgTaskMgr::getFirstTaskN(pgTask::TaskOrder order)
{
    return instance()->m_root_task[order].getFirstChildN();
}


pgTask* pgTaskMgr::getLastTaskN(pgTask::TaskOrder order)
{
    pgTask* task = instance()->m_root_task[order].getLastChildN();

    return task ? task->getLastDescendant() : NULL;
}


bool pgTaskMgr::isOrderActive(pgTask::TaskOrder order)
{
    return instance()->m_root_task[order].isActive();
}


void pgTaskMgr::setOrderActive(pgTask::TaskOrder from, pgTask::TaskOrder to, bool is_active)
{
    pgTaskMgr* ins = instance();

    if (from > to)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    for (s32 i = from; i <= to; i++)
    {
        ins->m_root_task[i].setActive(is_active);
    }
}


void pgTaskMgr::deleteOrder(pgTask::TaskOrder from, pgTask::TaskOrder to)
{
    pgTaskMgr* ins = instance();

    if (from > to)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    for (s32 i = from; i <= to; i++)
    {
        pgTask* root = &ins->m_root_task[i];

        for (pgTask* task = root->getLastChildN(); task; task = root->getLastChildN())
        {
            pgDeleteTask(task);
        }
    }
}


void pgTaskMgr::sendMessage(pgID msg_id, pgMsg<4>& msg)
{
    pgTaskMgr* ins = instance();

    for (u32 i = 0; i < ORDER_NUM; i++)
    {
        for (pgTask* task = ins->getFirstTaskN(static_cast<pgTask::TaskOrder>(i)); task; task = ins->m_next_task)
        {
            ins->m_next_task = task->getNextAllN();

            task->onMessage(msg_id, msg);
        }
    }

    ins->m_next_task = NULL;
}


void pgTaskMgr::resetFrameSkip()
{
    instance()->m_is_frame_skip_reset = true;
}


void pgTaskMgr::updateForSystem()
{
    pgTaskMgr* ins = instance();
    u64 update_start_time = pgSysMgr::getUsecTime();
    u64 task_start_time = update_start_time;

    for (u32 i = 0; i < ORDER_NUM; i++)
    {
        if (ins->m_root_task[i].isActive())
        {
            for (pgTask* task = ins->getFirstTaskN(static_cast<pgTask::TaskOrder>(i)); task; task = ins->m_next_task)
            {
                if (task->isActive())
                {
                    ins->m_cur_task = task;
                    ins->m_next_task = task->getNextAllN();

                    task->onUpdate();

                    if (ins->m_cur_task)
                    {
                        u64 end_time = pgSysMgr::getUsecTime();

                        task->m_execute_time = end_time - task_start_time;

                        task_start_time = end_time;
                    }
                }
                else
                {
                    ins->m_next_task = task->getLastDescendant()->getNextAllN();
                }
            }
        }
    }

    u64 update_end_time = pgSysMgr::getUsecTime();

    ins->m_execute_time = update_end_time - update_start_time;
    ins->m_render_time = 0;
    ins->m_last_update_time = update_start_time;
    ins->m_next_task = NULL;

    ins->m_frame_count++;
}


u32 pgTaskMgr::setNextTaskNameForSystem(const char* name)
{
    pgTaskMgr* ins = instance();

    if (!name)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (ins->m_is_in_destructor)
    {
        ins->m_is_in_destructor = false;
        pgThrow(ExceptionNewTaskInDestructor);
    }

    ins->m_next_task_name = name;

    return 0;
}


void pgTaskMgr::deleteTaskForSystem(pgTask* task, bool is_direct_delete)
{
    pgTaskMgr* ins = pgTaskMgr::instance();

    if (!task)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (is_direct_delete)
    {
        if (ins->m_is_in_destructor)
        {
            ins->m_is_in_destructor = false;
            pgThrow(ExceptionDeleteTaskInDestructor);
        }

        ins->m_is_in_destructor = true;
    }

    pgTask* end_task = task->getPrevAllN();
    pgTask* prev_task;

    for (task = task->getLastDescendant(); task != end_task; task = prev_task)
    {
        prev_task = task->getPrevAllN();

        if (task == ins->m_cur_task)
        {
            ins->m_cur_task = NULL;
        }

        if (task == ins->m_next_task)
        {
            ins->m_next_task = task->getNextAllN();
        }

        ins->m_next_task_name = reinterpret_cast<const char*>(task);
        pgDelete(task, pgTask);
    }

    ins->m_is_in_destructor = false;
}


bool pgTaskMgr::isFrameSkipResetForSystem()
{
    pgTaskMgr* ins = instance();
    bool is_frame_skip_reset = ins->m_is_frame_skip_reset;

    ins->m_is_frame_skip_reset = false;

    return is_frame_skip_reset;
}


void pgTaskMgr::measureRenderTimeForSystem(void (*render_func)())
{
    pgTaskMgr* ins = instance();

    if (!render_func)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    u64 render_start_time = pgSysMgr::getUsecTime();

    render_func();

    ins->m_render_time = pgSysMgr::getUsecTime() - render_start_time;

    if (ins->m_fps_measure_count % FPS_MEASURE_FRAME_NUM == 0)
    {
        ins->m_cur_fps = static_cast<r32>(1000000.0 / (render_start_time - ins->m_fps_measure_time) * FPS_MEASURE_FRAME_NUM);
        ins->m_fps_measure_time = render_start_time;
    }

    ins->m_fps_measure_count++;
}


pgTaskMgr::pgTaskMgr(u16 aim_fps)
{
    for (u32 i = 0; i < ORDER_NUM; i++)
    {
        m_root_task[i].m_order = static_cast<pgTask::TaskOrder>(i);
    }

    m_aim_fps = aim_fps;
    m_cur_fps = aim_fps;
    m_execute_time = 0;
    m_render_time = 0;
    m_last_update_time = pgSysMgr::getUsecTime() - 1000000 / m_aim_fps;
    m_frame_count = 0;
    m_fps_measure_time = pgSysMgr::getUsecTime() - 1000000 / m_aim_fps * FPS_MEASURE_FRAME_NUM;
    m_fps_measure_count = 0;
    m_cur_task = NULL;
    m_next_task = NULL;
    m_next_task_name = NULL;
    m_is_frame_skip_reset = true;
    m_is_in_destructor = false;
}


pgTaskMgr::~pgTaskMgr()
{
    deleteOrder(static_cast<pgTask::TaskOrder>(0), static_cast<pgTask::TaskOrder>(ORDER_NUM - 1));
}


PG_DEFINE_OPERATOR_EQUAL(pgTaskMgr)


PG_DEFINE_MANAGER_INSTANCE(pgTaskMgr)
