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


static void assertInitialSprtData(ckSprt& sprt, u16 index)
{
    ckAssert(isEqual(sprt.dataPos(index), ckVec::ZERO));
    ckAssert(sprt.dataCol(index) == ckCol::FULL);
    ckAssert(isEqual(sprt.dataW(index), 0.0f) && isEqual(sprt.dataH(index), 0.0f));
    ckAssert(isEqual(sprt.dataU1(index), 0.0f) && isEqual(sprt.dataV1(index), 0.0f));
    ckAssert(isEqual(sprt.dataU2(index), 1.0f) && isEqual(sprt.dataV2(index), 1.0f));
    ckAssert(sprt.dataAng(index) == 0);
}


static void setSprtData(ckSprt& sprt, u16 index, u32 key)
{
    sprt.dataPos(index) = ckVec(1.0f, 2.0f, 3.0f + key);
    sprt.dataCol(index) = ckCol(4, 5, 6, 7 + key);
    sprt.dataW(index) = 0.1f + key;
    sprt.dataH(index) = 0.2f + key;
    sprt.dataU1(index) = 0.3f + key;
    sprt.dataV1(index) = 0.4f + key;
    sprt.dataU2(index) = 0.5f + key;
    sprt.dataV2(index) = 0.6f + key;
    sprt.dataAng(index) = -123 + key;
}


static void assertSprtData(ckSprt& sprt, u16 index, u32 key)
{
    ckAssert(isEqual(sprt.dataPos(index), ckVec(1.0f, 2.0f, 3.0f + key)));
    ckAssert(sprt.dataCol(index) == ckCol(4, 5, 6, 7 + key));
    ckAssert(isEqual(sprt.dataW(index), 0.1f + key) && isEqual(sprt.dataH(index), 0.2f + key));
    ckAssert(isEqual(sprt.dataU1(index), 0.3f + key) && isEqual(sprt.dataV1(index), 0.4f + key));
    ckAssert(isEqual(sprt.dataU2(index), 0.5f + key) && isEqual(sprt.dataV2(index), 0.6f + key));
    ckAssert(sprt.dataAng(index) == -123 + static_cast<s32>(key));
}


static void assertThrowSprtData(ckSprt& sprt, u16 index)
{
    ckAssertThrow(sprt.dataPos(index), ckSprt::ExceptionInvalidArgument);
    ckAssertThrow(sprt.dataCol(index), ckSprt::ExceptionInvalidArgument);
    ckAssertThrow(sprt.dataW(index), ckSprt::ExceptionInvalidArgument);
    ckAssertThrow(sprt.dataH(index), ckSprt::ExceptionInvalidArgument);
    ckAssertThrow(sprt.dataU1(index), ckSprt::ExceptionInvalidArgument);
    ckAssertThrow(sprt.dataV1(index), ckSprt::ExceptionInvalidArgument);
    ckAssertThrow(sprt.dataU2(index), ckSprt::ExceptionInvalidArgument);
    ckAssertThrow(sprt.dataV2(index), ckSprt::ExceptionInvalidArgument);
    ckAssertThrow(sprt.dataAng(index), ckSprt::ExceptionInvalidArgument);
}


