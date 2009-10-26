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
