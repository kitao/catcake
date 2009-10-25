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


static void assertInitialSprtData(pgSprt& sprt, u16 index)
{
    pgAssert(isEqual(sprt.dataPos(index), pgVec::ZERO));
    pgAssert(sprt.dataCol(index) == pgCol::FULL);
    pgAssert(isEqual(sprt.dataW(index), 0.0f) && isEqual(sprt.dataH(index), 0.0f));
    pgAssert(isEqual(sprt.dataU1(index), 0.0f) && isEqual(sprt.dataV1(index), 0.0f));
    pgAssert(isEqual(sprt.dataU2(index), 1.0f) && isEqual(sprt.dataV2(index), 1.0f));
    pgAssert(sprt.dataAng(index) == 0);
}


static void setSprtData(pgSprt& sprt, u16 index, u32 key)
{
    sprt.dataPos(index) = pgVec(1.0f, 2.0f, 3.0f + key);
    sprt.dataCol(index) = pgCol(4, 5, 6, 7 + key);
    sprt.dataW(index) = 0.1f + key;
    sprt.dataH(index) = 0.2f + key;
    sprt.dataU1(index) = 0.3f + key;
    sprt.dataV1(index) = 0.4f + key;
    sprt.dataU2(index) = 0.5f + key;
    sprt.dataV2(index) = 0.6f + key;
    sprt.dataAng(index) = -123 + key;
}


static void assertSprtData(pgSprt& sprt, u16 index, u32 key)
{
    pgAssert(isEqual(sprt.dataPos(index), pgVec(1.0f, 2.0f, 3.0f + key)));
    pgAssert(sprt.dataCol(index) == pgCol(4, 5, 6, 7 + key));
    pgAssert(isEqual(sprt.dataW(index), 0.1f + key) && isEqual(sprt.dataH(index), 0.2f + key));
    pgAssert(isEqual(sprt.dataU1(index), 0.3f + key) && isEqual(sprt.dataV1(index), 0.4f + key));
    pgAssert(isEqual(sprt.dataU2(index), 0.5f + key) && isEqual(sprt.dataV2(index), 0.6f + key));
    pgAssert(sprt.dataAng(index) == -123 + static_cast<s32>(key));
}


static void assertThrowSprtData(pgSprt& sprt, u16 index)
{
    pgAssertThrow(sprt.dataPos(index), pgSprt::ExceptionInvalidArgument);
    pgAssertThrow(sprt.dataCol(index), pgSprt::ExceptionInvalidArgument);
    pgAssertThrow(sprt.dataW(index), pgSprt::ExceptionInvalidArgument);
    pgAssertThrow(sprt.dataH(index), pgSprt::ExceptionInvalidArgument);
    pgAssertThrow(sprt.dataU1(index), pgSprt::ExceptionInvalidArgument);
    pgAssertThrow(sprt.dataV1(index), pgSprt::ExceptionInvalidArgument);
    pgAssertThrow(sprt.dataU2(index), pgSprt::ExceptionInvalidArgument);
    pgAssertThrow(sprt.dataV2(index), pgSprt::ExceptionInvalidArgument);
    pgAssertThrow(sprt.dataAng(index), pgSprt::ExceptionInvalidArgument);
}


