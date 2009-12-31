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
