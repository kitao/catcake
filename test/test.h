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


#ifndef TEST_H_
#define TEST_H_


#include "pogolyn.h"


#define TEST_DATA_DIR "../../../../test/test_data/"


#define pgAssert(expression) \
    do \
    { \
        if (!(expression)) \
        { \
            pgError("Assertion Failed: %s(%d)", __FILE__, __LINE__); \
        } \
    } \
    while (false)


#ifdef PG_NO_THROW_EXCEPTION
#define pgAssertThrow(...)
#else
#define pgAssertThrow(expression, ...) \
    do \
    { \
        pgTry \
        { \
            expression; \
            pgAssert(false); \
        } \
        pgCatch(__VA_ARGS__ e) \
        { \
            pgSysMgr::printf("Caught Exception: %s in %s(%d)\n", e.getException(), e.getFile(), e.getLine()); \
        } \
    } \
    while (false)
#endif


template<class T> bool isEqual(const T* str1, const T* str2)
{
    for (s32 i = 0; ; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }

        if (str2[i] == 0)
        {
            return true;
        }
    }

    return false;
}


/*
    TestUtil
*/
bool isEqual(r32 a, r32 b);
bool isEqual(const pgVec& vec1, const pgVec& vec2);
bool isEqual(const pgMat& mat1, const pgMat& mat2);

/*
    MemTest
*/
void pgMemMgrTest();
void pgMemUtilTest();

/*
    MathTest
*/
void pgMathTest();
void pgVecTest();
void pgMatTest();
void pgQuatTest();
void pgFixTest();

/*
    GenTest
*/
void pgIDTest();
void pgTypeTest();
void pgFlagTest();
void pgListTest();
void pgTreeTest();
void pgBufTest();
void pgMapTest();
void pgStrTest();
void pgMsgTest();

/*
    CdtTest
*/
void pgCdtAABBTest();
void pgCdtSphTest();
void pgCdtBoxTest();
void pgCdtTriTest();
void pgCdtRayTest();
void pgCdtTest();

/*
    SysTest
*/
void pgSysMgrTest();
void pgSysUtilTest();

/*
    TaskTest
*/
void pgTaskMgrTest();
void pgTaskUtilTest();
void pgTaskTest();

/*
    KeyTest
*/
void pgKeyMgrTest();

/*
    ResTest
*/
void pgResMgrTest();
void pgResTest();

/*
    ConfTest
*/
void pgConfMgrTest();
void pgConfTest1();
void pgConfTest2();
void pgEntTest();

/*
    DrawTest
*/
void pgColTest();
void pgDrawMgrTest();
void pgScrTest();
void pgTexTest();
void pgFontTest();
void pgShdTest();
void pgLitTest();
void pgLtsTest();
void pgNodeTest();
void pgPrimTest();
void pgSprtTest();
void pgMdlDataTest();
void pgMdlTest();
void pgMotDataTest();
void pgMotTest();
void pgRendTest();

void pgRend_ShaderTest();
void pgRend_LightTest();

/*
    SndTest
*/
void pgSndMgrTest();

/*
    DbgTest
*/
void pgDbgMgrTest();

/*
    UtilTest
*/
void pgUtilTest();


#endif // TEST_H_
