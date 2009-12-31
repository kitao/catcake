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

#include "ck_private_macro.h"


bool ckTask::hasOrder() const
{
    return (m_tree.hasParent() && !m_tree.getParentN()->hasParent());
}


ckTask::TaskOrder ckTask::getOrder() const
{
    if (!hasOrder())
    {
        ckThrow(ExceptionInvalidCall);
    }

    return m_tree.getParentN()->getSelf()->m_order.getType();
}


bool ckTask::hasParent() const
{
    return (m_tree.hasParent() && m_tree.getParentN()->hasParent());
}


ckTask* ckTask::getParentN() const
{
    return hasParent() ? m_tree.getParentN()->getSelf() : NULL;
}


ckTask* ckTask::getPrevAllN() const
{
    ckTree<ckTask>* prev = m_tree.getPrevAllN();

    return (prev && prev->hasParent()) ? prev->getSelf() : NULL;
}


ckTask* ckTask::getNextAllN() const
{
    ckTree<ckTask>* next = m_tree.getNextAllN();

    return next ? next->getSelf() : NULL;
}


ckTask* ckTask::getPrevSiblingN() const
{
    ckTree<ckTask>* sibling = m_tree.getPrevSiblingN();

    return sibling ? sibling->getSelf() : NULL;
}


ckTask* ckTask::getNextSiblingN() const
{
    ckTree<ckTask>* sibling = m_tree.getNextSiblingN();

    return sibling ? sibling->getSelf() : NULL;
}


ckTask* ckTask::getLastDescendant() const
{
    return m_tree.getLastDescendant()->getSelf();
}


bool ckTask::hasChild() const
{
    return m_tree.hasChild();
}


ckTask* ckTask::getFirstChildN() const
{
    ckTree<ckTask>* child = m_tree.getFirstChildN();

    return child ? child->getSelf() : NULL;
}


ckTask* ckTask::getLastChildN() const
{
    ckTree<ckTask>* child = m_tree.getLastChildN();

    return child ? child->getSelf() : NULL;
}


const char* ckTask::getName() const
{
    return m_name;
}


u64 ckTask::getExecuteUsecTime() const
{
    return m_execute_time;
}


bool ckTask::isActive() const
{
    return m_flag.isOn(FLAG_ACTIVE);
}


void ckTask::setActive(bool is_active)
{
    m_flag.set(FLAG_ACTIVE, is_active);
}


void ckTask::onUpdate() {}


void ckTask::onMessage(ckID msg_id, ckMsg<4>&) {}


ckTask::ckTask(TaskOrder order)
{
    ckTaskMgr* ins = ckTaskMgr::instance();

    if (!ins->m_next_task_name)
    {
        ckThrow(ExceptionInvalidCall);
    }

    m_tree.init(this);
    m_flag.setOn(FLAG_ACTIVE);

    m_name = ins->m_next_task_name;
    m_execute_time = 0;

    ins->m_root_task[order].m_tree.addLast(&m_tree);

    ins->m_next_task_name = NULL;
}


ckTask::ckTask(ckTask* parent)
{
    ckTaskMgr* ins = ckTaskMgr::instance();

    if (!ins->m_next_task_name)
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (!parent)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_tree.init(this);
    m_flag.setOn(FLAG_ACTIVE);

    m_name = ins->m_next_task_name;
    m_execute_time = 0;

    parent->m_tree.addLast(&m_tree);

    ins->m_next_task_name = NULL;
}


ckTask::~ckTask()
{
    ckTaskMgr* ins = ckTaskMgr::instance();

    if (m_tree.hasParent())
    {
        if (ins->m_next_task_name != reinterpret_cast<const char*>(this))
        {
            ckThrow(ExceptionInvalidCall);
        }

        ins->m_next_task_name = NULL;
    }
}


ckTask::ckTask()
{
    m_tree.init(this);
    m_flag.setOn(FLAG_ACTIVE);

    m_name = "";
    m_execute_time = 0;
}


CK_DEFINE_COPY_CONSTRUCTOR(ckTask)


CK_DEFINE_OPERATOR_EQUAL(ckTask)
