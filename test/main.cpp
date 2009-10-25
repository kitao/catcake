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


#include "pogolyn_main.h"
#include "test.h"


#define MODULE_TEST_START \
    s_used_memory_size = pgMemMgr::getCurUsedMemorySize(); \
    \
    do

#define MODULE_TEST_END \
    while (false); \
    \
    pgAssert(pgMemMgr::getCurUsedMemorySize() == s_used_memory_size);

static u32 s_used_memory_size;


pgMain()
{
    /*
        pgMem
    */
    {
        pgMemMgrTest();
        pgMemUtilTest();
    }

    pgMemMgr::createFirst();

    /*
        pgMath
    */
    MODULE_TEST_START
    {
        pgMathTest();
        pgVecTest();
        pgMatTest();
        pgQuatTest();
        pgFixTest();
    }
    MODULE_TEST_END

    /*
        pgGen
    */
    MODULE_TEST_START
    {
        pgIDTest();
        pgTypeTest();
        pgFlagTest();
        pgListTest();
        pgTreeTest();
        pgBufTest();
        pgMapTest();
        pgStrTest();
        pgMsgTest();
    }
    MODULE_TEST_END

    /*
        pgCdt
    */
    MODULE_TEST_START
    {
        pgCdtAABBTest();
        pgCdtSphTest();
        pgCdtBoxTest();
        pgCdtTriTest();
        pgCdtRayTest();
        pgCdtTest();
    }
    MODULE_TEST_END

    /*
        pgSysMgr
    */
    MODULE_TEST_START
    {
        pgSysMgrTest();
    }
    MODULE_TEST_END

    pgSysMgr::createAfterMem("Pogolyn Test", 320, 240, pgSysMgr::FLAG_VARIABLE_SIZE);

    /*
        pgTask
    */
    MODULE_TEST_START
    {
        pgTaskMgrTest();
        pgTaskTest();
    }
    MODULE_TEST_END
    {
        pgTaskUtilTest(); // This function leaks memory due to some tests.
    }

    pgTaskMgr::createAfterSys(60);

    /*
        pgKey
    */
    MODULE_TEST_START
    {
        pgKeyMgrTest();
    }
    MODULE_TEST_END

    pgKeyMgr::createAfterTask();

    /*
        pgRes
    */
    MODULE_TEST_START
    {
        pgResMgrTest();
        pgResTest();
    }
    MODULE_TEST_END

    pgResMgr::createAfterTask();

    /*
        pgConf
    */
    MODULE_TEST_START
    {
        pgConfMgrTest();
        pgConfTest1();
        pgEntTest();
    }
    MODULE_TEST_END

    pgConfMgr::createAfterRes();

    /*
        pgDraw
    */
    MODULE_TEST_START
    {
        pgColTest();
        pgDrawMgrTest();
        pgScrTest();
        pgTexTest();
        pgFontTest();
        pgShdTest();
        pgLitTest();
        pgLtsTest();
        pgNodeTest();
        pgPrimTest();
        pgSprtTest();
        pgMdlDataTest();
        pgMdlTest();
        pgMotDataTest();
        pgMotTest();
        pgRendTest();

        pgRend_ShaderTest();
        pgRend_LightTest();
    }
    MODULE_TEST_END

    pgDrawMgr::createAfterRes();

    /*
        pgSnd
    */
    MODULE_TEST_START
    {
        pgSndMgrTest();
    }
    MODULE_TEST_END

    pgSndMgr::createAfterRes();

    /*
        pgDbg
    */
    MODULE_TEST_START
    {
        pgDbgMgrTest();
    }
    MODULE_TEST_END

    pgDbgMgr::createLast();

    /*
        pgUtil
    */
    MODULE_TEST_START
    {
        pgUtilTest();
    }
    MODULE_TEST_END

    pgTaskMgr::destroyFirst();
    pgDbgMgr::destroySecond();
    pgSndMgr::destroyBeforeRes();
    pgDrawMgr::destroyBeforeRes();
    pgKeyMgr::destroyBeforeSys();
    pgConfMgr::destroyBeforeRes();
    pgResMgr::destroyBeforeSys();
    pgSysMgr::destroyBeforeMem();
    pgMemMgr::destroyLast();

    /*
        pgConf
    */
    pgConfTest2();

    /*
        pgSysUtil
    */
    pgSysUtilTest();
}
