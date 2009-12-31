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


static bool s_is_new_task_in_dtor;
static ckTask* s_delete_task_in_dtor;


class TaskUtilTestClass : public ckTask
{
public:
    TaskUtilTestClass(ckTask::TaskOrder order) : ckTask(order) {}

    TaskUtilTestClass(ckTask* parent) : ckTask(parent) {}

    virtual ~TaskUtilTestClass()
    {
        if (s_is_new_task_in_dtor)
        {
            ckNewTask(TaskUtilTestClass)(ckTask::ORDER_ZERO);
        }

        if (s_delete_task_in_dtor)
        {
            ckDeleteTask(s_delete_task_in_dtor);
        }
    }
};


void ckTaskUtilTest()
{
    /*
        ckNewTask(type)
        ckDeleteTask(task)
    */
    {
        s_is_new_task_in_dtor = false;
        s_delete_task_in_dtor = NULL;

        ckAssertThrow(ckNewTask(TaskUtilTestClass)(ckTask::ORDER_ZERO), ckTaskMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDeleteTask(reinterpret_cast<ckTask*>(1)), ckTaskMgr::ExceptionNotInitialized);

        ckTaskMgr::createAfterSys(60);

        ckTask* task1 = ckNewTask(TaskUtilTestClass)(ckTask::ORDER_ZERO);
        ckTask* task2 = ckNewTask(TaskUtilTestClass)(ckTask::ORDER_ZERO);

        ckAssertThrow(ckNew(TaskUtilTestClass)(ckTask::ORDER_ZERO), ckTask::ExceptionInvalidCall);
        // This test leaks memory due to throw an exception in the constructor of the test class.

        ckDeleteTask(task1);

        task1 = ckNewTask(TaskUtilTestClass)(ckTask::ORDER_ZERO);

        s_is_new_task_in_dtor = true;
        ckAssertThrow(ckDeleteTask(task1), ckTaskMgr::ExceptionNewTaskInDestructor);
        s_is_new_task_in_dtor = false;

        task1 = ckNewTask(TaskUtilTestClass)(ckTask::ORDER_ZERO);

        s_delete_task_in_dtor = task2;
        ckAssertThrow(ckDeleteTask(task1), ckTaskMgr::ExceptionDeleteTaskInDestructor);
        s_delete_task_in_dtor = NULL;

        ckAssertThrow(ckDelete(task2, TaskUtilTestClass), ckTask::ExceptionInvalidCall);
        // This test leaks memory due to throw an exception in the destructor of the test class.

        ckAssertThrow(ckDeleteTask(NULL), ckTaskMgr::ExceptionInvalidArgument);

        ckTaskMgr::destroyFirst();
    }
}
