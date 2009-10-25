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


#include "test.h"


static u8 s_ctor_flag;
static u8 s_dtor_flag;
static u8 s_update_flag;
static u8 s_message_flag;


class TaskMgrTestClass : public pgTask
{
public:
    TaskMgrTestClass(pgTask::TaskOrder order, u8 no) : pgTask(order)
    {
        m_no_flag = 1 << no;

        s_ctor_flag |= m_no_flag;
    }

    TaskMgrTestClass(pgTask* parent, u8 no) : pgTask(parent)
    {
        m_no_flag = 1 << no;

        s_ctor_flag |= m_no_flag;
    }

    virtual ~TaskMgrTestClass()
    {
        s_dtor_flag |= m_no_flag;
    }

    virtual void onUpdate()
    {
        pgAssert(m_no_flag > s_update_flag);

        s_update_flag |= m_no_flag;

        pgSysMgr::sleepUsec(10000);
    }

    virtual void onMessage(pgID msg_id, pgMsg<4>& msg)
    {
        pgAssert(msg.getParam<u16>(0) == 123 && msg.getParam<u16>(1) == 456 && msg.getParam<u16>(2) == 789 && msg.getParam<s8>(3) == -10);
        pgAssert(m_no_flag > s_message_flag);

        s_message_flag |= m_no_flag;

        if (msg_id == pgID_("DELETE"))
        {
            pgDeleteTask(this);
            return;
        }
    }

private:
    u8 m_no_flag;
};


static void renderFuncTest()
{
    pgSysMgr::sleepUsec(10000);
}


