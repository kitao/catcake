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


#include "ck_task_all.h"

#include "ck_sys_all.h"
#include "ck_low_level_api.h"
#include "ck_private_macro.h"


ckTaskMgr* ckTaskMgr::m_instance = NULL;


CK_DEFINE_MANAGER_IS_CREATED(ckTaskMgr)


void ckTaskMgr::createAfterSys(u16 aim_fps)
{
    if (aim_fps == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    destroyFirst();

    m_instance = ckNew(ckTaskMgr)(aim_fps);
}


CK_DEFINE_MANAGER_DESTROY(ckTaskMgr, First)


u16 ckTaskMgr::getAimFPS()
{
    return instance()->m_aim_fps;
}


r32 ckTaskMgr::getCurFPS()
{
    return instance()->m_cur_fps;
}


u64 ckTaskMgr::getExecuteUsecTime()
{
    ckTaskMgr* ins = instance();

    return ins->m_execute_time + ins->m_render_time;
}


u64 ckTaskMgr::getRenderUsecTime()
{
    return instance()->m_render_time;
}


u32 ckTaskMgr::getFrameCount()
{
    return instance()->m_frame_count;
}


ckTask* ckTaskMgr::getFirstTaskN(ckTask::TaskOrder order)
{
    return instance()->m_root_task[order].getFirstChildN();
}


ckTask* ckTaskMgr::getLastTaskN(ckTask::TaskOrder order)
{
    ckTask* task = instance()->m_root_task[order].getLastChildN();

    return task ? task->getLastDescendant() : NULL;
}


bool ckTaskMgr::isOrderActive(ckTask::TaskOrder order)
{
    return instance()->m_root_task[order].isActive();
}


void ckTaskMgr::setOrderActive(ckTask::TaskOrder from, ckTask::TaskOrder to, bool is_active)
{
    ckTaskMgr* ins = instance();

    if (from > to)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    for (s32 i = from; i <= to; i++)
    {
        ins->m_root_task[i].setActive(is_active);
    }
}


void ckTaskMgr::deleteOrder(ckTask::TaskOrder from, ckTask::TaskOrder to)
{
    ckTaskMgr* ins = instance();

    if (from > to)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    for (s32 i = from; i <= to; i++)
    {
        ckTask* root = &ins->m_root_task[i];

        for (ckTask* task = root->getLastChildN(); task; task = root->getLastChildN())
        {
            ckDeleteTask(task);
        }
    }
}


void ckTaskMgr::sendMessage(ckID msg_id, ckMsg<4>& msg)
{
    ckTaskMgr* ins = instance();

    for (u32 i = 0; i < ORDER_NUM; i++)
    {
        for (ckTask* task = ins->getFirstTaskN(static_cast<ckTask::TaskOrder>(i)); task; task = ins->m_next_task)
        {
            ins->m_next_task = task->getNextAllN();

            task->onMessage(msg_id, msg);
        }
    }

    ins->m_next_task = NULL;
}


void ckTaskMgr::resetFrameSkip()
{
    instance()->m_is_frame_skip_reset = true;
}


void ckTaskMgr::updateForSystem()
{
    ckTaskMgr* ins = instance();
    u64 update_start_time = ckSysMgr::getUsecTime();
    u64 task_start_time = update_start_time;

    for (u32 i = 0; i < ORDER_NUM; i++)
    {
        if (ins->m_root_task[i].isActive())
        {
            for (ckTask* task = ins->getFirstTaskN(static_cast<ckTask::TaskOrder>(i)); task; task = ins->m_next_task)
            {
                if (task->isActive())
                {
                    ins->m_cur_task = task;
                    ins->m_next_task = task->getNextAllN();

                    task->onUpdate();

                    if (ins->m_cur_task)
                    {
                        u64 end_time = ckSysMgr::getUsecTime();

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

    u64 update_end_time = ckSysMgr::getUsecTime();

    ins->m_execute_time = update_end_time - update_start_time;
    ins->m_render_time = 0;
    ins->m_last_update_time = update_start_time;
    ins->m_next_task = NULL;

    ins->m_frame_count++;
}


u32 ckTaskMgr::setNextTaskNameForSystem(const char* name)
{
    ckTaskMgr* ins = instance();

    if (!name)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (ins->m_is_in_destructor)
    {
        ins->m_is_in_destructor = false;
        ckThrow(ExceptionNewTaskInDestructor);
    }

    ins->m_next_task_name = name;

    return 0;
}


void ckTaskMgr::deleteTaskForSystem(ckTask* task)
{
    ckTaskMgr* ins = ckTaskMgr::instance();

    if (!task)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (ins->m_is_in_destructor)
    {
        ins->m_is_in_destructor = false;
        ckThrow(ExceptionDeleteTaskInDestructor);
    }

    ins->m_is_in_destructor = true;

    ckTask* end_task = task->getPrevAllN();
    ckTask* prev_task;

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
        ckDelete(task, ckTask);
    }

    ins->m_is_in_destructor = false;
}


bool ckTaskMgr::isFrameSkipResetForSystem()
{
    ckTaskMgr* ins = instance();
    bool is_frame_skip_reset = ins->m_is_frame_skip_reset;

    ins->m_is_frame_skip_reset = false;

    return is_frame_skip_reset;
}


void ckTaskMgr::measureRenderTimeForSystem(void (*render_func)())
{
    ckTaskMgr* ins = instance();

    if (!render_func)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    u64 render_start_time = ckSysMgr::getUsecTime();

    render_func();

    ins->m_render_time = ckSysMgr::getUsecTime() - render_start_time;

    if (ins->m_fps_measure_count % FPS_MEASURE_FRAME_NUM == 0)
    {
        ins->m_cur_fps = static_cast<r32>(1000000.0 / (render_start_time - ins->m_fps_measure_time) * FPS_MEASURE_FRAME_NUM);
        ins->m_fps_measure_time = render_start_time;
    }

    ins->m_fps_measure_count++;
}


ckTaskMgr::ckTaskMgr(u16 aim_fps)
{
    for (u32 i = 0; i < ORDER_NUM; i++)
    {
        m_root_task[i].m_order = static_cast<ckTask::TaskOrder>(i);
    }

    m_aim_fps = aim_fps;
    m_cur_fps = aim_fps;
    m_execute_time = 0;
    m_render_time = 0;
    m_last_update_time = ckSysMgr::getUsecTime() - 1000000 / m_aim_fps;
    m_frame_count = 0;
    m_fps_measure_time = ckSysMgr::getUsecTime() - 1000000 / m_aim_fps * FPS_MEASURE_FRAME_NUM;
    m_fps_measure_count = 0;
    m_cur_task = NULL;
    m_next_task = NULL;
    m_next_task_name = NULL;
    m_is_frame_skip_reset = true;
    m_is_in_destructor = false;
}


ckTaskMgr::~ckTaskMgr()
{
    deleteOrder(static_cast<ckTask::TaskOrder>(0), static_cast<ckTask::TaskOrder>(ORDER_NUM - 1));
}


CK_DEFINE_OPERATOR_EQUAL(ckTaskMgr)


CK_DEFINE_MANAGER_INSTANCE(ckTaskMgr)
