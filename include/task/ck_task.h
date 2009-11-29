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


class ckTaskMgr;


/*!
    @ingroup ckTask
    Task.
*/
class CK_API ckTask
{
    friend class ckTaskMgr;

public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);

    /*!
        The types of the task orders.
    */
    enum TaskOrder
    {
        ORDER_MINUS_8_FOR_SYSTEM, ORDER_MINUS_7, ORDER_MINUS_6, ORDER_MINUS_5, //
        ORDER_MINUS_4, ORDER_MINUS_3, ORDER_MINUS_2, ORDER_MINUS_1, //
        ORDER_ZERO, //
        ORDER_PLUS_1, ORDER_PLUS_2, ORDER_PLUS_3, ORDER_PLUS_4, //
        ORDER_PLUS_5, ORDER_PLUS_6, ORDER_PLUS_7, ORDER_PLUS_8_FOR_SYSTEM
    };

    /*!
        Returns whether this task has the order.
        @return Whether this task has the order.
    */
    bool hasOrder() const;

    /*!
        Retuens the order of this task.
        @return The order of this task.
    */
    TaskOrder getOrder() const;

    /*!
        Returns whether this task has the order.
        @return Whether this task has the order.
    */
    bool hasParent() const;

    /*!
        Returns the parent of this task. If the parent doesn't exist, returns NULL.
        @return The parent of this task.
    */
    ckTask* getParentN() const;

    /*!
        Returns the previous task. If the previous task doesn't exist, returns NULL.
        @return The previous task.
    */
    ckTask* getPrevAllN() const;

    /*!
        Returns the next task. If the next task doesn't exist, returns NULL.
        @return The next task.
    */
    ckTask* getNextAllN() const;

    /*!
        Returns the previous sibling of this task.
        If this task has no previous sibling, returns NULL.
        @return The previous sibling of this task.
    */
    ckTask* getPrevSiblingN() const;

    /*!
        Returns the next sibling of this task.
        If this task has no previous sibling, returns NULL.
        @return The previous sibling of this task.
    */
    ckTask* getNextSiblingN() const;

    /*!
        Returns the last descendant of this task.
        If this task has no descendant, returns this task.
        @return The last descendant of this task.
    */
    ckTask* getLastDescendant() const;

    /*!
        Returns whether this task has a child.
        @return Whether this task has a child.
    */
    bool hasChild() const;

    /*!
        Returns the first child of this task. If the first child doesn't exist, returns NULL.
        @return The first child of this task.
    */
    ckTask* getFirstChildN() const;

    /*!
        Returns the last child of this task. If the last child doesn't exist, returns NULL.
        @return The last child of this task.
    */
    ckTask* getLastChildN() const;

    /*!
        Returns the name of this task.
        @return The name of this task.
    */
    const char* getName() const;

    /*!
        Returns the process time of this task in usec.
        @return The process time of this task.
    */
    u64 getExecuteUsecTime() const;

    /*!
        Returns whether this task is active.
        @return Whether this task is active.
    */
    bool isActive() const;

    /*!
        Determines whether this task is active.
        @param[in] is_active whetehr this task is active.
    */
    void setActive(bool is_active);

    /*!
        Updates the frame of this task.
    */
    virtual void onUpdate();

    /*!
        Receives a message.
        @param[in] msg_id The ID of a message.
        @param[in] msg A message.
    */
    virtual void onMessage(ckID msg_id, ckMsg<4>& msg);

protected:
    ckTask(TaskOrder order);
    ckTask(ckTask* parent);
    virtual ~ckTask();

private:
    enum TaskFlag
    {
        FLAG_ACTIVE
    };

    ckTask();
    ckTask(const ckTask&);
    void operator=(const ckTask&);

    ckTree<ckTask> m_tree;
    ckType<u8, TaskOrder> m_order;
    ckFlag<u8, TaskFlag> m_flag;
    const char* m_name;
    u64 m_execute_time;
};
