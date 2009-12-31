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


class TaskTestClass : public ckTask
{
public:
    TaskTestClass(ckTask::TaskOrder order, u8 no) : ckTask(order)
    {
        /*
            ckTask(TaskOrder order)
        */

        ckAssert(no < 8);

        m_no_flag = 1 << no;
        m_update_type = 0;

        s_ctor_flag |= m_no_flag;
    }

    TaskTestClass(ckTask* parent, u8 no) : ckTask(parent)
    {
        /*
            ckTask(ckTask* parent)
        */

        m_no_flag = 1 << no;
        m_update_type = 0;

        s_ctor_flag |= m_no_flag;
    }

    virtual ~TaskTestClass()
    {
        /*
            virtual ~ckTask()
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

        ckAssert(m_no_flag > s_update_flag);

        s_update_flag |= m_no_flag;

        switch (m_update_type)
        {
        case 0:
            ckSysMgr::sleepUsec(10000);
            break;

        case 1:
            ckDeleteTask(this);
            return;

        case 2:
            ckTaskMgr::deleteOrder(ckTask::ORDER_MINUS_3, ckTask::ORDER_PLUS_3);
            break;
        }
    }

    virtual void onMessage(ckID msg_id, ckMsg<4>& msg)
    {
        /*
            virtual void onMessage(ckID msg_id, ckMsg<4>& msg)
        */

        ckAssert(msg_id == ckID_("TEST"));
        ckAssert(msg.getParam<u16>(0) == 123 && msg.getParam<u16>(1) == 456 && msg.getParam<u16>(2) == 789 && msg.getParam<s8>(3) == -10);
        ckAssert(m_no_flag > s_message_flag);

        s_message_flag |= m_no_flag;
    }

private:
    u8 m_no_flag;
    u8 m_update_type;
};


