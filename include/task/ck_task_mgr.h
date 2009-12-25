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
    @ingroup ckTask
    The task manager singleton.
*/
class CK_API ckTaskMgr
{
    friend class ckTask;

public:
    ckDefineException(ExceptionDeleteTaskInDestructor);
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionNewTaskInDestructor);
    ckDefineException(ExceptionNotInitialized);

    /*!
        Returns whether the task manager singleton is created.
    */
    static bool isCreated();

    /*!
        Creates the task manager singleton.
        @param[in] aim_fps An aim fps.
    */
    static void createAfterSys(u16 aim_fps);

    /*!
        Destroys the task manager singleton.
    */
    static void destroyFirst();

    /*!
        Returns the aim fps.
        @return The aim fps.
    */
    static u16 getAimFPS();

    /*!
        Returns the current fps.
        @return The current fps.
    */
    static r32 getCurFPS();

    /*!
        Returns the execute time in usec.
        @return The execute time.
    */
    static u64 getExecuteUsecTime();

    /*!
        Returns the rendering time in usec.
        @return The rendering time in usec.
    */
    static u64 getRenderUsecTime();

    /*!
        Returns the number of elapsed frames.
    */
    static u32 getFrameCount();

    /*!
        Returns the first task of the specified order. If the first task doesn't exist, returns NULL.
        @return The first task of the specified order.
    */
    static ckTask* getFirstTaskN(ckTask::TaskOrder order);

    /*!
        Returns the last task of the specified order. If the last task doesn't exist, returns NULL.
        @return The last task of the specified order.
    */
    static ckTask* getLastTaskN(ckTask::TaskOrder order);

    /*!
        Returns the specified order is active.
        @param[in] order The task order.
        @return The specified order is active.
    */
    static bool isOrderActive(ckTask::TaskOrder order);

    /*!
        Determines the specified orders is active.
        @param[in] from The first task order.
        @param[in] to The last task order.
        @param[in] is_active Whether the task orders is active.
    */
    static void setOrderActive(ckTask::TaskOrder from, ckTask::TaskOrder to, bool is_active);

    /*!
        Deletes all the tasks of the specified order.
        @param[in] from The first task order.
        @param[in] to The last task order.
    */
    static void deleteOrder(ckTask::TaskOrder from, ckTask::TaskOrder to);

    /*!
        Sends a message to the specified task.
        @param[in] msg_id The ID of a message. If uxID::ZERO is specified, sends to all the tasks.
        @param[in] msg A message.
    */
    static void sendMessage(ckID msg_id, ckMsg<4>& msg);

    /*!
        Suspends the frame-skipping of the current frame.
    */
    static void resetFrameSkip();

    /*!
        Updates the frame. This method is only for system.
    */
    static void updateForSystem();

    /*!
        Measures the rendering time. This method is only for system.
        @param[in] render_func A rendering function.
    */
    static void measureRenderTimeForSystem(void (*render_func)());

    /*!
        Sets the name of the next task.
        @param[in] name The name of the next task.
    */
    static u32 setNextTaskNameForSystem(const char* name);

    /*!
        Deletes the specified task. This method is only for system.
        @param[in] task A task.
    */
    static void deleteTaskForSystem(ckTask* task);

    /*!
        Returns whether frame-skipping is on.
        @return Whether frame-skipping is on.
    */
    static bool isFrameSkipResetForSystem();

private:
    static const u32 ORDER_NUM = 8 + 1 + 8;
    static const u32 FPS_MEASURE_FRAME_NUM = 10;

    ckTaskMgr(u16 aim_fps);
    ~ckTaskMgr();
    void operator=(const ckTaskMgr&);

    static ckTaskMgr* instance();

    ckTask m_root_task[ORDER_NUM];
    u16 m_aim_fps;
    r32 m_cur_fps;
    u64 m_execute_time;
    u64 m_render_time;
    u64 m_last_update_time;
    u32 m_frame_count;
    u64 m_fps_measure_time;
    u32 m_fps_measure_count;
    ckTask* m_cur_task;
    ckTask* m_next_task;
    const char* m_next_task_name;
    bool m_is_frame_skip_reset;
    bool m_is_in_destructor;

    static ckTaskMgr* m_instance;
};
