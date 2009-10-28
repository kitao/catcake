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
