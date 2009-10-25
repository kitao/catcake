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

#include "pg_private_macro.h"


bool pgTask::hasOrder() const
{
    return (m_tree.hasParent() && !m_tree.getParentN()->hasParent());
}


pgTask::TaskOrder pgTask::getOrder() const
{
    if (!hasOrder())
    {
        pgThrow(ExceptionInvalidCall);
    }

    return m_tree.getParentN()->getSelf()->m_order.getType();
}


bool pgTask::hasParent() const
{
    return (m_tree.hasParent() && m_tree.getParentN()->hasParent());
}


pgTask* pgTask::getParentN() const
{
    return hasParent() ? m_tree.getParentN()->getSelf() : NULL;
}


pgTask* pgTask::getPrevAllN() const
{
    pgTree<pgTask>* prev = m_tree.getPrevAllN();

    return (prev && prev->hasParent()) ? prev->getSelf() : NULL;
}


pgTask* pgTask::getNextAllN() const
{
    pgTree<pgTask>* next = m_tree.getNextAllN();

    return next ? next->getSelf() : NULL;
}


pgTask* pgTask::getPrevSiblingN() const
{
    pgTree<pgTask>* sibling = m_tree.getPrevSiblingN();

    return sibling ? sibling->getSelf() : NULL;
}


pgTask* pgTask::getNextSiblingN() const
{
    pgTree<pgTask>* sibling = m_tree.getNextSiblingN();

    return sibling ? sibling->getSelf() : NULL;
}


pgTask* pgTask::getLastDescendant() const
{
    return m_tree.getLastDescendant()->getSelf();
}


bool pgTask::hasChild() const
{
    return m_tree.hasChild();
}


pgTask* pgTask::getFirstChildN() const
{
    pgTree<pgTask>* child = m_tree.getFirstChildN();

    return child ? child->getSelf() : NULL;
}


pgTask* pgTask::getLastChildN() const
{
    pgTree<pgTask>* child = m_tree.getLastChildN();

    return child ? child->getSelf() : NULL;
}


const char* pgTask::getName() const
{
    return m_name;
}


u64 pgTask::getExecuteUsecTime() const
{
    return m_execute_time;
}


bool pgTask::isActive() const
{
    return m_flag.isOn(FLAG_ACTIVE);
}


void pgTask::setActive(bool is_active)
{
    m_flag.set(FLAG_ACTIVE, is_active);
}


void pgTask::onUpdate() {}


void pgTask::onMessage(pgID msg_id, pgMsg<4>&) {}


pgTask::pgTask(TaskOrder order)
{
    pgTaskMgr* ins = pgTaskMgr::instance();

    if (!ins->m_next_task_name)
    {
        pgThrow(ExceptionInvalidCall);
    }

    m_tree.init(this);
    m_flag.setOn(FLAG_ACTIVE);

    m_name = ins->m_next_task_name;
    m_execute_time = 0;

    ins->m_root_task[order].m_tree.addLast(&m_tree);

    ins->m_next_task_name = NULL;
}


pgTask::pgTask(pgTask* parent)
{
    pgTaskMgr* ins = pgTaskMgr::instance();

    if (!ins->m_next_task_name)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (!parent)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_tree.init(this);
    m_flag.setOn(FLAG_ACTIVE);

    m_name = ins->m_next_task_name;
    m_execute_time = 0;

    parent->m_tree.addLast(&m_tree);

    ins->m_next_task_name = NULL;
}


pgTask::~pgTask()
{
    pgTaskMgr* ins = pgTaskMgr::instance();

    if (m_tree.hasParent())
    {
        if (ins->m_next_task_name != reinterpret_cast<const char*>(this))
        {
            pgThrow(ExceptionInvalidCall);
        }

        ins->m_next_task_name = NULL;
    }
}


pgTask::pgTask()
{
    m_tree.init(this);
    m_flag.setOn(FLAG_ACTIVE);

    m_name = "";
    m_execute_time = 0;
}


PG_DEFINE_COPY_CONSTRUCTOR(pgTask)


PG_DEFINE_OPERATOR_EQUAL(pgTask)