void ckTaskTest()
{
    /*
        bool hasOrder() const
        TaskOrder getOrder() const
        bool hasParent() const
        ckTask* getParentN() const
        ckTask* getPrevAllN() const
        ckTask* getNextAllN() const
        ckTask* getPrevSiblingN() const
        ckTask* getNextSiblingN() const
        ckTask* getLastDescendant() const
        bool hasChild() const
        ckTask* getFirstChildN() const
        ckTask* getLastChildN() const
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

        ckTaskMgr::createAfterSys(60);

        ckTask* task1 = ckNewTask(TaskTestClass)(ckTask::ORDER_MINUS_4, 0);
        ckTask* task2 = ckNewTask(TaskTestClass)(ckTask::ORDER_MINUS_3, 1);
        ckTask* task3 = ckNewTask(TaskTestClass)(task2, 2);
        ckTask* task4 = ckNewTask(TaskTestClass)(task3, 3);
        ckTask* task5 = ckNewTask(TaskTestClass)(task3, 4);
        ckTask* task6 = ckNewTask(TaskTestClass)(task2, 5);
        ckTask* task7 = ckNewTask(TaskTestClass)(task6, 6);
        ckTask* task8 = ckNewTask(TaskTestClass)(ckTask::ORDER_MINUS_2, 7);

        ckAssert(s_ctor_flag == 0xff && s_dtor_flag == 0x00 && s_update_flag == 0x00 && s_message_flag == 0x00);

        ckAssert(task1->hasOrder() && task1->getOrder() == ckTask::ORDER_MINUS_4);
        ckAssert(!task1->hasParent() && !task1->getParentN());
        ckAssert(!task1->getPrevAllN() && !task1->getNextAllN());
        ckAssert(!task1->getPrevSiblingN() && !task1->getNextSiblingN());
        ckAssert(task1->getLastDescendant() == task1);
        ckAssert(!task1->hasChild() && !task1->getFirstChildN() && !task1->getLastChildN());
        ckAssert(isEqual(task1->getName(), __FILE__));
        ckAssert(task1->getExecuteUsecTime() == 0);
        ckAssert(task1->isActive());

        ckAssert(task2->hasOrder() && task2->getOrder() == ckTask::ORDER_MINUS_3);
        ckAssert(!task2->hasParent() && !task2->getParentN());
        ckAssert(!task2->getPrevAllN() && task2->getNextAllN() == task3);
        ckAssert(!task2->getPrevSiblingN() && !task2->getNextSiblingN());
        ckAssert(task2->getLastDescendant() == task7);
        ckAssert(task2->hasChild() && task2->getFirstChildN() == task3 && task2->getLastChildN() == task6);
        ckAssert(isEqual(task2->getName(), __FILE__));
        ckAssert(task2->getExecuteUsecTime() == 0);
        ckAssert(task2->isActive());

        ckAssert(!task3->hasOrder());
        ckAssertThrow(task3->getOrder(), ckTask::ExceptionInvalidCall);
        ckAssert(task3->hasParent() && task3->getParentN() == task2);
        ckAssert(task3->getPrevAllN() == task2 && task3->getNextAllN() == task4);
        ckAssert(!task3->getPrevSiblingN() && task3->getNextSiblingN() == task6);
        ckAssert(task3->getLastDescendant() == task5);
        ckAssert(task3->hasChild() && task3->getFirstChildN() == task4 && task3->getLastChildN() == task5);
        ckAssert(isEqual(task3->getName(), __FILE__));
        ckAssert(task3->getExecuteUsecTime() == 0);
        ckAssert(task3->isActive());

        ckAssert(!task4->hasOrder());
        ckAssertThrow(task4->getOrder(), ckTask::ExceptionInvalidCall);
        ckAssert(task4->hasParent() && task4->getParentN() == task3);
        ckAssert(task4->getPrevAllN() == task3 && task4->getNextAllN() == task5);
        ckAssert(!task4->getPrevSiblingN() && task4->getNextSiblingN() == task5);
        ckAssert(task4->getLastDescendant() == task4);
        ckAssert(!task4->hasChild() && !task4->getFirstChildN() && !task4->getLastChildN());
        ckAssert(isEqual(task4->getName(), __FILE__));
        ckAssert(task4->getExecuteUsecTime() == 0);
        ckAssert(task4->isActive());

        ckAssert(!task7->hasOrder());
        ckAssertThrow(task7->getOrder(), ckTask::ExceptionInvalidCall);
        ckAssert(task7->hasParent() && task7->getParentN() == task6);
        ckAssert(task7->getPrevAllN() == task6 && !task7->getNextAllN());
        ckAssert(!task7->getPrevSiblingN() && !task7->getNextSiblingN());
        ckAssert(task7->getLastDescendant() == task7);
        ckAssert(!task7->hasChild() && !task7->getFirstChildN() && !task7->getLastChildN());
        ckAssert(isEqual(task7->getName(), __FILE__));
        ckAssert(task7->getExecuteUsecTime() == 0);
        ckAssert(task7->isActive());

        ckTaskMgr::updateForSystem();
        ckAssert(s_ctor_flag == 0xff && s_dtor_flag == 0x00 && s_update_flag == 0xff && s_message_flag == 0x00);

        ckAssert(task1->getExecuteUsecTime() > 0);
        ckAssert(task2->getExecuteUsecTime() > 0);
        ckAssert(task3->getExecuteUsecTime() > 0);
        ckAssert(task4->getExecuteUsecTime() > 0);
        ckAssert(task5->getExecuteUsecTime() > 0);
        ckAssert(task6->getExecuteUsecTime() > 0);
        ckAssert(task7->getExecuteUsecTime() > 0);
        ckAssert(task8->getExecuteUsecTime() > 0);

        task3->setActive(false);
        ckAssert(!task3->isActive());

        s_update_flag = 0;

        ckTaskMgr::updateForSystem();
        ckAssert(s_ctor_flag == 0xff && s_dtor_flag == 0x00 && s_update_flag == 0xe3 && s_message_flag == 0x00);

        ckMsg<4> msg;
        msg.setParam<u16>(0, 123);
        msg.setParam<u16>(1, 456);
        msg.setParam<u16>(2, 789);
        msg.setParam<s8>(3, -10);

        ckTaskMgr::sendMessage(ckID_("TEST"), msg);
        ckAssert(s_ctor_flag == 0xff && s_dtor_flag == 0x00 && s_update_flag == 0xe3 && s_message_flag == 0xff);

        task3->setActive(true);
        dynamic_cast<TaskTestClass*>(task3)->setUpdateType(1);
        dynamic_cast<TaskTestClass*>(task6)->setUpdateType(2);

        s_update_flag = 0;

        ckTaskMgr::updateForSystem();
        ckAssert(s_ctor_flag == 0xff && s_dtor_flag == 0xfe && s_update_flag == 0x27 && s_message_flag == 0xff);

        ckTaskMgr::destroyFirst();
    }
}