void pgSprtTest()
{
    /*
        sprtData()
    */
    {
        pgSprt::SprtData sprt_data;

        pgAssert(isEqual(sprt_data.pos, pgVec::ZERO));
        pgAssert(sprt_data.col == pgCol::FULL);
        pgAssert(isEqual(sprt_data.w, 0.0f) && isEqual(sprt_data.h, 0.0f));
        pgAssert(isEqual(sprt_data.u1, 0.0f) && isEqual(sprt_data.v1, 0.0f));
        pgAssert(isEqual(sprt_data.u2, 1.0f) && isEqual(sprt_data.v2, 1.0f));
        pgAssert(sprt_data.ang == 0);
    }

    /*
        DrawType getType() const

        pgSprt()
        ~pgSprt()
        void init(u16 max_data_num, pgID scr_id)
        u16 getCurDataNum() const
        void setCurDataNum(u16 cur_data_num)
        u16 getMaxDataNum() const
        void reallocData(u16 max_data_num)
        pgVec& dataPos(u16 index)
        pgCol& dataCol(u16 index)
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
        pgDrawMgr::createAfterRes();

        pgSprt sprt;

        pgAssertThrow(sprt.getType(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(sprt.getCurDataNum(), pgSprt::ExceptionNotInitialized);
        pgAssertThrow(sprt.setCurDataNum(0), pgSprt::ExceptionNotInitialized);
        pgAssertThrow(sprt.getMaxDataNum(), pgSprt::ExceptionNotInitialized);
        pgAssertThrow(sprt.reallocData(0), pgSprt::ExceptionNotInitialized);
        pgAssertThrow(sprt.dataPos(0), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt.dataCol(0), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt.dataW(0), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt.dataH(0), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt.dataU1(0), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt.dataV1(0), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt.dataU2(0), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt.dataV2(0), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt.dataAng(0), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt.setDataSize(0, 0.0f, 0.0f), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt.setDataUV(0, 0.0f, 0.0f, 0.0f, 0.0f), pgSprt::ExceptionInvalidArgument);

        sprt.init(1, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        sprt.init(4, pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        pgAssert(sprt.getType() == pgDraw::TYPE_SPRT);
        pgAssert(sprt.getCurDataNum() == 4 && sprt.getMaxDataNum() == 4);

        sprt.setCurDataNum(0);
        sprt.setCurDataNum(2);
        pgAssert(sprt.getCurDataNum() == 2);

        pgAssertThrow(sprt.setCurDataNum(5), pgSprt::ExceptionInvalidArgument);

        for (u32 i = 0; i < sprt.getMaxDataNum(); i++)
        {
            assertInitialSprtData(sprt, i);

            setSprtData(sprt, i, i);

            assertSprtData(sprt, i, i);
        }

        assertThrowSprtData(sprt, 5);

        pgDrawMgr::renderForSystem();

        sprt.reallocData(3);
        pgAssert(sprt.getCurDataNum() == 2 && sprt.getMaxDataNum() == 3);

        for (u32 i = 0; i < sprt.getMaxDataNum(); i++)
        {
            assertSprtData(sprt, i, i);
        }

        assertThrowSprtData(sprt, 4);

        pgDrawMgr::renderForSystem();

        sprt.reallocData(8);
        pgAssert(sprt.getCurDataNum() == 2 && sprt.getMaxDataNum() == 8);

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

        pgDrawMgr::renderForSystem();

        sprt.setDataSize(7, 123.0f, 456.0f);
        pgAssert(isEqual(sprt.dataW(7), 123.0f) && isEqual(sprt.dataH(7), 456.0f));

        sprt.setDataUV(7, 1.0f, 2.0f, 3.0f, 4.0f);
        pgAssert(isEqual(sprt.dataU1(7), 1.0f) && isEqual(sprt.dataV1(7), 2.0f) && isEqual(sprt.dataU2(7), 3.0f) && isEqual(sprt.dataV2(7), 4.0f));

        pgAssertThrow(sprt.setCurDataNum(9), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt.setDataSize(8, 123.0f, 456.0f), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt.setDataUV(8, 1.0f, 2.0f, 3.0f, 4.0f), pgSprt::ExceptionInvalidArgument);

        sprt.setCurDataNum(8);

        pgAssertThrow(sprt.init(0, pgDrawMgr::DEFAULT_3D_SCREEN_ID), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt.init(1, pgID::ZERO), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt.reallocData(0), pgSprt::ExceptionInvalidArgument);

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        void init(u16 max_data_num, pgDraw* parent)
        void copyData(u16 dest_index, const pgSprt* src_sprt, u16 src_index)
    */
    {
        pgDrawMgr::createAfterRes();

        pgSprt sprt1, sprt2;

        sprt1.init(3, pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        sprt2.init(1, &sprt1);
        sprt2.init(2, &sprt1);

        pgAssert(sprt2.getCurDataNum() == 2 && sprt2.getMaxDataNum() == 2);

        sprt2.setCurDataNum(0);
        sprt2.setCurDataNum(2);
        pgAssert(sprt2.getCurDataNum() == 2);

        pgAssertThrow(sprt2.setCurDataNum(3), pgSprt::ExceptionInvalidArgument);

        for (u32 i = 0; i < sprt2.getMaxDataNum(); i++)
        {
            assertInitialSprtData(sprt2, i);
        }

        assertThrowSprtData(sprt2, 2);

        pgDrawMgr::renderForSystem();

        setSprtData(sprt1, 2, 2);

        sprt2.copyData(1, &sprt1, 2);
        assertSprtData(sprt2, 1, 2);

        pgDrawMgr::renderForSystem();

        pgAssertThrow(sprt2.init(0, &sprt1), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt2.init(1, NULL), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt1.copyData(3, &sprt2, 0), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt1.copyData(0, NULL, 0), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt1.copyData(0, &sprt2, 2), pgSprt::ExceptionInvalidArgument);

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        void init(SprtData* sprt_data, u16 max_data_num, pgID scr_id)
        void init(SprtData* sprt_data, u16 max_data_num, pgDraw* parent)
        bool isShareData() const
    */
    {
        pgDrawMgr::createAfterRes();

        pgSprt sprt1, sprt2;
        pgSprt::SprtData sprt_data[3];

        pgAssertThrow(sprt1.isShareData(), pgPrim::ExceptionNotInitialized);

        sprt1.init(sprt_data, 3, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        sprt1.init(4, pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        pgAssert(!sprt1.isShareData());

        sprt2.init(2, &sprt1);
        sprt2.init(sprt_data, 3, &sprt1);

        pgAssert(sprt2.isShareData());

        sprt1.init(sprt_data, 3, pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        pgAssert(sprt1.isShareData());

        pgAssertThrow(sprt1.reallocData(3), pgSprt::ExceptionInvalidCall);
        pgAssertThrow(sprt2.reallocData(3), pgSprt::ExceptionInvalidCall);

        pgAssertThrow(sprt1.init(NULL, 1, pgDrawMgr::DEFAULT_3D_SCREEN_ID), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt1.init(sprt_data, 0, pgDrawMgr::DEFAULT_3D_SCREEN_ID), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt1.init(sprt_data, 1, pgID::ZERO), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt2.init(NULL, 1, &sprt1), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt2.init(sprt_data, 0, &sprt1), pgSprt::ExceptionInvalidArgument);
        pgAssertThrow(sprt2.init(sprt_data, 1, NULL), pgSprt::ExceptionInvalidArgument);

        pgDrawMgr::destroyBeforeRes();
    }
}
