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


#include "test.h"


static u8 s_ctor_flag;
static u8 s_dtor_flag;
static u8 s_update_flag;
static u8 s_message_flag;


class TaskMgrTestClass : public ckTask
{
public:
    TaskMgrTestClass(ckTask::TaskOrder order, u8 no) : ckTask(order)
    {
        m_no_flag = 1 << no;

        s_ctor_flag |= m_no_flag;
    }

    TaskMgrTestClass(ckTask* parent, u8 no) : ckTask(parent)
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
        ckAssert(m_no_flag > s_update_flag);

        s_update_flag |= m_no_flag;

        ckSysMgr::sleepUsec(10000);
    }

    virtual void onMessage(ckID msg_id, ckMsg<4>& msg)
    {
        ckAssert(msg.getParam<u16>(0) == 123 && msg.getParam<u16>(1) == 456 && msg.getParam<u16>(2) == 789 && msg.getParam<s8>(3) == -10);
        ckAssert(m_no_flag > s_message_flag);

        s_message_flag |= m_no_flag;

        if (msg_id == ckID_("DELETE"))
        {
            ckDeleteTask(this);
            return;
        }
    }

private:
    u8 m_no_flag;
};


static void renderFuncTest()
{
    ckSysMgr::sleepUsec(10000);
}


