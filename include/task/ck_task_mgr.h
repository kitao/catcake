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


/*!
    @ingroup pgTask
    TODO
*/
class PG_API pgTaskMgr
{
    friend class pgTask;

public:
    pgDefineException(ExceptionDeleteTaskInDestructor);
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionNewTaskInDestructor);
    pgDefineException(ExceptionNotInitialized);

    static bool isCreated();
    static void createAfterSys(u16 aim_fps);
    static void destroyFirst();

    static u16 getAimFPS();
    static r32 getCurFPS();
    static u64 getExecuteUsecTime();
    static u64 getRenderUsecTime();
    static u32 getFrameCount();

    static pgTask* getFirstTaskN(pgTask::TaskOrder order);
    static pgTask* getLastTaskN(pgTask::TaskOrder order);

    static bool isOrderActive(pgTask::TaskOrder order);
    static void setOrderActive(pgTask::TaskOrder from, pgTask::TaskOrder to, bool is_active);
    static void deleteOrder(pgTask::TaskOrder from, pgTask::TaskOrder to);

    static void sendMessage(pgID msg_id, pgMsg<4>& msg);

    static void resetFrameSkip();

    static void updateForSystem();
    static void measureRenderTimeForSystem(void (*render_func)());
    static u32 setNextTaskNameForSystem(const char* name);
    static void deleteTaskForSystem(pgTask* task, bool is_direct_delete);
    static bool isFrameSkipResetForSystem();

private:
    static const u32 ORDER_NUM = 8 + 1 + 8;
    static const u32 FPS_MEASURE_FRAME_NUM = 10;

    pgTaskMgr(u16 aim_fps);
    ~pgTaskMgr();
    void operator=(const pgTaskMgr&);

    static pgTaskMgr* instance();

    pgTask m_root_task[ORDER_NUM];
    u16 m_aim_fps;
    r32 m_cur_fps;
    u64 m_execute_time;
    u64 m_render_time;
    u64 m_last_update_time;
    u32 m_frame_count;
    u64 m_fps_measure_time;
    u32 m_fps_measure_count;
    pgTask* m_cur_task;
    pgTask* m_next_task;
    const char* m_next_task_name;
    bool m_is_frame_skip_reset;
    bool m_is_in_destructor;

    static pgTaskMgr* m_instance;
};
