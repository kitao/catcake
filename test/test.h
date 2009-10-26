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


#include "catcake.h"


#define TEST_DATA_DIR "../../../../test/test_data/"


#define ckAssert(expression) \
    do \
    { \
        if (!(expression)) \
        { \
            ckError("Assertion Failed: %s(%d)", __FILE__, __LINE__); \
        } \
    } \
    while (false)


#ifdef CK_NO_THROW_EXCEPTION
#define ckAssertThrow(...)
#else
#define ckAssertThrow(expression, ...) \
    do \
    { \
        ckTry \
        { \
            expression; \
            ckAssert(false); \
        } \
        ckCatch(__VA_ARGS__ e) \
        { \
            ckSysMgr::printf("Caught Exception: %s in %s(%d)\n", e.getException(), e.getFile(), e.getLine()); \
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
bool isEqual(const ckVec& vec1, const ckVec& vec2);
bool isEqual(const ckMat& mat1, const ckMat& mat2);

/*
    MemTest
*/
void ckMemMgrTest();
void ckMemUtilTest();

/*
    MathTest
*/
void ckMathTest();
void ckVecTest();
void ckMatTest();
void ckQuatTest();
void ckFixTest();

/*
    GenTest
*/
void ckIDTest();
void ckTypeTest();
void ckFlagTest();
void ckListTest();
void ckTreeTest();
void ckBufTest();
void ckMapTest();
void ckStrTest();
void ckMsgTest();

/*
    CdtTest
*/
void ckCdtAABBTest();
void ckCdtSphTest();
void ckCdtBoxTest();
void ckCdtTriTest();
void ckCdtRayTest();
void ckCdtTest();

/*
    SysTest
*/
void ckSysMgrTest();
void ckSysUtilTest();

/*
    TaskTest
*/
void ckTaskMgrTest();
void ckTaskUtilTest();
void ckTaskTest();

/*
    KeyTest
*/
void ckKeyMgrTest();

/*
    ResTest
*/
void ckResMgrTest();
void ckResTest();

/*
    ConfTest
*/
void ckConfMgrTest();
void ckConfTest1();
void ckConfTest2();
void ckEntTest();

/*
    DrawTest
*/
void ckColTest();
void ckDrawMgrTest();
void ckScrTest();
void ckTexTest();
void ckFontTest();
void ckShdTest();
void ckLitTest();
void ckLtsTest();
void ckNodeTest();
void ckPrimTest();
void ckSprtTest();
void ckMdlDataTest();
void ckMdlTest();
void ckMotDataTest();
void ckMotTest();
void ckRendTest();

void ckRend_ShaderTest();
void ckRend_LightTest();

/*
    SndTest
*/
void ckSndMgrTest();

/*
    DbgTest
*/
void ckDbgMgrTest();

/*
    UtilTest
*/
void ckUtilTest();


#endif // TEST_H_
