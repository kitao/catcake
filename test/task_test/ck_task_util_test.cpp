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


static bool s_is_new_task_in_dtor;
static pgTask* s_delete_task_in_dtor;


class TaskUtilTestClass : public pgTask
{
public:
    TaskUtilTestClass(pgTask::TaskOrder order) : pgTask(order) {}

    TaskUtilTestClass(pgTask* parent) : pgTask(parent) {}

    virtual ~TaskUtilTestClass()
    {
        if (s_is_new_task_in_dtor)
        {
            pgNewTask(TaskUtilTestClass)(pgTask::ORDER_ZERO);
        }

        if (s_delete_task_in_dtor)
        {
            pgDeleteTask(s_delete_task_in_dtor);
        }
    }
};


void pgTaskUtilTest()
{
    /*
        pgNewTask(type)
        pgDeleteTask(task)
    */
    {
        s_is_new_task_in_dtor = false;
        s_delete_task_in_dtor = NULL;

        pgAssertThrow(pgNewTask(TaskUtilTestClass)(pgTask::ORDER_ZERO), pgTaskMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDeleteTask(reinterpret_cast<pgTask*>(1)), pgTaskMgr::ExceptionNotInitialized);

        pgTaskMgr::createAfterSys(60);

        pgTask* task1 = pgNewTask(TaskUtilTestClass)(pgTask::ORDER_ZERO);
        pgTask* task2 = pgNewTask(TaskUtilTestClass)(pgTask::ORDER_ZERO);

        pgAssertThrow(pgNew(TaskUtilTestClass)(pgTask::ORDER_ZERO), pgTask::ExceptionInvalidCall);
        // This test leaks memory due to throw an exception in the constructor of the test class.

        pgDeleteTask(task1);

        task1 = pgNewTask(TaskUtilTestClass)(pgTask::ORDER_ZERO);

        s_is_new_task_in_dtor = true;
        pgAssertThrow(pgDeleteTask(task1), pgTaskMgr::ExceptionNewTaskInDestructor);
        s_is_new_task_in_dtor = false;

        task1 = pgNewTask(TaskUtilTestClass)(pgTask::ORDER_ZERO);

        s_delete_task_in_dtor = task2;
        pgAssertThrow(pgDeleteTask(task1), pgTaskMgr::ExceptionDeleteTaskInDestructor);
        s_delete_task_in_dtor = NULL;

        pgAssertThrow(pgDelete(task2, TaskUtilTestClass), pgTask::ExceptionInvalidCall);
        // This test leaks memory due to throw an exception in the destructor of the test class.

        pgAssertThrow(pgDeleteTask(NULL), pgTaskMgr::ExceptionInvalidArgument);

        pgTaskMgr::destroyFirst();
    }
}
