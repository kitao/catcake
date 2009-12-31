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


class ckTaskMgr;


/*!
    @ingroup ckTask
    A task, which realizes non-preemptive multi task.
*/
class CK_API ckTask
{
    friend class ckTaskMgr;

public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    //! @endcond

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
    //! @cond
    ckTask(TaskOrder order);
    ckTask(ckTask* parent);
    virtual ~ckTask();
    //! @endcond

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
