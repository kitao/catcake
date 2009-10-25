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


class TaskTestClass : public pgTask
{
public:
    TaskTestClass(pgTask::TaskOrder order, u8 no) : pgTask(order)
    {
        /*
            pgTask(TaskOrder order)
        */

        pgAssert(no < 8);

        m_no_flag = 1 << no;
        m_update_type = 0;

        s_ctor_flag |= m_no_flag;
    }

    TaskTestClass(pgTask* parent, u8 no) : pgTask(parent)
    {
        /*
            pgTask(pgTask* parent)
        */

        m_no_flag = 1 << no;
        m_update_type = 0;

        s_ctor_flag |= m_no_flag;
    }

    virtual ~TaskTestClass()
    {
        /*
            virtual ~pgTask()
        */

        s_dtor_flag |= m_no_flag;
    }

    void setUpdateType(u8 update_type)
    {
        m_update_type = update_type;
    }

    virtual void onUpdate()
    {
        /*
            virtual void onUpdate()
        */

        pgAssert(m_no_flag > s_update_flag);

        s_update_flag |= m_no_flag;

        switch (m_update_type)
        {
        case 0:
            pgSysMgr::sleepUsec(10000);
            break;

        case 1:
            pgDeleteTask(this);
            return;

        case 2:
            pgTaskMgr::deleteOrder(pgTask::ORDER_MINUS_3, pgTask::ORDER_PLUS_3);
            break;
        }
    }

    virtual void onMessage(pgID msg_id, pgMsg<4>& msg)
    {
        /*
            virtual void onMessage(pgID msg_id, pgMsg<4>& msg)
        */

        pgAssert(msg_id == pgID_("TEST"));
        pgAssert(msg.getParam<u16>(0) == 123 && msg.getParam<u16>(1) == 456 && msg.getParam<u16>(2) == 789 && msg.getParam<s8>(3) == -10);
        pgAssert(m_no_flag > s_message_flag);

        s_message_flag |= m_no_flag;
    }

private:
    u8 m_no_flag;
    u8 m_update_type;
};


