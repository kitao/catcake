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
    TODO
*/
class CK_API ckTask
{
    friend class ckTaskMgr;

public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);

    /*!
        TODO
    */
    enum TaskOrder
    {
        ORDER_MINUS_8_FOR_SYSTEM, ORDER_MINUS_7, ORDER_MINUS_6, ORDER_MINUS_5, //
        ORDER_MINUS_4, ORDER_MINUS_3, ORDER_MINUS_2, ORDER_MINUS_1, //
        ORDER_ZERO, //
        ORDER_PLUS_1, ORDER_PLUS_2, ORDER_PLUS_3, ORDER_PLUS_4, //
        ORDER_PLUS_5, ORDER_PLUS_6, ORDER_PLUS_7, ORDER_PLUS_8_FOR_SYSTEM
    };

    bool hasOrder() const;
    TaskOrder getOrder() const;

    bool hasParent() const;
    ckTask* getParentN() const;

    ckTask* getPrevAllN() const;
    ckTask* getNextAllN() const;
    ckTask* getPrevSiblingN() const;
    ckTask* getNextSiblingN() const;
    ckTask* getLastDescendant() const;

    bool hasChild() const;
    ckTask* getFirstChildN() const;
    ckTask* getLastChildN() const;

    const char* getName() const;
    u64 getExecuteUsecTime() const;

    bool isEnabled() const;
    void setEnabled(bool is_enabled);

    virtual void onUpdate();
    virtual void onMessage(ckID msg_id, ckMsg<4>& msg);

protected:
    ckTask(TaskOrder order);
    ckTask(ckTask* parent);
    virtual ~ckTask();

private:
    enum TaskFlag
    {
        FLAG_ENABLED
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
