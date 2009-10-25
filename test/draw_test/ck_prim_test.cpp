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


static void assertInitialPrimData(pgPrim& prim, u16 index)
{
    pgAssert(isEqual(prim.dataPos(index), pgVec::ZERO));
    pgAssert(prim.dataCol(index) == pgCol::FULL);
    pgAssert(isEqual(prim.dataU(index), 0.0f));
    pgAssert(isEqual(prim.dataV(index), 0.0f));
}


static void setPrimData(pgPrim& prim, u16 index, u32 key)
{
    prim.dataPos(index) = pgVec(1.0f, 2.0f, 3.0f + key);
    prim.dataCol(index) = pgCol(4, 5, 6, 7 + key);
    prim.dataU(index) = 0.1f + key;
    prim.dataV(index) = 0.2f + key;
}


static void assertPrimData(pgPrim& prim, u16 index, u32 key)
{
    pgAssert(isEqual(prim.dataPos(index), pgVec(1.0f, 2.0f, 3.0f + key)));
    pgAssert(prim.dataCol(index) == pgCol(4, 5, 6, 7 + key));
    pgAssert(isEqual(prim.dataU(index), 0.1f + key));
    pgAssert(isEqual(prim.dataV(index), 0.2f + key));
}


static void assertThrowPrimData(pgPrim& prim, u16 index)
{
    pgAssertThrow(prim.dataPos(index), pgPrim::ExceptionInvalidArgument);
    pgAssertThrow(prim.dataCol(index), pgPrim::ExceptionInvalidArgument);
    pgAssertThrow(prim.dataU(index), pgPrim::ExceptionInvalidArgument);
    pgAssertThrow(prim.dataV(index), pgPrim::ExceptionInvalidArgument);
}


