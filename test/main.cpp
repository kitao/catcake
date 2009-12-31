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
