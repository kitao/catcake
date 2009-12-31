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


static void assertInitialPrimData(ckPrim& prim, u16 index)
{
    ckAssert(isEqual(prim.dataPos(index), ckVec::ZERO));
    ckAssert(prim.dataCol(index) == ckCol::FULL);
    ckAssert(isEqual(prim.dataU(index), 0.0f));
    ckAssert(isEqual(prim.dataV(index), 0.0f));
}


static void setPrimData(ckPrim& prim, u16 index, u32 key)
{
    prim.dataPos(index) = ckVec(1.0f, 2.0f, 3.0f + key);
    prim.dataCol(index) = ckCol(4, 5, 6, 7 + key);
    prim.dataU(index) = 0.1f + key;
    prim.dataV(index) = 0.2f + key;
}


static void assertPrimData(ckPrim& prim, u16 index, u32 key)
{
    ckAssert(isEqual(prim.dataPos(index), ckVec(1.0f, 2.0f, 3.0f + key)));
    ckAssert(prim.dataCol(index) == ckCol(4, 5, 6, 7 + key));
    ckAssert(isEqual(prim.dataU(index), 0.1f + key));
    ckAssert(isEqual(prim.dataV(index), 0.2f + key));
}


static void assertThrowPrimData(ckPrim& prim, u16 index)
{
    ckAssertThrow(prim.dataPos(index), ckPrim::ExceptionInvalidArgument);
    ckAssertThrow(prim.dataCol(index), ckPrim::ExceptionInvalidArgument);
    ckAssertThrow(prim.dataU(index), ckPrim::ExceptionInvalidArgument);
    ckAssertThrow(prim.dataV(index), ckPrim::ExceptionInvalidArgument);
}