void pgTaskTest()
{
    /*
        bool hasOrder() const
        TaskOrder getOrder() const
        bool hasParent() const
        pgTask* getParentN() const
        pgTask* getPrevAllN() const
        pgTask* getNextAllN() const
        pgTask* getPrevSiblingN() const
        pgTask* getNextSiblingN() const
        pgTask* getLastDescendant() const
        bool hasChild() const
        pgTask* getFirstChildN() const
        pgTask* getLastChildN() const
        const char* getName() const
        u64 getExecuteUsecTime() const
        bool isActive() const
        void setActive(bool is_active)
    */
    {
        s_ctor_flag = 0x00;
        s_dtor_flag = 0x00;
        s_update_flag = 0x00;
        s_message_flag = 0x00;

        pgTaskMgr::createAfterSys(60);

        pgTask* task1 = pgNewTask(TaskTestClass)(pgTask::ORDER_MINUS_4, 0);
        pgTask* task2 = pgNewTask(TaskTestClass)(pgTask::ORDER_MINUS_3, 1);
        pgTask* task3 = pgNewTask(TaskTestClass)(task2, 2);
        pgTask* task4 = pgNewTask(TaskTestClass)(task3, 3);
        pgTask* task5 = pgNewTask(TaskTestClass)(task3, 4);
        pgTask* task6 = pgNewTask(TaskTestClass)(task2, 5);
        pgTask* task7 = pgNewTask(TaskTestClass)(task6, 6);
        pgTask* task8 = pgNewTask(TaskTestClass)(pgTask::ORDER_MINUS_2, 7);

        pgAssert(s_ctor_flag == 0xff && s_dtor_flag == 0x00 && s_update_flag == 0x00 && s_message_flag == 0x00);

        pgAssert(task1->hasOrder() && task1->getOrder() == pgTask::ORDER_MINUS_4);
        pgAssert(!task1->hasParent() && !task1->getParentN());
        pgAssert(!task1->getPrevAllN() && !task1->getNextAllN());
        pgAssert(!task1->getPrevSiblingN() && !task1->getNextSiblingN());
        pgAssert(task1->getLastDescendant() == task1);
        pgAssert(!task1->hasChild() && !task1->getFirstChildN() && !task1->getLastChildN());
        pgAssert(isEqual(task1->getName(), __FILE__));
        pgAssert(task1->getExecuteUsecTime() == 0);
        pgAssert(task1->isActive());

        pgAssert(task2->hasOrder() && task2->getOrder() == pgTask::ORDER_MINUS_3);
        pgAssert(!task2->hasParent() && !task2->getParentN());
        pgAssert(!task2->getPrevAllN() && task2->getNextAllN() == task3);
        pgAssert(!task2->getPrevSiblingN() && !task2->getNextSiblingN());
        pgAssert(task2->getLastDescendant() == task7);
        pgAssert(task2->hasChild() && task2->getFirstChildN() == task3 && task2->getLastChildN() == task6);
        pgAssert(isEqual(task2->getName(), __FILE__));
        pgAssert(task2->getExecuteUsecTime() == 0);
        pgAssert(task2->isActive());

        pgAssert(!task3->hasOrder());
        pgAssertThrow(task3->getOrder(), pgTask::ExceptionInvalidCall);
        pgAssert(task3->hasParent() && task3->getParentN() == task2);
        pgAssert(task3->getPrevAllN() == task2 && task3->getNextAllN() == task4);
        pgAssert(!task3->getPrevSiblingN() && task3->getNextSiblingN() == task6);
        pgAssert(task3->getLastDescendant() == task5);
        pgAssert(task3->hasChild() && task3->getFirstChildN() == task4 && task3->getLastChildN() == task5);
        pgAssert(isEqual(task3->getName(), __FILE__));
        pgAssert(task3->getExecuteUsecTime() == 0);
        pgAssert(task3->isActive());

        pgAssert(!task4->hasOrder());
        pgAssertThrow(task4->getOrder(), pgTask::ExceptionInvalidCall);
        pgAssert(task4->hasParent() && task4->getParentN() == task3);
        pgAssert(task4->getPrevAllN() == task3 && task4->getNextAllN() == task5);
        pgAssert(!task4->getPrevSiblingN() && task4->getNextSiblingN() == task5);
        pgAssert(task4->getLastDescendant() == task4);
        pgAssert(!task4->hasChild() && !task4->getFirstChildN() && !task4->getLastChildN());
        pgAssert(isEqual(task4->getName(), __FILE__));
        pgAssert(task4->getExecuteUsecTime() == 0);
        pgAssert(task4->isActive());

        pgAssert(!task7->hasOrder());
        pgAssertThrow(task7->getOrder(), pgTask::ExceptionInvalidCall);
        pgAssert(task7->hasParent() && task7->getParentN() == task6);
        pgAssert(task7->getPrevAllN() == task6 && !task7->getNextAllN());
        pgAssert(!task7->getPrevSiblingN() && !task7->getNextSiblingN());
        pgAssert(task7->getLastDescendant() == task7);
        pgAssert(!task7->hasChild() && !task7->getFirstChildN() && !task7->getLastChildN());
        pgAssert(isEqual(task7->getName(), __FILE__));
        pgAssert(task7->getExecuteUsecTime() == 0);
        pgAssert(task7->isActive());

        pgTaskMgr::updateForSystem();
        pgAssert(s_ctor_flag == 0xff && s_dtor_flag == 0x00 && s_update_flag == 0xff && s_message_flag == 0x00);

        pgAssert(task1->getExecuteUsecTime() > 0);
        pgAssert(task2->getExecuteUsecTime() > 0);
        pgAssert(task3->getExecuteUsecTime() > 0);
        pgAssert(task4->getExecuteUsecTime() > 0);
        pgAssert(task5->getExecuteUsecTime() > 0);
        pgAssert(task6->getExecuteUsecTime() > 0);
        pgAssert(task7->getExecuteUsecTime() > 0);
        pgAssert(task8->getExecuteUsecTime() > 0);

        task3->setActive(false);
        pgAssert(!task3->isActive());

        s_update_flag = 0;

        pgTaskMgr::updateForSystem();
        pgAssert(s_ctor_flag == 0xff && s_dtor_flag == 0x00 && s_update_flag == 0xe3 && s_message_flag == 0x00);

        pgMsg<4> msg;
        msg.setParam<u16>(0, 123);
        msg.setParam<u16>(1, 456);
        msg.setParam<u16>(2, 789);
        msg.setParam<s8>(3, -10);

        pgTaskMgr::sendMessage(pgID_("TEST"), msg);
        pgAssert(s_ctor_flag == 0xff && s_dtor_flag == 0x00 && s_update_flag == 0xe3 && s_message_flag == 0xff);

        task3->setActive(true);
        dynamic_cast<TaskTestClass*>(task3)->setUpdateType(1);
        dynamic_cast<TaskTestClass*>(task6)->setUpdateType(2);

        s_update_flag = 0;

        pgTaskMgr::updateForSystem();
        pgAssert(s_ctor_flag == 0xff && s_dtor_flag == 0xfe && s_update_flag == 0x27 && s_message_flag == 0xff);

        pgTaskMgr::destroyFirst();
    }
}