void pgTaskMgrTest()
{
    /*
        static bool isCreated()
        static void createAfterSys(u16 aim_fps)
        static void destroyFirst()
    */
    {
        s_ctor_flag = 0x00;
        s_dtor_flag = 0x00;

        pgAssert(!pgTaskMgr::isCreated());

        pgAssertThrow(pgTaskMgr::createAfterSys(0), pgTaskMgr::ExceptionInvalidArgument);

        pgTaskMgr::createAfterSys(10);
        pgTaskMgr::createAfterSys(20);

        pgAssert(pgSysMgr::isCreated());

        pgTask* task = pgNewTask(TaskMgrTestClass)(pgTask::ORDER_ZERO, 0);
        pgNewTask(TaskMgrTestClass)(task, 1);
        pgNewTask(TaskMgrTestClass)(pgTask::ORDER_PLUS_7, 2);

        pgAssert(s_ctor_flag == 0x07 && s_dtor_flag == 0x00);

        pgTaskMgr::destroyFirst();
        pgTaskMgr::destroyFirst();

        pgAssert(s_ctor_flag == 0x07 && s_dtor_flag == 0x07);
        pgAssert(!pgTaskMgr::isCreated());
    }

    /*
        static u16 getAimFPS()
        static r32 getCurFPS()
        static u64 getExecuteUsecTime()
        static u64 getRenderUsecTime()
        static u32 getFrameCount()
        static void updateForSystem()
        static void measureRenderTimeForSystem(void (*render_func)())
    */
    {
        s_update_flag = 0x00;

        pgAssertThrow(pgTaskMgr::getAimFPS(), pgTaskMgr::ExceptionNotInitialized);
        pgAssertThrow(pgTaskMgr::getCurFPS(), pgTaskMgr::ExceptionNotInitialized);
        pgAssertThrow(pgTaskMgr::getExecuteUsecTime(), pgTaskMgr::ExceptionNotInitialized);
        pgAssertThrow(pgTaskMgr::getRenderUsecTime(), pgTaskMgr::ExceptionNotInitialized);
        pgAssertThrow(pgTaskMgr::getFrameCount(), pgTaskMgr::ExceptionNotInitialized);
        pgAssertThrow(pgTaskMgr::updateForSystem(), pgTaskMgr::ExceptionNotInitialized);
        pgAssertThrow(pgTaskMgr::measureRenderTimeForSystem(NULL), pgTaskMgr::ExceptionNotInitialized);

        pgTaskMgr::createAfterSys(60);

        pgAssert(pgTaskMgr::getAimFPS() == 60);
        pgAssert(isEqual(pgTaskMgr::getCurFPS(), 60.0f));
        pgAssert(pgTaskMgr::getExecuteUsecTime() == 0);
        pgAssert(pgTaskMgr::getRenderUsecTime() == 0);
        pgAssert(pgTaskMgr::getFrameCount() == 0);

        pgTask* task = pgNewTask(TaskMgrTestClass)(pgTask::ORDER_MINUS_7, 0);
        pgNewTask(TaskMgrTestClass)(task, 1);

        pgAssert(s_update_flag == 0x00);
        pgTaskMgr::updateForSystem();
        pgAssert(s_update_flag == 0x03);

        pgAssert(pgTaskMgr::getCurFPS() > 55.0f && pgTaskMgr::getCurFPS() < 65.0f);
        pgAssert(pgTaskMgr::getFrameCount() == 1);

        s_update_flag = 0x00;
        pgTaskMgr::updateForSystem();

        pgAssert(isEqual(pgTaskMgr::getAimFPS(), 60.0f));
        pgAssert(pgTaskMgr::getCurFPS() > 55.0f && pgTaskMgr::getCurFPS() < 65.0f);
        pgAssert(pgTaskMgr::getExecuteUsecTime() > 1000000 / 65);
        pgAssert(pgTaskMgr::getRenderUsecTime() == 0);
        pgAssert(pgTaskMgr::getFrameCount() == 2);

        u64 last_execute_time = pgTaskMgr::getExecuteUsecTime();

        pgTaskMgr::measureRenderTimeForSystem(renderFuncTest);
        pgAssert(pgTaskMgr::getExecuteUsecTime() == last_execute_time + pgTaskMgr::getRenderUsecTime());
        pgAssert(pgTaskMgr::getRenderUsecTime() > 1000);

        s_update_flag = 0x00;
        pgTaskMgr::updateForSystem();

        pgAssert(pgTaskMgr::getRenderUsecTime() == 0);

        pgAssertThrow(pgTaskMgr::measureRenderTimeForSystem(NULL), pgTaskMgr::ExceptionInvalidArgument);

        pgTaskMgr::destroyFirst();
    }

    /*
        static pgTask* getFirstTaskN(pgTask::TaskOrder order)
        static pgTask* getLastTaskN(pgTask::TaskOrder order)
        static void sendMessage(pgID msg_id, pgMsg<4>& msg)
    */
    {
        s_ctor_flag = 0x00;
        s_dtor_flag = 0x00;
        s_message_flag = 0x00;

        pgMsg<4> dummy;

        pgAssertThrow(pgTaskMgr::getFirstTaskN(pgTask::ORDER_MINUS_2), pgTaskMgr::ExceptionNotInitialized);
        pgAssertThrow(pgTaskMgr::getLastTaskN(pgTask::ORDER_PLUS_2), pgTaskMgr::ExceptionNotInitialized);
        pgAssertThrow(pgTaskMgr::sendMessage(pgID::ZERO, dummy), pgTaskMgr::ExceptionNotInitialized);

        pgTaskMgr::createAfterSys(60);

        pgTask* task1 = pgNewTask(TaskMgrTestClass)(pgTask::ORDER_MINUS_3, 0);
        pgTask* task2 = pgNewTask(TaskMgrTestClass)(task1, 1);
        pgTask* task3 = pgNewTask(TaskMgrTestClass)(pgTask::ORDER_ZERO, 2);
        pgTask* task4 = pgNewTask(TaskMgrTestClass)(task3, 3);
        pgTask* task5 = pgNewTask(TaskMgrTestClass)(task4, 4);
        pgTask* task6 = pgNewTask(TaskMgrTestClass)(pgTask::ORDER_PLUS_3, 5);
        pgTask* task7 = pgNewTask(TaskMgrTestClass)(task6, 6);

        pgAssert(!pgTaskMgr::getFirstTaskN(pgTask::ORDER_MINUS_4));
        pgAssert(!pgTaskMgr::getLastTaskN(pgTask::ORDER_MINUS_4));
        pgAssert(pgTaskMgr::getFirstTaskN(pgTask::ORDER_MINUS_3) == task1);
        pgAssert(pgTaskMgr::getLastTaskN(pgTask::ORDER_MINUS_3) == task2);
        pgAssert(pgTaskMgr::getFirstTaskN(pgTask::ORDER_ZERO) == task3);
        pgAssert(pgTaskMgr::getLastTaskN(pgTask::ORDER_ZERO) == task5);
        pgAssert(pgTaskMgr::getFirstTaskN(pgTask::ORDER_PLUS_3) == task6);
        pgAssert(pgTaskMgr::getLastTaskN(pgTask::ORDER_PLUS_3) == task7);
        pgAssert(!pgTaskMgr::getFirstTaskN(pgTask::ORDER_PLUS_4));
        pgAssert(!pgTaskMgr::getLastTaskN(pgTask::ORDER_PLUS_4));

        pgMsg<4> msg;
        msg.setParam<u16>(0, 123);
        msg.setParam<u16>(1, 456);
        msg.setParam<u16>(2, 789);
        msg.setParam<s8>(3, -10);

        pgTaskMgr::sendMessage(pgID::ZERO, msg);
        pgAssert(s_ctor_flag == 0x7f && s_dtor_flag == 0x00 && s_message_flag == 0x7f);

        s_message_flag = 0x00;

        pgTaskMgr::sendMessage(pgID_("DELETE"), msg);
        pgAssert(s_ctor_flag == 0x7f && s_dtor_flag == 0x7f && s_message_flag == 0x25);

        pgTaskMgr::destroyFirst();
    }

    /*
        static bool isOrderActive(pgTask::TaskOrder order)
        static void setOrderActive(pgTask::TaskOrder from, pgTask::TaskOrder to, bool is_active)
        static void deleteOrder(pgTask::TaskOrder from, pgTask::TaskOrder to)
    */
    {
        s_dtor_flag = 0x00;
        s_update_flag = 0x00;

        pgAssertThrow(pgTaskMgr::isOrderActive(pgTask::ORDER_MINUS_2), pgTaskMgr::ExceptionNotInitialized);
        pgAssertThrow(pgTaskMgr::setOrderActive(pgTask::ORDER_MINUS_1, pgTask::ORDER_PLUS_1, true), pgTaskMgr::ExceptionNotInitialized);
        pgAssertThrow(pgTaskMgr::deleteOrder(pgTask::ORDER_MINUS_5, pgTask::ORDER_PLUS_5), pgTaskMgr::ExceptionNotInitialized);

        pgTaskMgr::createAfterSys(60);

        for (s32 i = pgTask::ORDER_MINUS_8_FOR_SYSTEM; i <= pgTask::ORDER_PLUS_8_FOR_SYSTEM; i++)
        {
            pgAssert(pgTaskMgr::isOrderActive(static_cast<pgTask::TaskOrder>(i)));
        }

        pgNewTask(TaskMgrTestClass)(pgTask::ORDER_MINUS_6, 0);
        pgNewTask(TaskMgrTestClass)(pgTask::ORDER_MINUS_4, 1);
        pgNewTask(TaskMgrTestClass)(pgTask::ORDER_MINUS_2, 2);
        pgTask* task = pgNewTask(TaskMgrTestClass)(pgTask::ORDER_ZERO, 3);
        pgNewTask(TaskMgrTestClass)(task, 4);
        pgNewTask(TaskMgrTestClass)(pgTask::ORDER_PLUS_3, 5);
        pgNewTask(TaskMgrTestClass)(pgTask::ORDER_PLUS_5, 6);

        pgTaskMgr::updateForSystem();
        pgAssert(s_update_flag == 0x7f);

        pgTaskMgr::setOrderActive(pgTask::ORDER_MINUS_7, pgTask::ORDER_PLUS_3, false);

        for (s32 i = pgTask::ORDER_MINUS_8_FOR_SYSTEM; i <= pgTask::ORDER_PLUS_8_FOR_SYSTEM; i++)
        {
            if (i >= pgTask::ORDER_MINUS_7 && i <= pgTask::ORDER_PLUS_3)
            {
                pgAssert(!pgTaskMgr::isOrderActive(static_cast<pgTask::TaskOrder>(i)));
            }
            else
            {
                pgAssert(pgTaskMgr::isOrderActive(static_cast<pgTask::TaskOrder>(i)));
            }
        }

        s_update_flag = 0;

        pgTaskMgr::updateForSystem();
        pgAssert(s_update_flag == 0x40);

        pgTaskMgr::deleteOrder(pgTask::ORDER_MINUS_5, pgTask::ORDER_PLUS_7);
        pgAssert(s_dtor_flag == 0x7e);

        s_update_flag = 0;

        pgTaskMgr::updateForSystem();
        pgAssert(s_update_flag == 0x00);

        pgTaskMgr::setOrderActive(pgTask::ORDER_MINUS_7, pgTask::ORDER_PLUS_7, true);

        s_update_flag = 0;

        pgTaskMgr::updateForSystem();
        pgAssert(s_update_flag == 0x01);

        pgAssertThrow(pgTaskMgr::setOrderActive(pgTask::ORDER_PLUS_1, pgTask::ORDER_ZERO, false), pgTaskMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgTaskMgr::deleteOrder(pgTask::ORDER_PLUS_7, pgTask::ORDER_PLUS_6), pgTaskMgr::ExceptionInvalidArgument);

        pgTaskMgr::destroyFirst();
    }

    /*
        static u32 setNextTaskNameForSystem(const char* name)
        static void deleteTaskForSystem(pgTask* task, bool is_direct_delete)
    */
    {
        pgAssertThrow(pgTaskMgr::setNextTaskNameForSystem("TEST"), pgTaskMgr::ExceptionNotInitialized);
        pgAssertThrow(pgTaskMgr::deleteTaskForSystem(NULL, false), pgTaskMgr::ExceptionNotInitialized);

        pgTaskMgr::createAfterSys(60);

        const char* name = "TEST";
        pgTaskMgr::setNextTaskNameForSystem(name);
        pgTask* task = pgNew(TaskMgrTestClass)(pgTask::ORDER_ZERO, 0);
        pgAssert(task->getName() == name);

        pgAssert(pgTaskMgr::getFirstTaskN(pgTask::ORDER_ZERO) == task);
        pgAssert(pgTaskMgr::getLastTaskN(pgTask::ORDER_ZERO) == task);

        pgTaskMgr::deleteTaskForSystem(task, true);

        task = pgNewTask(TaskMgrTestClass)(pgTask::ORDER_ZERO, 0);

        pgTaskMgr::deleteTaskForSystem(task, false);

        pgAssert(!pgTaskMgr::getFirstTaskN(pgTask::ORDER_ZERO));
        pgAssert(!pgTaskMgr::getLastTaskN(pgTask::ORDER_ZERO));

        pgAssertThrow(pgTaskMgr::setNextTaskNameForSystem(NULL), pgTaskMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgTaskMgr::deleteTaskForSystem(NULL, false), pgTaskMgr::ExceptionInvalidArgument);

        pgTaskMgr::destroyFirst();
    }

    /*
        static void resetFrameSkip()
        static bool isFrameSkipResetForSystem()
    */
    {
        pgAssertThrow(pgTaskMgr::resetFrameSkip(), pgTaskMgr::ExceptionNotInitialized);
        pgAssertThrow(pgTaskMgr::isFrameSkipResetForSystem(), pgTaskMgr::ExceptionNotInitialized);

        pgTaskMgr::createAfterSys(60);

        pgAssert(pgTaskMgr::isFrameSkipResetForSystem());
        pgAssert(!pgTaskMgr::isFrameSkipResetForSystem());
        pgAssert(!pgTaskMgr::isFrameSkipResetForSystem());

        pgTaskMgr::resetFrameSkip();
        pgAssert(pgTaskMgr::isFrameSkipResetForSystem());
        pgAssert(!pgTaskMgr::isFrameSkipResetForSystem());
        pgAssert(!pgTaskMgr::isFrameSkipResetForSystem());

        pgTaskMgr::destroyFirst();
    }
}