void ckTaskMgrTest()
{
    /*
        static bool isCreated()
        static void createAfterSys(u16 aim_fps)
        static void destroyFirst()
    */
    {
        s_ctor_flag = 0x00;
        s_dtor_flag = 0x00;

        ckAssert(!ckTaskMgr::isCreated());

        ckAssertThrow(ckTaskMgr::createAfterSys(0), ckTaskMgr::ExceptionInvalidArgument);

        ckTaskMgr::createAfterSys(10);
        ckTaskMgr::createAfterSys(20);

        ckAssert(ckSysMgr::isCreated());

        ckTask* task = ckNewTask(TaskMgrTestClass)(ckTask::ORDER_ZERO, 0);
        ckNewTask(TaskMgrTestClass)(task, 1);
        ckNewTask(TaskMgrTestClass)(ckTask::ORDER_PLUS_7, 2);

        ckAssert(s_ctor_flag == 0x07 && s_dtor_flag == 0x00);

        ckTaskMgr::destroyFirst();
        ckTaskMgr::destroyFirst();

        ckAssert(s_ctor_flag == 0x07 && s_dtor_flag == 0x07);
        ckAssert(!ckTaskMgr::isCreated());
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

        ckAssertThrow(ckTaskMgr::getAimFPS(), ckTaskMgr::ExceptionNotInitialized);
        ckAssertThrow(ckTaskMgr::getCurFPS(), ckTaskMgr::ExceptionNotInitialized);
        ckAssertThrow(ckTaskMgr::getExecuteUsecTime(), ckTaskMgr::ExceptionNotInitialized);
        ckAssertThrow(ckTaskMgr::getRenderUsecTime(), ckTaskMgr::ExceptionNotInitialized);
        ckAssertThrow(ckTaskMgr::getFrameCount(), ckTaskMgr::ExceptionNotInitialized);
        ckAssertThrow(ckTaskMgr::updateForSystem(), ckTaskMgr::ExceptionNotInitialized);
        ckAssertThrow(ckTaskMgr::measureRenderTimeForSystem(NULL), ckTaskMgr::ExceptionNotInitialized);

        ckTaskMgr::createAfterSys(60);

        ckAssert(ckTaskMgr::getAimFPS() == 60);
        ckAssert(isEqual(ckTaskMgr::getCurFPS(), 60.0f));
        ckAssert(ckTaskMgr::getExecuteUsecTime() == 0);
        ckAssert(ckTaskMgr::getRenderUsecTime() == 0);
        ckAssert(ckTaskMgr::getFrameCount() == 0);

        ckTask* task = ckNewTask(TaskMgrTestClass)(ckTask::ORDER_MINUS_7, 0);
        ckNewTask(TaskMgrTestClass)(task, 1);

        ckAssert(s_update_flag == 0x00);
        ckTaskMgr::updateForSystem();
        ckAssert(s_update_flag == 0x03);

        ckAssert(ckTaskMgr::getCurFPS() > 55.0f && ckTaskMgr::getCurFPS() < 65.0f);
        ckAssert(ckTaskMgr::getFrameCount() == 1);

        s_update_flag = 0x00;
        ckTaskMgr::updateForSystem();

        ckAssert(isEqual(ckTaskMgr::getAimFPS(), 60.0f));
        ckAssert(ckTaskMgr::getCurFPS() > 55.0f && ckTaskMgr::getCurFPS() < 65.0f);
        ckAssert(ckTaskMgr::getExecuteUsecTime() > 1000000 / 65);
        ckAssert(ckTaskMgr::getRenderUsecTime() == 0);
        ckAssert(ckTaskMgr::getFrameCount() == 2);

        u64 last_execute_time = ckTaskMgr::getExecuteUsecTime();

        ckTaskMgr::measureRenderTimeForSystem(renderFuncTest);
        ckAssert(ckTaskMgr::getExecuteUsecTime() == last_execute_time + ckTaskMgr::getRenderUsecTime());
        ckAssert(ckTaskMgr::getRenderUsecTime() > 1000);

        s_update_flag = 0x00;
        ckTaskMgr::updateForSystem();

        ckAssert(ckTaskMgr::getRenderUsecTime() == 0);

        ckAssertThrow(ckTaskMgr::measureRenderTimeForSystem(NULL), ckTaskMgr::ExceptionInvalidArgument);

        ckTaskMgr::destroyFirst();
    }

    /*
        static ckTask* getFirstTaskN(ckTask::TaskOrder order)
        static ckTask* getLastTaskN(ckTask::TaskOrder order)
        static void sendMessage(ckID msg_id, ckMsg<4>& msg)
    */
    {
        s_ctor_flag = 0x00;
        s_dtor_flag = 0x00;
        s_message_flag = 0x00;

        ckMsg<4> dummy;

        ckAssertThrow(ckTaskMgr::getFirstTaskN(ckTask::ORDER_MINUS_2), ckTaskMgr::ExceptionNotInitialized);
        ckAssertThrow(ckTaskMgr::getLastTaskN(ckTask::ORDER_PLUS_2), ckTaskMgr::ExceptionNotInitialized);
        ckAssertThrow(ckTaskMgr::sendMessage(ckID::ZERO, dummy), ckTaskMgr::ExceptionNotInitialized);

        ckTaskMgr::createAfterSys(60);

        ckTask* task1 = ckNewTask(TaskMgrTestClass)(ckTask::ORDER_MINUS_3, 0);
        ckTask* task2 = ckNewTask(TaskMgrTestClass)(task1, 1);
        ckTask* task3 = ckNewTask(TaskMgrTestClass)(ckTask::ORDER_ZERO, 2);
        ckTask* task4 = ckNewTask(TaskMgrTestClass)(task3, 3);
        ckTask* task5 = ckNewTask(TaskMgrTestClass)(task4, 4);
        ckTask* task6 = ckNewTask(TaskMgrTestClass)(ckTask::ORDER_PLUS_3, 5);
        ckTask* task7 = ckNewTask(TaskMgrTestClass)(task6, 6);

        ckAssert(!ckTaskMgr::getFirstTaskN(ckTask::ORDER_MINUS_4));
        ckAssert(!ckTaskMgr::getLastTaskN(ckTask::ORDER_MINUS_4));
        ckAssert(ckTaskMgr::getFirstTaskN(ckTask::ORDER_MINUS_3) == task1);
        ckAssert(ckTaskMgr::getLastTaskN(ckTask::ORDER_MINUS_3) == task2);
        ckAssert(ckTaskMgr::getFirstTaskN(ckTask::ORDER_ZERO) == task3);
        ckAssert(ckTaskMgr::getLastTaskN(ckTask::ORDER_ZERO) == task5);
        ckAssert(ckTaskMgr::getFirstTaskN(ckTask::ORDER_PLUS_3) == task6);
        ckAssert(ckTaskMgr::getLastTaskN(ckTask::ORDER_PLUS_3) == task7);
        ckAssert(!ckTaskMgr::getFirstTaskN(ckTask::ORDER_PLUS_4));
        ckAssert(!ckTaskMgr::getLastTaskN(ckTask::ORDER_PLUS_4));

        ckMsg<4> msg;
        msg.setParam<u16>(0, 123);
        msg.setParam<u16>(1, 456);
        msg.setParam<u16>(2, 789);
        msg.setParam<s8>(3, -10);

        ckTaskMgr::sendMessage(ckID::ZERO, msg);
        ckAssert(s_ctor_flag == 0x7f && s_dtor_flag == 0x00 && s_message_flag == 0x7f);

        s_message_flag = 0x00;

        ckTaskMgr::sendMessage(ckID_("DELETE"), msg);
        ckAssert(s_ctor_flag == 0x7f && s_dtor_flag == 0x7f && s_message_flag == 0x25);

        ckTaskMgr::destroyFirst();
    }

    /*
        static bool isOrderActive(ckTask::TaskOrder order)
        static void setOrderActive(ckTask::TaskOrder from, ckTask::TaskOrder to, bool is_active)
        static void deleteOrder(ckTask::TaskOrder from, ckTask::TaskOrder to)
    */
    {
        s_dtor_flag = 0x00;
        s_update_flag = 0x00;

        ckAssertThrow(ckTaskMgr::isOrderActive(ckTask::ORDER_MINUS_2), ckTaskMgr::ExceptionNotInitialized);
        ckAssertThrow(ckTaskMgr::setOrderActive(ckTask::ORDER_MINUS_1, ckTask::ORDER_PLUS_1, true), ckTaskMgr::ExceptionNotInitialized);
        ckAssertThrow(ckTaskMgr::deleteOrder(ckTask::ORDER_MINUS_5, ckTask::ORDER_PLUS_5), ckTaskMgr::ExceptionNotInitialized);

        ckTaskMgr::createAfterSys(60);

        for (s32 i = ckTask::ORDER_MINUS_8_FOR_SYSTEM; i <= ckTask::ORDER_PLUS_8_FOR_SYSTEM; i++)
        {
            ckAssert(ckTaskMgr::isOrderActive(static_cast<ckTask::TaskOrder>(i)));
        }

        ckNewTask(TaskMgrTestClass)(ckTask::ORDER_MINUS_6, 0);
        ckNewTask(TaskMgrTestClass)(ckTask::ORDER_MINUS_4, 1);
        ckNewTask(TaskMgrTestClass)(ckTask::ORDER_MINUS_2, 2);
        ckTask* task = ckNewTask(TaskMgrTestClass)(ckTask::ORDER_ZERO, 3);
        ckNewTask(TaskMgrTestClass)(task, 4);
        ckNewTask(TaskMgrTestClass)(ckTask::ORDER_PLUS_3, 5);
        ckNewTask(TaskMgrTestClass)(ckTask::ORDER_PLUS_5, 6);

        ckTaskMgr::updateForSystem();
        ckAssert(s_update_flag == 0x7f);

        ckTaskMgr::setOrderActive(ckTask::ORDER_MINUS_7, ckTask::ORDER_PLUS_3, false);

        for (s32 i = ckTask::ORDER_MINUS_8_FOR_SYSTEM; i <= ckTask::ORDER_PLUS_8_FOR_SYSTEM; i++)
        {
            if (i >= ckTask::ORDER_MINUS_7 && i <= ckTask::ORDER_PLUS_3)
            {
                ckAssert(!ckTaskMgr::isOrderActive(static_cast<ckTask::TaskOrder>(i)));
            }
            else
            {
                ckAssert(ckTaskMgr::isOrderActive(static_cast<ckTask::TaskOrder>(i)));
            }
        }

        s_update_flag = 0;

        ckTaskMgr::updateForSystem();
        ckAssert(s_update_flag == 0x40);

        ckTaskMgr::deleteOrder(ckTask::ORDER_MINUS_5, ckTask::ORDER_PLUS_7);
        ckAssert(s_dtor_flag == 0x7e);

        s_update_flag = 0;

        ckTaskMgr::updateForSystem();
        ckAssert(s_update_flag == 0x00);

        ckTaskMgr::setOrderActive(ckTask::ORDER_MINUS_7, ckTask::ORDER_PLUS_7, true);

        s_update_flag = 0;

        ckTaskMgr::updateForSystem();
        ckAssert(s_update_flag == 0x01);

        ckAssertThrow(ckTaskMgr::setOrderActive(ckTask::ORDER_PLUS_1, ckTask::ORDER_ZERO, false), ckTaskMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckTaskMgr::deleteOrder(ckTask::ORDER_PLUS_7, ckTask::ORDER_PLUS_6), ckTaskMgr::ExceptionInvalidArgument);

        ckTaskMgr::destroyFirst();
    }

    /*
        static u32 setNextTaskNameForSystem(const char* name)
        static void deleteTaskForSystem(ckTask* task)
    */
    {
        ckAssertThrow(ckTaskMgr::setNextTaskNameForSystem("TEST"), ckTaskMgr::ExceptionNotInitialized);
        ckAssertThrow(ckTaskMgr::deleteTaskForSystem(NULL), ckTaskMgr::ExceptionNotInitialized);

        ckTaskMgr::createAfterSys(60);

        const char* name = "TEST";
        ckTaskMgr::setNextTaskNameForSystem(name);
        ckTask* task = ckNew(TaskMgrTestClass)(ckTask::ORDER_ZERO, 0);
        ckAssert(task->getName() == name);

        ckAssert(ckTaskMgr::getFirstTaskN(ckTask::ORDER_ZERO) == task);
        ckAssert(ckTaskMgr::getLastTaskN(ckTask::ORDER_ZERO) == task);

        ckTaskMgr::deleteTaskForSystem(task);

        ckAssert(!ckTaskMgr::getFirstTaskN(ckTask::ORDER_ZERO));
        ckAssert(!ckTaskMgr::getLastTaskN(ckTask::ORDER_ZERO));

        ckAssertThrow(ckTaskMgr::setNextTaskNameForSystem(NULL), ckTaskMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckTaskMgr::deleteTaskForSystem(NULL), ckTaskMgr::ExceptionInvalidArgument);

        ckTaskMgr::destroyFirst();
    }

    /*
        static void resetFrameSkip()
        static bool isFrameSkipResetForSystem()
    */
    {
        ckAssertThrow(ckTaskMgr::resetFrameSkip(), ckTaskMgr::ExceptionNotInitialized);
        ckAssertThrow(ckTaskMgr::isFrameSkipResetForSystem(), ckTaskMgr::ExceptionNotInitialized);

        ckTaskMgr::createAfterSys(60);

        ckAssert(ckTaskMgr::isFrameSkipResetForSystem());
        ckAssert(!ckTaskMgr::isFrameSkipResetForSystem());
        ckAssert(!ckTaskMgr::isFrameSkipResetForSystem());

        ckTaskMgr::resetFrameSkip();
        ckAssert(ckTaskMgr::isFrameSkipResetForSystem());
        ckAssert(!ckTaskMgr::isFrameSkipResetForSystem());
        ckAssert(!ckTaskMgr::isFrameSkipResetForSystem());

        ckTaskMgr::destroyFirst();
    }
}