void ckPrimTest()
{
    /*
        PrimData()
    */
    {
        ckPrim::PrimData prim_data;

        ckAssert(isEqual(prim_data.pos, ckVec::ZERO));
        ckAssert(prim_data.col == ckCol::FULL);
        ckAssert(isEqual(prim_data.u, 0.0f) && isEqual(prim_data.v, 0.0f));
    }

    /*
        DrawType getType() const

        ckPrim()
        ~ckPrim()
        void init(PrimMode prim_mode, u16 max_data_num, ckID scr_id)
        PrimMode getPrimMode() const
        void setPrimMode(PrimMode prim_mode)
        u16 getCurDataNum() const
        void setCurDataNum(u16 cur_data_num)
        u16 getMaxDataNum() const
        void reallocData(u16 max_data_num)
        ckVec& dataPos(u16 index)
        ckCol& dataCol(u16 index)
        r32& dataU(u16 index)
        r32& dataV(u16 index)
        void setDataUV(u16 index, r32 u, r32 v)
        void setDataRect(u16 index, const ckVec& center, r32 width, r32 height, ckCol col, r32 u1, r32 v1, r32 u2, r32 v2);
    */
    {
        ckDrawMgr::createAfterRes();

        ckPrim prim;

        ckAssertThrow(prim.getType(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(prim.getPrimMode(), ckPrim::ExceptionNotInitialized);
        ckAssertThrow(prim.setPrimMode(ckPrim::MODE_POINTS), ckPrim::ExceptionNotInitialized);
        ckAssertThrow(prim.getCurDataNum(), ckPrim::ExceptionNotInitialized);
        ckAssertThrow(prim.setCurDataNum(0), ckPrim::ExceptionNotInitialized);
        ckAssertThrow(prim.setCurDataNum(0), ckPrim::ExceptionNotInitialized);
        ckAssertThrow(prim.getMaxDataNum(), ckPrim::ExceptionNotInitialized);
        ckAssertThrow(prim.reallocData(0), ckPrim::ExceptionNotInitialized);
        ckAssertThrow(prim.dataPos(0), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim.dataCol(0), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim.dataU(0), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim.dataV(0), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim.setDataUV(0, 0.0f, 0.0f), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim.setDataRect(0, ckVec::ZERO, 0.0f, 0.0f, ckCol::ZERO, 0.0f, 0.0f, 0.0f, 0.0f), ckPrim::ExceptionInvalidArgument);

        prim.init(ckPrim::MODE_LINES, 1, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        prim.init(ckPrim::MODE_TRIANGLES, 3, ckDrawMgr::DEFAULT_3D_SCREEN_ID);

        ckAssert(prim.getType() == ckDraw::TYPE_PRIM);
        ckAssert(prim.getPrimMode() == ckPrim::MODE_TRIANGLES);
        ckAssert(prim.getCurDataNum() == 3 && prim.getMaxDataNum() == 3);

        prim.setPrimMode(ckPrim::MODE_TRIANGLE_STRIP);
        ckAssert(prim.getPrimMode() == ckPrim::MODE_TRIANGLE_STRIP);

        prim.setCurDataNum(0);
        prim.setCurDataNum(3);
        ckAssert(prim.getCurDataNum() == 3);

        ckAssertThrow(prim.setCurDataNum(4), ckPrim::ExceptionInvalidArgument);

        for (u32 i = 0; i < prim.getMaxDataNum(); i++)
        {
            assertInitialPrimData(prim, i);

            setPrimData(prim, i, i);

            assertPrimData(prim, i, i);
        }

        assertThrowPrimData(prim, 3);

        ckDrawMgr::renderForSystem();

        prim.reallocData(2);
        ckAssert(prim.getCurDataNum() == 2 && prim.getMaxDataNum() == 2);

        for (u32 i = 0; i < prim.getMaxDataNum(); i++)
        {
            assertPrimData(prim, i, i);
        }

        assertThrowPrimData(prim, 2);

        ckDrawMgr::renderForSystem();

        prim.reallocData(5);
        ckAssert(prim.getCurDataNum() == 2 && prim.getMaxDataNum() == 5);

        for (u32 i = 0; i < prim.getMaxDataNum(); i++)
        {
            if (i >= 2)
            {
                assertInitialPrimData(prim, i);
            }
            else
            {
                assertPrimData(prim, i, i);
            }
        }

        assertThrowPrimData(prim, 5);

        ckDrawMgr::renderForSystem();

        prim.setDataUV(3, 123.0f, 456.0f);
        ckAssert(isEqual(prim.dataU(3), 123.0f) && isEqual(prim.dataV(3), 456.0f));

        prim.setDataRect(1, ckVec(100.0f, 200.0f, 300.0f), 10.0f, 20.0f, ckCol(1, 2, 3, 4), 0.1f, 0.2f, 0.3f, 0.4f);
        ckAssert(isEqual(prim.dataPos(1), ckVec(95.0f, 210.0f, 300.0f)));
        ckAssert(isEqual(prim.dataPos(2), ckVec(95.0f, 190.0f, 300.0f)));
        ckAssert(isEqual(prim.dataPos(3), ckVec(105.0f, 190.0f, 300.0f)));
        ckAssert(isEqual(prim.dataPos(4), ckVec(105.0f, 210.0f, 300.0f)));
        ckAssert(prim.dataCol(1) == ckCol(1, 2, 3, 4));
        ckAssert(prim.dataCol(2) == ckCol(1, 2, 3, 4));
        ckAssert(prim.dataCol(3) == ckCol(1, 2, 3, 4));
        ckAssert(prim.dataCol(4) == ckCol(1, 2, 3, 4));
        ckAssert(isEqual(prim.dataU(1), 0.1f) && isEqual(prim.dataV(1), 0.2f));
        ckAssert(isEqual(prim.dataU(2), 0.1f) && isEqual(prim.dataV(2), 0.4f));
        ckAssert(isEqual(prim.dataU(3), 0.3f) && isEqual(prim.dataV(3), 0.4f));
        ckAssert(isEqual(prim.dataU(4), 0.3f) && isEqual(prim.dataV(4), 0.2f));

        ckAssertThrow(prim.setCurDataNum(6), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim.setDataUV(5, 123.0f, 456.0f), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim.setDataRect(2, ckVec::ZERO, 1.0f, 2.0f, ckCol::ZERO, 3.0f, 4.0f, 5.0f, 6.0f), ckPrim::ExceptionInvalidArgument);

        prim.setCurDataNum(4);

        ckAssertThrow(prim.init(ckPrim::MODE_LINE_STRIP, 0, ckDrawMgr::DEFAULT_3D_SCREEN_ID), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim.init(ckPrim::MODE_LINE_STRIP, 1, ckID::ZERO), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim.reallocData(0), ckPrim::ExceptionInvalidArgument);

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        void init(PrimMode prim_mode, u16 max_data_num, ckDraw* parent)
        void copyData(u16 dest_index, const ckPrim* src_prim, u16 src_index)
    */
    {
        ckDrawMgr::createAfterRes();

        ckPrim prim1, prim2;

        prim1.init(ckPrim::MODE_TRIANGLES, 3, ckDrawMgr::DEFAULT_3D_SCREEN_ID);

        prim2.init(ckPrim::MODE_POINTS, 1, &prim1);
        prim2.init(ckPrim::MODE_TRIANGLE_FAN, 2, &prim1);

        ckAssert(prim2.getPrimMode() == ckPrim::MODE_TRIANGLE_FAN);
        ckAssert(prim2.getCurDataNum() == 2 && prim2.getMaxDataNum() == 2);

        prim2.setPrimMode(ckPrim::MODE_TRIANGLE_STRIP);
        ckAssert(prim2.getPrimMode() == ckPrim::MODE_TRIANGLE_STRIP);

        prim2.setCurDataNum(0);
        prim2.setCurDataNum(2);
        ckAssert(prim2.getCurDataNum() == 2);

        ckAssertThrow(prim2.setCurDataNum(3), ckPrim::ExceptionInvalidArgument);

        for (u32 i = 0; i < prim2.getMaxDataNum(); i++)
        {
            assertInitialPrimData(prim2, i);
        }

        assertThrowPrimData(prim2, 2);

        ckDrawMgr::renderForSystem();

        setPrimData(prim1, 2, 2);

        prim2.copyData(1, &prim1, 2);
        assertPrimData(prim2, 1, 2);

        ckDrawMgr::renderForSystem();

        ckAssertThrow(prim2.init(ckPrim::MODE_POINTS, 0, &prim1), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim2.init(ckPrim::MODE_LINE_LOOP, 1, NULL), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim1.copyData(3, &prim2, 0), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim1.copyData(0, NULL, 0), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim1.copyData(0, &prim2, 2), ckPrim::ExceptionInvalidArgument);

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        void init(PrimMode prim_mode, PrimData* prim_data, u16 max_data_num, ckID scr_id)
        void init(PrimMode prim_mode, PrimData* prim_data, u16 max_data_num, ckDraw* parent)
        bool isShareData() const
    */
    {
        ckDrawMgr::createAfterRes();

        ckPrim prim1, prim2;
        ckPrim::PrimData prim_data[3];

        ckAssertThrow(prim1.isShareData(), ckPrim::ExceptionNotInitialized);

        prim1.init(ckPrim::MODE_POINTS, prim_data, 3, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        prim1.init(ckPrim::MODE_POINTS, 4, ckDrawMgr::DEFAULT_3D_SCREEN_ID);

        ckAssert(!prim1.isShareData());

        prim2.init(ckPrim::MODE_LINE_STRIP, 2, &prim1);
        prim2.init(ckPrim::MODE_LINE_STRIP, prim_data, 3, &prim1);

        ckAssert(prim2.isShareData());

        prim1.init(ckPrim::MODE_POINTS, prim_data, 3, ckDrawMgr::DEFAULT_3D_SCREEN_ID);

        ckAssert(prim1.isShareData());

        ckAssertThrow(prim1.reallocData(3), ckPrim::ExceptionInvalidCall);
        ckAssertThrow(prim2.reallocData(3), ckPrim::ExceptionInvalidCall);

        ckAssertThrow(prim1.init(ckPrim::MODE_LINE_STRIP, NULL, 1, ckDrawMgr::DEFAULT_3D_SCREEN_ID), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim1.init(ckPrim::MODE_LINE_STRIP, prim_data, 0, ckDrawMgr::DEFAULT_3D_SCREEN_ID), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim1.init(ckPrim::MODE_LINE_STRIP, prim_data, 1, ckID::ZERO), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim2.init(ckPrim::MODE_LINE_STRIP, NULL, 1, &prim1), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim2.init(ckPrim::MODE_LINE_STRIP, prim_data, 0, &prim1), ckPrim::ExceptionInvalidArgument);
        ckAssertThrow(prim2.init(ckPrim::MODE_LINE_STRIP, prim_data, 1, NULL), ckPrim::ExceptionInvalidArgument);

        ckDrawMgr::destroyBeforeRes();
    }
}