void pgPrimTest()
{
    /*
        PrimData()
    */
    {
        pgPrim::PrimData prim_data;

        pgAssert(isEqual(prim_data.pos, pgVec::ZERO));
        pgAssert(prim_data.col == pgCol::FULL);
        pgAssert(isEqual(prim_data.u, 0.0f) && isEqual(prim_data.v, 0.0f));
    }

    /*
        DrawType getType() const

        pgPrim()
        ~pgPrim()
        void init(PrimMode prim_mode, u16 max_data_num, pgID scr_id)
        PrimMode getPrimMode() const
        void setPrimMode(PrimMode prim_mode)
        u16 getCurDataNum() const
        void setCurDataNum(u16 cur_data_num)
        u16 getMaxDataNum() const
        void reallocData(u16 max_data_num)
        pgVec& dataPos(u16 index)
        pgCol& dataCol(u16 index)
        r32& dataU(u16 index)
        r32& dataV(u16 index)
        void setDataUV(u16 index, r32 u, r32 v)
        void setDataRect(u16 index, const pgVec& center, r32 width, r32 height, pgCol col, r32 u1, r32 v1, r32 u2, r32 v2);
    */
    {
        pgDrawMgr::createAfterRes();

        pgPrim prim;

        pgAssertThrow(prim.getType(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(prim.getPrimMode(), pgPrim::ExceptionNotInitialized);
        pgAssertThrow(prim.setPrimMode(pgPrim::MODE_POINTS), pgPrim::ExceptionNotInitialized);
        pgAssertThrow(prim.getCurDataNum(), pgPrim::ExceptionNotInitialized);
        pgAssertThrow(prim.setCurDataNum(0), pgPrim::ExceptionNotInitialized);
        pgAssertThrow(prim.setCurDataNum(0), pgPrim::ExceptionNotInitialized);
        pgAssertThrow(prim.getMaxDataNum(), pgPrim::ExceptionNotInitialized);
        pgAssertThrow(prim.reallocData(0), pgPrim::ExceptionNotInitialized);
        pgAssertThrow(prim.dataPos(0), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim.dataCol(0), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim.dataU(0), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim.dataV(0), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim.setDataUV(0, 0.0f, 0.0f), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim.setDataRect(0, pgVec::ZERO, 0.0f, 0.0f, pgCol::ZERO, 0.0f, 0.0f, 0.0f, 0.0f), pgPrim::ExceptionInvalidArgument);

        prim.init(pgPrim::MODE_LINES, 1, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        prim.init(pgPrim::MODE_TRIANGLES, 3, pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        pgAssert(prim.getType() == pgDraw::TYPE_PRIM);
        pgAssert(prim.getPrimMode() == pgPrim::MODE_TRIANGLES);
        pgAssert(prim.getCurDataNum() == 3 && prim.getMaxDataNum() == 3);

        prim.setPrimMode(pgPrim::MODE_TRIANGLE_STRIP);
        pgAssert(prim.getPrimMode() == pgPrim::MODE_TRIANGLE_STRIP);

        prim.setCurDataNum(0);
        prim.setCurDataNum(3);
        pgAssert(prim.getCurDataNum() == 3);

        pgAssertThrow(prim.setCurDataNum(4), pgPrim::ExceptionInvalidArgument);

        for (u32 i = 0; i < prim.getMaxDataNum(); i++)
        {
            assertInitialPrimData(prim, i);

            setPrimData(prim, i, i);

            assertPrimData(prim, i, i);
        }

        assertThrowPrimData(prim, 3);

        pgDrawMgr::renderForSystem();

        prim.reallocData(2);
        pgAssert(prim.getCurDataNum() == 2 && prim.getMaxDataNum() == 2);

        for (u32 i = 0; i < prim.getMaxDataNum(); i++)
        {
            assertPrimData(prim, i, i);
        }

        assertThrowPrimData(prim, 2);

        pgDrawMgr::renderForSystem();

        prim.reallocData(5);
        pgAssert(prim.getCurDataNum() == 2 && prim.getMaxDataNum() == 5);

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

        pgDrawMgr::renderForSystem();

        prim.setDataUV(3, 123.0f, 456.0f);
        pgAssert(isEqual(prim.dataU(3), 123.0f) && isEqual(prim.dataV(3), 456.0f));

        prim.setDataRect(1, pgVec(100.0f, 200.0f, 300.0f), 10.0f, 20.0f, pgCol(1, 2, 3, 4), 0.1f, 0.2f, 0.3f, 0.4f);
        pgAssert(isEqual(prim.dataPos(1), pgVec(95.0f, 210.0f, 300.0f)));
        pgAssert(isEqual(prim.dataPos(2), pgVec(95.0f, 190.0f, 300.0f)));
        pgAssert(isEqual(prim.dataPos(3), pgVec(105.0f, 190.0f, 300.0f)));
        pgAssert(isEqual(prim.dataPos(4), pgVec(105.0f, 210.0f, 300.0f)));
        pgAssert(prim.dataCol(1) == pgCol(1, 2, 3, 4));
        pgAssert(prim.dataCol(2) == pgCol(1, 2, 3, 4));
        pgAssert(prim.dataCol(3) == pgCol(1, 2, 3, 4));
        pgAssert(prim.dataCol(4) == pgCol(1, 2, 3, 4));
        pgAssert(isEqual(prim.dataU(1), 0.1f) && isEqual(prim.dataV(1), 0.2f));
        pgAssert(isEqual(prim.dataU(2), 0.1f) && isEqual(prim.dataV(2), 0.4f));
        pgAssert(isEqual(prim.dataU(3), 0.3f) && isEqual(prim.dataV(3), 0.4f));
        pgAssert(isEqual(prim.dataU(4), 0.3f) && isEqual(prim.dataV(4), 0.2f));

        pgAssertThrow(prim.setCurDataNum(6), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim.setDataUV(5, 123.0f, 456.0f), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim.setDataRect(2, pgVec::ZERO, 1.0f, 2.0f, pgCol::ZERO, 3.0f, 4.0f, 5.0f, 6.0f), pgPrim::ExceptionInvalidArgument);

        prim.setCurDataNum(4);

        pgAssertThrow(prim.init(pgPrim::MODE_LINE_STRIP, 0, pgDrawMgr::DEFAULT_3D_SCREEN_ID), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim.init(pgPrim::MODE_LINE_STRIP, 1, pgID::ZERO), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim.reallocData(0), pgPrim::ExceptionInvalidArgument);

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        void init(PrimMode prim_mode, u16 max_data_num, pgDraw* parent)
        void copyData(u16 dest_index, const pgPrim* src_prim, u16 src_index)
    */
    {
        pgDrawMgr::createAfterRes();

        pgPrim prim1, prim2;

        prim1.init(pgPrim::MODE_TRIANGLES, 3, pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        prim2.init(pgPrim::MODE_POINTS, 1, &prim1);
        prim2.init(pgPrim::MODE_TRIANGLE_FAN, 2, &prim1);

        pgAssert(prim2.getPrimMode() == pgPrim::MODE_TRIANGLE_FAN);
        pgAssert(prim2.getCurDataNum() == 2 && prim2.getMaxDataNum() == 2);

        prim2.setPrimMode(pgPrim::MODE_TRIANGLE_STRIP);
        pgAssert(prim2.getPrimMode() == pgPrim::MODE_TRIANGLE_STRIP);

        prim2.setCurDataNum(0);
        prim2.setCurDataNum(2);
        pgAssert(prim2.getCurDataNum() == 2);

        pgAssertThrow(prim2.setCurDataNum(3), pgPrim::ExceptionInvalidArgument);

        for (u32 i = 0; i < prim2.getMaxDataNum(); i++)
        {
            assertInitialPrimData(prim2, i);
        }

        assertThrowPrimData(prim2, 2);

        pgDrawMgr::renderForSystem();

        setPrimData(prim1, 2, 2);

        prim2.copyData(1, &prim1, 2);
        assertPrimData(prim2, 1, 2);

        pgDrawMgr::renderForSystem();

        pgAssertThrow(prim2.init(pgPrim::MODE_POINTS, 0, &prim1), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim2.init(pgPrim::MODE_LINE_LOOP, 1, NULL), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim1.copyData(3, &prim2, 0), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim1.copyData(0, NULL, 0), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim1.copyData(0, &prim2, 2), pgPrim::ExceptionInvalidArgument);

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        void init(PrimMode prim_mode, PrimData* prim_data, u16 max_data_num, pgID scr_id)
        void init(PrimMode prim_mode, PrimData* prim_data, u16 max_data_num, pgDraw* parent)
        bool isShareData() const
    */
    {
        pgDrawMgr::createAfterRes();

        pgPrim prim1, prim2;
        pgPrim::PrimData prim_data[3];

        pgAssertThrow(prim1.isShareData(), pgPrim::ExceptionNotInitialized);

        prim1.init(pgPrim::MODE_POINTS, prim_data, 3, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        prim1.init(pgPrim::MODE_POINTS, 4, pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        pgAssert(!prim1.isShareData());

        prim2.init(pgPrim::MODE_LINE_STRIP, 2, &prim1);
        prim2.init(pgPrim::MODE_LINE_STRIP, prim_data, 3, &prim1);

        pgAssert(prim2.isShareData());

        prim1.init(pgPrim::MODE_POINTS, prim_data, 3, pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        pgAssert(prim1.isShareData());

        pgAssertThrow(prim1.reallocData(3), pgPrim::ExceptionInvalidCall);
        pgAssertThrow(prim2.reallocData(3), pgPrim::ExceptionInvalidCall);

        pgAssertThrow(prim1.init(pgPrim::MODE_LINE_STRIP, NULL, 1, pgDrawMgr::DEFAULT_3D_SCREEN_ID), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim1.init(pgPrim::MODE_LINE_STRIP, prim_data, 0, pgDrawMgr::DEFAULT_3D_SCREEN_ID), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim1.init(pgPrim::MODE_LINE_STRIP, prim_data, 1, pgID::ZERO), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim2.init(pgPrim::MODE_LINE_STRIP, NULL, 1, &prim1), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim2.init(pgPrim::MODE_LINE_STRIP, prim_data, 0, &prim1), pgPrim::ExceptionInvalidArgument);
        pgAssertThrow(prim2.init(pgPrim::MODE_LINE_STRIP, prim_data, 1, NULL), pgPrim::ExceptionInvalidArgument);

        pgDrawMgr::destroyBeforeRes();
    }
}