void ckSprtTest()
{
    /*
        sprtData()
    */
    {
        ckSprt::SprtData sprt_data;

        ckAssert(isEqual(sprt_data.pos, ckVec::ZERO));
        ckAssert(sprt_data.col == ckCol::FULL);
        ckAssert(isEqual(sprt_data.w, 0.0f) && isEqual(sprt_data.h, 0.0f));
        ckAssert(isEqual(sprt_data.u1, 0.0f) && isEqual(sprt_data.v1, 0.0f));
        ckAssert(isEqual(sprt_data.u2, 1.0f) && isEqual(sprt_data.v2, 1.0f));
        ckAssert(sprt_data.ang == 0);
    }

    /*
        DrawType getType() const

        ckSprt()
        ~ckSprt()
        void init(u16 max_data_num, ckID scr_id)
        u16 getCurDataNum() const
        void setCurDataNum(u16 cur_data_num)
        u16 getMaxDataNum() const
        void reallocData(u16 max_data_num)
        ckVec& dataPos(u16 index)
        ckCol& dataCol(u16 index)
        r32& dataW(u16 index)
        r32& dataH(u16 index)
        r32& dataU1(u16 index)
        r32& dataV1(u16 index)
        r32& dataU2(u16 index)
        r32& dataV2(u16 index)
        s32& dataAng(u16 index)
        void setDataSize(u16 index, r32 width, r32 height)
        void setDataUV(u16 index, r32 u1, r32 v1, r32 u2, r32 v2)
    */
    {
        ckDrawMgr::createAfterRes();

        ckSprt sprt;

        ckAssertThrow(sprt.getType(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(sprt.getCurDataNum(), ckSprt::ExceptionNotInitialized);
        ckAssertThrow(sprt.setCurDataNum(0), ckSprt::ExceptionNotInitialized);
        ckAssertThrow(sprt.getMaxDataNum(), ckSprt::ExceptionNotInitialized);
        ckAssertThrow(sprt.reallocData(0), ckSprt::ExceptionNotInitialized);
        ckAssertThrow(sprt.dataPos(0), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt.dataCol(0), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt.dataW(0), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt.dataH(0), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt.dataU1(0), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt.dataV1(0), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt.dataU2(0), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt.dataV2(0), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt.dataAng(0), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt.setDataSize(0, 0.0f, 0.0f), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt.setDataUV(0, 0.0f, 0.0f, 0.0f, 0.0f), ckSprt::ExceptionInvalidArgument);

        sprt.init(1, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        sprt.init(4, ckDrawMgr::DEFAULT_3D_SCREEN_ID);

        ckAssert(sprt.getType() == ckDraw::TYPE_SPRT);
        ckAssert(sprt.getCurDataNum() == 4 && sprt.getMaxDataNum() == 4);

        sprt.setCurDataNum(0);
        sprt.setCurDataNum(2);
        ckAssert(sprt.getCurDataNum() == 2);

        ckAssertThrow(sprt.setCurDataNum(5), ckSprt::ExceptionInvalidArgument);

        for (u32 i = 0; i < sprt.getMaxDataNum(); i++)
        {
            assertInitialSprtData(sprt, i);

            setSprtData(sprt, i, i);

            assertSprtData(sprt, i, i);
        }

        assertThrowSprtData(sprt, 5);

        ckDrawMgr::renderForSystem();

        sprt.reallocData(3);
        ckAssert(sprt.getCurDataNum() == 2 && sprt.getMaxDataNum() == 3);

        for (u32 i = 0; i < sprt.getMaxDataNum(); i++)
        {
            assertSprtData(sprt, i, i);
        }

        assertThrowSprtData(sprt, 4);

        ckDrawMgr::renderForSystem();

        sprt.reallocData(8);
        ckAssert(sprt.getCurDataNum() == 2 && sprt.getMaxDataNum() == 8);

        for (u32 i = 0; i < sprt.getMaxDataNum(); i++)
        {
            if (i >= 3)
            {
                assertInitialSprtData(sprt, i);
            }
            else
            {
                assertSprtData(sprt, i, i);
            }
        }

        assertThrowSprtData(sprt, 8);

        ckDrawMgr::renderForSystem();

        sprt.setDataSize(7, 123.0f, 456.0f);
        ckAssert(isEqual(sprt.dataW(7), 123.0f) && isEqual(sprt.dataH(7), 456.0f));

        sprt.setDataUV(7, 1.0f, 2.0f, 3.0f, 4.0f);
        ckAssert(isEqual(sprt.dataU1(7), 1.0f) && isEqual(sprt.dataV1(7), 2.0f) && isEqual(sprt.dataU2(7), 3.0f) && isEqual(sprt.dataV2(7), 4.0f));

        ckAssertThrow(sprt.setCurDataNum(9), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt.setDataSize(8, 123.0f, 456.0f), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt.setDataUV(8, 1.0f, 2.0f, 3.0f, 4.0f), ckSprt::ExceptionInvalidArgument);

        sprt.setCurDataNum(8);

        ckAssertThrow(sprt.init(0, ckDrawMgr::DEFAULT_3D_SCREEN_ID), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt.init(1, ckID::ZERO), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt.reallocData(0), ckSprt::ExceptionInvalidArgument);

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        void init(u16 max_data_num, ckDraw* parent)
        void copyData(u16 dest_index, const ckSprt* src_sprt, u16 src_index)
    */
    {
        ckDrawMgr::createAfterRes();

        ckSprt sprt1, sprt2;

        sprt1.init(3, ckDrawMgr::DEFAULT_3D_SCREEN_ID);

        sprt2.init(1, &sprt1);
        sprt2.init(2, &sprt1);

        ckAssert(sprt2.getCurDataNum() == 2 && sprt2.getMaxDataNum() == 2);

        sprt2.setCurDataNum(0);
        sprt2.setCurDataNum(2);
        ckAssert(sprt2.getCurDataNum() == 2);

        ckAssertThrow(sprt2.setCurDataNum(3), ckSprt::ExceptionInvalidArgument);

        for (u32 i = 0; i < sprt2.getMaxDataNum(); i++)
        {
            assertInitialSprtData(sprt2, i);
        }

        assertThrowSprtData(sprt2, 2);

        ckDrawMgr::renderForSystem();

        setSprtData(sprt1, 2, 2);

        sprt2.copyData(1, &sprt1, 2);
        assertSprtData(sprt2, 1, 2);

        ckDrawMgr::renderForSystem();

        ckAssertThrow(sprt2.init(0, &sprt1), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt2.init(1, NULL), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt1.copyData(3, &sprt2, 0), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt1.copyData(0, NULL, 0), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt1.copyData(0, &sprt2, 2), ckSprt::ExceptionInvalidArgument);

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        void init(SprtData* sprt_data, u16 max_data_num, ckID scr_id)
        void init(SprtData* sprt_data, u16 max_data_num, ckDraw* parent)
        bool isShareData() const
    */
    {
        ckDrawMgr::createAfterRes();

        ckSprt sprt1, sprt2;
        ckSprt::SprtData sprt_data[3];

        ckAssertThrow(sprt1.isShareData(), ckPrim::ExceptionNotInitialized);

        sprt1.init(sprt_data, 3, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        sprt1.init(4, ckDrawMgr::DEFAULT_3D_SCREEN_ID);

        ckAssert(!sprt1.isShareData());

        sprt2.init(2, &sprt1);
        sprt2.init(sprt_data, 3, &sprt1);

        ckAssert(sprt2.isShareData());

        sprt1.init(sprt_data, 3, ckDrawMgr::DEFAULT_3D_SCREEN_ID);

        ckAssert(sprt1.isShareData());

        ckAssertThrow(sprt1.reallocData(3), ckSprt::ExceptionInvalidCall);
        ckAssertThrow(sprt2.reallocData(3), ckSprt::ExceptionInvalidCall);

        ckAssertThrow(sprt1.init(NULL, 1, ckDrawMgr::DEFAULT_3D_SCREEN_ID), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt1.init(sprt_data, 0, ckDrawMgr::DEFAULT_3D_SCREEN_ID), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt1.init(sprt_data, 1, ckID::ZERO), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt2.init(NULL, 1, &sprt1), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt2.init(sprt_data, 0, &sprt1), ckSprt::ExceptionInvalidArgument);
        ckAssertThrow(sprt2.init(sprt_data, 1, NULL), ckSprt::ExceptionInvalidArgument);

        ckDrawMgr::destroyBeforeRes();
    }
}
