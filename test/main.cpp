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


#include "catcake_main.h"
#include "test.h"


#define MODULE_TEST_START \
    s_used_memory_size = ckMemMgr::getCurUsedMemorySize(); \
    \
    do

#define MODULE_TEST_END \
    while (false); \
    \
    ckAssert(ckMemMgr::getCurUsedMemorySize() == s_used_memory_size);

static u32 s_used_memory_size;


ckMain()
{
    /*
        ckMem
    */
    {
        ckMemMgrTest();
        ckMemUtilTest();
    }

    ckMemMgr::createFirst();

    /*
        ckMath
    */
    MODULE_TEST_START
    {
        ckMathTest();
        ckVecTest();
        ckMatTest();
        ckQuatTest();
        ckFixTest();
    }
    MODULE_TEST_END

    /*
        ckGen
    */
    MODULE_TEST_START
    {
        ckIDTest();
        ckTypeTest();
        ckFlagTest();
        ckListTest();
        ckTreeTest();
        ckBufTest();
        ckMapTest();
        ckStrTest();
        ckMsgTest();
    }
    MODULE_TEST_END

    /*
        ckCdt
    */
    MODULE_TEST_START
    {
        ckCdtAABBTest();
        ckCdtSphTest();
        ckCdtBoxTest();
        ckCdtTriTest();
        ckCdtRayTest();
        ckCdtTest();
    }
    MODULE_TEST_END

    /*
        ckSysMgr
    */
    MODULE_TEST_START
    {
        ckSysMgrTest();
    }
    MODULE_TEST_END

    ckSysMgr::createAfterMem("Catcake Test", 320, 240, ckSysMgr::FLAG_VARIABLE_SIZE);

    /*
        ckTask
    */
    MODULE_TEST_START
    {
        ckTaskMgrTest();
        ckTaskTest();
    }
    MODULE_TEST_END
    {
        ckTaskUtilTest(); // This function leaks memory due to some tests.
    }

    ckTaskMgr::createAfterSys(60);

    /*
        ckKey
    */
    MODULE_TEST_START
    {
        ckKeyMgrTest();
    }
    MODULE_TEST_END

    ckKeyMgr::createAfterTask();

    /*
        ckRes
    */
    MODULE_TEST_START
    {
        ckResMgrTest();
        ckResTest();
    }
    MODULE_TEST_END

    ckResMgr::createAfterTask();

    /*
        ckConf
    */
    MODULE_TEST_START
    {
        ckConfMgrTest();
        ckConfTest1();
        ckEntTest();
    }
    MODULE_TEST_END

    ckConfMgr::createAfterRes();

    /*
        ckDraw
    */
    MODULE_TEST_START
    {
        ckColTest();
        ckDrawMgrTest();
        ckScrTest();
        ckTexTest();
        ckFontTest();
        ckShdTest();
        ckLitTest();
        ckLtsTest();
        ckNodeTest();
        ckPrimTest();
        ckSprtTest();
        ckMdlDataTest();
        ckMdlTest();
        ckMotDataTest();
        ckMotTest();
        ckRendTest();

        ckRend_ShaderTest();
        ckRend_LightTest();
    }
    MODULE_TEST_END

    ckDrawMgr::createAfterRes();

    /*
        ckSnd
    */
    MODULE_TEST_START
    {
        ckSndMgrTest();
    }
    MODULE_TEST_END

    ckSndMgr::createAfterRes();

    /*
        ckDbg
    */
    MODULE_TEST_START
    {
        ckDbgMgrTest();
    }
    MODULE_TEST_END

    ckDbgMgr::createLast();

    /*
        ckUtil
    */
    MODULE_TEST_START
    {
        ckUtilTest();
    }
    MODULE_TEST_END

    ckTaskMgr::destroyFirst();
    ckDbgMgr::destroySecond();
    ckSndMgr::destroyBeforeRes();
    ckDrawMgr::destroyBeforeRes();
    ckKeyMgr::destroyBeforeSys();
    ckConfMgr::destroyBeforeRes();
    ckResMgr::destroyBeforeSys();
    ckSysMgr::destroyBeforeMem();
    ckMemMgr::destroyLast();

    /*
        ckConf
    */
    ckConfTest2();

    /*
        ckSysUtil
    */
    ckSysUtilTest();
}
