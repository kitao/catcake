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


class SampleTask : public ckTask
{
public:
    SampleTask();
    virtual void onUpdate();
};


SampleTask::SampleTask() : ckTask(ORDER_ZERO) {}


void SampleTask::onUpdate()
{
    ckError("Catcake test is complete.");
}


void ckSysUtilTest()
{
    /*
        ckReadLittleEndian(data, type)
        ckWriteLittleEndian(dest, value, type)
    */
    {
        static const u8 data1[] =
        {
            0x15, 0xcd, 0x5b, 0x07
        };

        static const u8 data2[] =
        {
            0x2e, 0xfb
        };

        static const u8 data3[] =
        {
            0x00, 0x40, 0x9a, 0x44
        };

        ckAssert(ckReadLittleEndian(data1, u32) == 123456789);
        ckAssert(ckReadLittleEndian(data2, s16) == -1234);
        ckAssert(isEqual(ckReadLittleEndian(data3, r32), 1234.0f));

        u8 data4[4], data5[3];

        ckWriteLittleEndian(data4, -54321, s32);
        ckAssert(ckReadLittleEndian(data4, s32) == -54321);

        ckWriteLittleEndian(data5, 4321, u16);
        ckAssert(ckReadLittleEndian(data5, u16) == 4321);
    }


    /*
        void ckCreateCatcake(const char* title, u16 width, u16 height, u16 aim_fps, u16 sys_flag = 0)
        void ckDestroyCatcake()
        void ckStartCatcake()
        void ckExitCatcake()
        void ckError(const char* msg, ...)
    */
    {
        ckCreateCatcake("Catcake Test", 640, 480, 60, ckSysMgr::FLAG_VARIABLE_SIZE);

        ckNewTask(SampleTask);

        ckStartCatcake();

        ckDestroyCatcake();
        ckDestroyCatcake();
    }
}
