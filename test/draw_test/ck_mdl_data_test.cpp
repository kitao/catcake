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


void pgMdlDataTest()
{
    /*
        static const u32 MODEL_DATA_VERSION
    */
    {
        pgAssert(pgMdlData::MODEL_DATA_VERSION == 50);
    }

    /*
        pgMdlData()
        ~pgMdlData()
        void initAsReader(const void* data, u32 data_size)
        void initAsWriter(u16 node_num, u16 vert_num, pgID tex_id, bool has_normal)
        ModelDataMode getMode() const
        u16 getNodeNum() const
        u16 getVertNum() const
        bool hasNormal() const
        pgID getTextureID() const
        u16 getNodeParentIndex(u16 node_index) const
        void setNodeParentIndex(u16 node_index, u16 parent_index)
        void setNodeParentIndex_noParent(u16 node_index)
        void setNodeNoParent(u16 node_index)
        const pgMat& getNodeLocal(u16 node_index) const
        void setNodeLocal(u16 node_index, const pgMat& local)
        pgPrim::PrimMode getNodePrimMode(u16 node_index) const
        void setNodePrimMode(u16 node_index, pgPrim::PrimMode prim_mode)
        pgPrim::BlendMode getNodeBlendMode(u16 node_index) const
        void setNodeBlendMode(u16 node_index, pgPrim::BlendMode blend_mode)
        u16 getNodeVertIndex(u16 node_index) const
        u16 getNodeVertNum(u16 node_index) const
        void setNodeVertInfo(u16 node_index, u16 vert_index, u16 vert_num)
        const pgPrim::PrimData* getNodePrimData(u16 node_index) const
        const pgVec* getNodeNormalData(u16 node_index) const
        const pgVec& getNodeClipBoundMinForReader(u16 node_index) const
        const pgVec& getNodeClipBoundMaxForReader(u16 node_index) const
        const pgVec& getVertPos(u16 vert_index) const
        void setVertPos(u16 vert_index, const pgVec& pos)
        pgCol getVertCol(u16 vert_index) const
        void setVertCol(u16 vert_index, pgCol col)
        r32 getVertU(u16 vert_index) const
        r32 getVertV(u16 vert_index) const
        void setVertUV(u16 vert_index, r32 u, r32 v)
        const pgVec& getVertN(u16 vert_index) const
        void setVertN(u16 vert_index, const pgVec& n)
        void registerAsResource(pgID res_id)
    */
    {
        pgMdlData mdl_data;

        pgAssertThrow(mdl_data.getMode(), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getNodeNum(), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getVertNum(), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.hasNormal(), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getTextureID(), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getNodeParentIndex(0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.setNodeParentIndex(0, 0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.setNodeParentIndex_noParent(0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getNodeLocal(0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.setNodeLocal(0, pgMat::UNIT), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getNodePrimMode(0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.setNodePrimMode(0, pgPrim::MODE_POINTS), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getNodeBlendMode(0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.setNodeBlendMode(0, pgPrim::BLEND_HALF), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getNodeVertIndex(0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getNodeVertNum(0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.setNodeVertInfo(0, 0, 0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getNodePrimData(0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getNodeNormalData(0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getNodeClipBoundMinForReader(0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getNodeClipBoundMaxForReader(0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getVertPos(0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.setVertPos(0, pgVec::ZERO), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getVertCol(0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.setVertCol(0, pgCol::FULL), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getVertU(0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getVertV(0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.setVertUV(0, 0.0f, 0.0f), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.getVertN(0), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.setVertN(0, pgVec::ZERO), pgMdlData::ExceptionNotInitialized);
        pgAssertThrow(mdl_data.registerAsResource(pgID::ZERO), pgMdlData::ExceptionNotInitialized);

        mdl_data.initAsWriter(2, 3, pgID_("test"), true);

        pgAssert(mdl_data.getMode() == pgMdlData::MODE_WRITE);
        pgAssert(mdl_data.getNodeNum() == 2);
        pgAssert(mdl_data.getVertNum() == 3);
        pgAssert(mdl_data.hasNormal());
        pgAssert(mdl_data.getTextureID() == pgID_("test"));

        for (s32 i = 0; i < 2; i++)
        {
            pgAssert(mdl_data.getNodeParentIndex(i) == 0);
            pgAssert(isEqual(mdl_data.getNodeLocal(i), pgMat::UNIT));
            pgAssert(mdl_data.getNodePrimMode(i) == pgPrim::MODE_POINTS);
            pgAssert(mdl_data.getNodeBlendMode(i) == pgPrim::BLEND_OFF);
            pgAssert(mdl_data.getNodeVertIndex(i) == 0);
            pgAssert(mdl_data.getNodeVertNum(i) == 0);
            pgAssert(mdl_data.getNodePrimData(i));
            pgAssert(mdl_data.getNodeNormalData(i));
        }

        mdl_data.setNodeParentIndex_noParent(0);
        mdl_data.setNodeParentIndex(1, 0);
        pgAssert(mdl_data.getNodeParentIndex(0) == 0 && mdl_data.getNodeParentIndex(1) == 0);

        mdl_data.setNodeLocal(0, pgMat::UNIT);
        mdl_data.setNodeLocal(1, pgMat::UNIT.translate(1.0f, 2.0f, 3.0f));
        pgAssert(isEqual(mdl_data.getNodeLocal(0), pgMat::UNIT) && isEqual(mdl_data.getNodeLocal(1), pgMat::UNIT.translate(1.0f, 2.0f, 3.0f)));

        mdl_data.setNodePrimMode(0, pgPrim::MODE_POINTS);
        mdl_data.setNodePrimMode(1, pgPrim::MODE_LINE_STRIP);
        pgAssert(mdl_data.getNodePrimMode(0) == pgPrim::MODE_POINTS && mdl_data.getNodePrimMode(1) == pgPrim::MODE_LINE_STRIP);

        mdl_data.setNodeBlendMode(0, pgPrim::BLEND_HALF);
        mdl_data.setNodeBlendMode(1, pgPrim::BLEND_ADD);
        pgAssert(mdl_data.getNodeBlendMode(0) == pgPrim::BLEND_HALF && mdl_data.getNodeBlendMode(1) == pgPrim::BLEND_ADD);

        mdl_data.setNodeVertInfo(0, 0, 3);
        mdl_data.setNodeVertInfo(1, 2, 1);
        pgAssert(mdl_data.getNodeVertIndex(0) == 0 && mdl_data.getNodeVertIndex(1) == 2);
        pgAssert(mdl_data.getNodeVertNum(0) == 3 && mdl_data.getNodeVertNum(1) == 1);

        pgAssert(mdl_data.getNodePrimData(0) && mdl_data.getNodePrimData(1));
        pgAssert(mdl_data.getNodeNormalData(0) && mdl_data.getNodeNormalData(1));

        for (s32 i = 0; i < 3; i++)
        {
            pgAssert(isEqual(mdl_data.getVertPos(i), pgVec::ZERO));
            pgAssert(mdl_data.getVertCol(i) == pgCol::FULL);
            pgAssert(isEqual(mdl_data.getVertU(i), 0.0f) && isEqual(mdl_data.getVertV(i), 0.0f));
            pgAssert(isEqual(mdl_data.getVertN(i), pgVec::Z_UNIT));
        }

        mdl_data.setVertPos(0, pgVec(10.0f, -2.0f, -3.0f));
        mdl_data.setVertPos(1, pgVec(-5.0f, 0.0f, 5.0f));

        mdl_data.setVertPos(2, pgVec(1.0f, 2.0f, 3.0f));
        pgAssert(isEqual(mdl_data.getVertPos(2), pgVec(1.0f, 2.0f, 3.0f)));

        mdl_data.setVertCol(2, pgCol(10, 20, 30, 40));
        pgAssert(mdl_data.getVertCol(2) == pgCol(10, 20, 30, 40));

        mdl_data.setVertUV(2, 0.11f, 0.22f);
        pgAssert(isEqual(mdl_data.getVertU(2), 0.11f) && isEqual(mdl_data.getVertV(2), 0.22f));

        mdl_data.setVertN(2, pgVec::X_UNIT);
        pgAssert(isEqual(mdl_data.getVertN(2), pgVec::X_UNIT));

        pgAssertThrow(mdl_data.getNodeClipBoundMinForReader(0), pgMdlData::ExceptionInvalidCall);
        pgAssertThrow(mdl_data.getNodeClipBoundMaxForReader(0), pgMdlData::ExceptionInvalidCall);
        pgAssertThrow(mdl_data.initAsWriter(0, 1, pgID::ZERO, false), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.initAsWriter(1, 0, pgID::ZERO, false), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.getNodeParentIndex(2), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.setNodeParentIndex(2, 0), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.setNodeParentIndex(0, 0), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.setNodeParentIndex_noParent(2), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.getNodeLocal(2), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.setNodeLocal(2, pgMat::UNIT), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.getNodePrimMode(2), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.setNodePrimMode(2, pgPrim::MODE_POINTS), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.getNodeBlendMode(2), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.setNodeBlendMode(2, pgPrim::BLEND_OFF), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.getNodeVertIndex(2), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.getNodeVertNum(2), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.setNodeVertInfo(2, 0, 0), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.setNodeVertInfo(0, 3, 0), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.setNodeVertInfo(0, 0, 4), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.getNodePrimData(2), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.getNodeNormalData(2), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.getVertPos(3), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.setVertPos(3, pgVec::ZERO), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.getVertCol(3), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.setVertCol(3, pgCol::FULL), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.getVertU(3), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.getVertV(3), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.setVertUV(3, 0.0f, 0.0f), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.getVertN(3), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.setVertN(3, pgVec::ZERO), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.registerAsResource(pgID::ZERO), pgMdlData::ExceptionInvalidArgument);

        mdl_data.registerAsResource(pgID_("test_mdl_data"));

        pgAssert(pgResMgr::hasResource(pgID_("test_mdl_data")));
        pgAssert(mdl_data.getMode() == pgMdlData::MODE_READ);

        mdl_data.initAsWriter(1, 2, pgID::ZERO, false);

        pgAssert(!mdl_data.hasNormal());

        pgAssertThrow(mdl_data.getNodeNormalData(0), pgMdlData::ExceptionInvalidCall);
        pgAssertThrow(mdl_data.getVertN(0), pgMdlData::ExceptionInvalidCall);
        pgAssertThrow(mdl_data.setVertN(0, pgVec::ZERO), pgMdlData::ExceptionInvalidCall);

        pgRes res = pgResMgr::getResource(pgID_("test_mdl_data"));

        mdl_data.initAsReader(res.getData<void>(), res.getDataSize());

        pgAssert(mdl_data.getMode() == pgMdlData::MODE_READ);
        pgAssert(mdl_data.getNodeNum() == 2);
        pgAssert(mdl_data.getVertNum() == 3);
        pgAssert(mdl_data.hasNormal());
        pgAssert(mdl_data.getTextureID() == pgID_("test"));
        pgAssert(mdl_data.getNodeParentIndex(0) == 0 && mdl_data.getNodeParentIndex(1) == 0);
        pgAssert(isEqual(mdl_data.getNodeLocal(0), pgMat::UNIT) && isEqual(mdl_data.getNodeLocal(1), pgMat::UNIT.translate(1.0f, 2.0f, 3.0f)));
        pgAssert(mdl_data.getNodePrimMode(0) == pgPrim::MODE_POINTS && mdl_data.getNodePrimMode(1) == pgPrim::MODE_LINE_STRIP);
        pgAssert(mdl_data.getNodeBlendMode(0) == pgPrim::BLEND_HALF && mdl_data.getNodeBlendMode(1) == pgPrim::BLEND_ADD);
        pgAssert(mdl_data.getNodeVertIndex(0) == 0 && mdl_data.getNodeVertIndex(1) == 2);
        pgAssert(mdl_data.getNodeVertNum(0) == 3 && mdl_data.getNodeVertNum(1) == 1);
        pgAssert(isEqual(mdl_data.getNodeClipBoundMinForReader(0), pgVec(-5.0f, -2.0f, -3.0f)));
        pgAssert(isEqual(mdl_data.getNodeClipBoundMinForReader(1), pgVec(1.0f, 2.0f, 3.0f)));
        pgAssert(isEqual(mdl_data.getNodeClipBoundMaxForReader(0), pgVec(10.0f, 2.0f, 5.0f)));
        pgAssert(isEqual(mdl_data.getNodeClipBoundMaxForReader(1), pgVec(1.0f, 2.0f, 3.0f)));
        pgAssert(mdl_data.getNodePrimData(0) && mdl_data.getNodePrimData(1));
        pgAssert(mdl_data.getNodeNormalData(0) && mdl_data.getNodeNormalData(1));
        pgAssert(isEqual(mdl_data.getVertPos(2), pgVec(1.0f, 2.0f, 3.0f)));
        pgAssert(mdl_data.getVertCol(2) == pgCol(10, 20, 30, 40));
        pgAssert(isEqual(mdl_data.getVertU(2), 0.11f) && isEqual(mdl_data.getVertV(2), 0.22f));
        pgAssert(isEqual(mdl_data.getVertN(2), pgVec::X_UNIT));

        pgAssertThrow(mdl_data.initAsReader(NULL, 100), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.initAsReader("dummy", 0), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.setNodeParentIndex(0, 1), pgMdlData::ExceptionInvalidCall);
        pgAssertThrow(mdl_data.setNodeParentIndex_noParent(0), pgMdlData::ExceptionInvalidCall);
        pgAssertThrow(mdl_data.setNodeLocal(0, pgMat::UNIT), pgMdlData::ExceptionInvalidCall);
        pgAssertThrow(mdl_data.setNodePrimMode(0, pgPrim::MODE_POINTS), pgMdlData::ExceptionInvalidCall);
        pgAssertThrow(mdl_data.setNodeBlendMode(0, pgPrim::BLEND_OFF), pgMdlData::ExceptionInvalidCall);
        pgAssertThrow(mdl_data.setNodeVertInfo(0, 0, 0), pgMdlData::ExceptionInvalidCall);
        pgAssertThrow(mdl_data.getNodeClipBoundMinForReader(2), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.getNodeClipBoundMaxForReader(2), pgMdlData::ExceptionInvalidArgument);
        pgAssertThrow(mdl_data.setVertPos(0, pgVec::ZERO), pgMdlData::ExceptionInvalidCall);
        pgAssertThrow(mdl_data.setVertCol(0, pgCol::FULL), pgMdlData::ExceptionInvalidCall);
        pgAssertThrow(mdl_data.setVertUV(0, 0.0f, 0.0f), pgMdlData::ExceptionInvalidCall);
        pgAssertThrow(mdl_data.setVertN(0, pgVec::ZERO), pgMdlData::ExceptionInvalidCall);
        pgAssertThrow(mdl_data.registerAsResource(pgID::genID()), pgMdlData::ExceptionInvalidCall);

        pgResMgr::removeResource(pgID_("test_mdl_data"));
    }

    /*
        void calcNormalAsTriangles(bool is_smoothing)
    */
    {
        pgMdlData mdl_data;

        pgAssertThrow(mdl_data.calcNormalAsTriangles(false), pgMdlData::ExceptionNotInitialized);

        mdl_data.initAsWriter(2, 7, pgID::ZERO, true);

        mdl_data.setVertPos(0, pgVec(0.0f, 0.0f, 0.0f));
        mdl_data.setVertPos(1, pgVec(0.0f, 0.0f, 10.0f));
        mdl_data.setVertPos(2, pgVec(10.0f, 0.0f, 10.0f));
        mdl_data.setVertPos(3, pgVec(10.0f, 0.0f, 10.0f));
        mdl_data.setVertPos(4, pgVec(0.0f, 0.0f, 10.0f));
        mdl_data.setVertPos(5, pgVec(30.0f, 30.0f, 10.0f));
        mdl_data.setVertPos(6, pgVec(30.0f, 30.0f, 10.0f));

        mdl_data.setNodeVertInfo(0, 0, 3);
        mdl_data.setNodeVertInfo(1, 3, 4);

        mdl_data.calcNormalAsTriangles(false);
        pgAssert(isEqual(mdl_data.getVertN(0), pgVec::Y_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(1), pgVec::Y_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(2), pgVec::Y_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(3), -pgVec::Z_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(4), -pgVec::Z_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(5), -pgVec::Z_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(6), pgVec::Z_UNIT));

        mdl_data.calcNormalAsTriangles(true);
        pgAssert(isEqual(mdl_data.getVertN(0), pgVec::Y_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(1), pgVec::Y_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(2), pgVec::Y_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(3), -pgVec::Z_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(4), -pgVec::Z_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(5), -pgVec::Z_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(6), pgVec::Z_UNIT));

        mdl_data.setNodeVertInfo(0, 0, 7);
        mdl_data.setNodeVertInfo(1, 0, 0);

        mdl_data.calcNormalAsTriangles(false);
        pgAssert(isEqual(mdl_data.getVertN(0), pgVec::Y_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(1), pgVec::Y_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(2), pgVec::Y_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(3), -pgVec::Z_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(4), -pgVec::Z_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(5), -pgVec::Z_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(6), pgVec::Z_UNIT));

        mdl_data.calcNormalAsTriangles(true);
        pgAssert(isEqual(mdl_data.getVertN(0), pgVec::Y_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(1), pgVec::Y_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(2), pgVec::Y_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(3), -pgVec::Z_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(4), -pgVec::Z_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(5), -pgVec::Z_UNIT));
        pgAssert(isEqual(mdl_data.getVertN(6), pgVec::Z_UNIT));

        mdl_data.registerAsResource(pgID_("test_mdl_data"));

        pgAssertThrow(mdl_data.calcNormalAsTriangles(false), pgMdlData::ExceptionInvalidCall);

        pgResMgr::removeResource(pgID_("test_mdl_data"));
    }
}
