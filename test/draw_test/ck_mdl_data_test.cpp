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


void ckMdlDataTest()
{
    /*
        static const u32 MODEL_DATA_VERSION
    */
    {
        ckAssert(ckMdlData::MODEL_DATA_VERSION == 50);
    }

    /*
        ckMdlData()
        ~ckMdlData()
        void initAsReader(const void* data, u32 data_size)
        void initAsWriter(u16 node_num, u16 vert_num, ckID tex_id, bool has_normal)
        ModelDataMode getMode() const
        u16 getNodeNum() const
        u16 getVertNum() const
        bool hasNormal() const
        ckID getTextureID() const
        u16 getNodeParentIndex(u16 node_index) const
        void setNodeParentIndex(u16 node_index, u16 parent_index)
        void setNodeParentIndex_noParent(u16 node_index)
        void setNodeNoParent(u16 node_index)
        const ckMat& getNodeLocal(u16 node_index) const
        void setNodeLocal(u16 node_index, const ckMat& local)
        ckPrim::PrimMode getNodePrimMode(u16 node_index) const
        void setNodePrimMode(u16 node_index, ckPrim::PrimMode prim_mode)
        ckPrim::BlendMode getNodeBlendMode(u16 node_index) const
        void setNodeBlendMode(u16 node_index, ckPrim::BlendMode blend_mode)
        u16 getNodeVertIndex(u16 node_index) const
        u16 getNodeVertNum(u16 node_index) const
        void setNodeVertInfo(u16 node_index, u16 vert_index, u16 vert_num)
        const ckPrim::PrimData* getNodePrimData(u16 node_index) const
        const ckVec* getNodeNormalData(u16 node_index) const
        const ckVec& getNodeClipBoundMinForReader(u16 node_index) const
        const ckVec& getNodeClipBoundMaxForReader(u16 node_index) const
        const ckVec& getVertPos(u16 vert_index) const
        void setVertPos(u16 vert_index, const ckVec& pos)
        ckCol getVertCol(u16 vert_index) const
        void setVertCol(u16 vert_index, ckCol col)
        r32 getVertU(u16 vert_index) const
        r32 getVertV(u16 vert_index) const
        void setVertUV(u16 vert_index, r32 u, r32 v)
        const ckVec& getVertN(u16 vert_index) const
        void setVertN(u16 vert_index, const ckVec& n)
        void registerAsResource(ckID res_id)
    */
    {
        ckMdlData mdl_data;

        ckAssertThrow(mdl_data.getMode(), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getNodeNum(), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getVertNum(), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.hasNormal(), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getTextureID(), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getNodeParentIndex(0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.setNodeParentIndex(0, 0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.setNodeParentIndex_noParent(0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getNodeLocal(0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.setNodeLocal(0, ckMat::UNIT), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getNodePrimMode(0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.setNodePrimMode(0, ckPrim::MODE_POINTS), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getNodeBlendMode(0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.setNodeBlendMode(0, ckPrim::BLEND_HALF), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getNodeVertIndex(0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getNodeVertNum(0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.setNodeVertInfo(0, 0, 0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getNodePrimData(0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getNodeNormalData(0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getNodeClipBoundMinForReader(0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getNodeClipBoundMaxForReader(0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getVertPos(0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.setVertPos(0, ckVec::ZERO), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getVertCol(0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.setVertCol(0, ckCol::FULL), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getVertU(0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getVertV(0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.setVertUV(0, 0.0f, 0.0f), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.getVertN(0), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.setVertN(0, ckVec::ZERO), ckMdlData::ExceptionNotInitialized);
        ckAssertThrow(mdl_data.registerAsResource(ckID::ZERO), ckMdlData::ExceptionNotInitialized);

        mdl_data.initAsWriter(2, 3, ckID_("test"), true);

        ckAssert(mdl_data.getMode() == ckMdlData::MODE_WRITE);
        ckAssert(mdl_data.getNodeNum() == 2);
        ckAssert(mdl_data.getVertNum() == 3);
        ckAssert(mdl_data.hasNormal());
        ckAssert(mdl_data.getTextureID() == ckID_("test"));

        for (s32 i = 0; i < 2; i++)
        {
            ckAssert(mdl_data.getNodeParentIndex(i) == 0);
            ckAssert(isEqual(mdl_data.getNodeLocal(i), ckMat::UNIT));
            ckAssert(mdl_data.getNodePrimMode(i) == ckPrim::MODE_POINTS);
            ckAssert(mdl_data.getNodeBlendMode(i) == ckPrim::BLEND_OFF);
            ckAssert(mdl_data.getNodeVertIndex(i) == 0);
            ckAssert(mdl_data.getNodeVertNum(i) == 0);
            ckAssert(mdl_data.getNodePrimData(i));
            ckAssert(mdl_data.getNodeNormalData(i));
        }

        mdl_data.setNodeParentIndex_noParent(0);
        mdl_data.setNodeParentIndex(1, 0);
        ckAssert(mdl_data.getNodeParentIndex(0) == 0 && mdl_data.getNodeParentIndex(1) == 0);

        mdl_data.setNodeLocal(0, ckMat::UNIT);
        mdl_data.setNodeLocal(1, ckMat::UNIT.translate(1.0f, 2.0f, 3.0f));
        ckAssert(isEqual(mdl_data.getNodeLocal(0), ckMat::UNIT) && isEqual(mdl_data.getNodeLocal(1), ckMat::UNIT.translate(1.0f, 2.0f, 3.0f)));

        mdl_data.setNodePrimMode(0, ckPrim::MODE_POINTS);
        mdl_data.setNodePrimMode(1, ckPrim::MODE_LINE_STRIP);
        ckAssert(mdl_data.getNodePrimMode(0) == ckPrim::MODE_POINTS && mdl_data.getNodePrimMode(1) == ckPrim::MODE_LINE_STRIP);

        mdl_data.setNodeBlendMode(0, ckPrim::BLEND_HALF);
        mdl_data.setNodeBlendMode(1, ckPrim::BLEND_ADD);
        ckAssert(mdl_data.getNodeBlendMode(0) == ckPrim::BLEND_HALF && mdl_data.getNodeBlendMode(1) == ckPrim::BLEND_ADD);

        mdl_data.setNodeVertInfo(0, 0, 3);
        mdl_data.setNodeVertInfo(1, 2, 1);
        ckAssert(mdl_data.getNodeVertIndex(0) == 0 && mdl_data.getNodeVertIndex(1) == 2);
        ckAssert(mdl_data.getNodeVertNum(0) == 3 && mdl_data.getNodeVertNum(1) == 1);

        ckAssert(mdl_data.getNodePrimData(0) && mdl_data.getNodePrimData(1));
        ckAssert(mdl_data.getNodeNormalData(0) && mdl_data.getNodeNormalData(1));

        for (s32 i = 0; i < 3; i++)
        {
            ckAssert(isEqual(mdl_data.getVertPos(i), ckVec::ZERO));
            ckAssert(mdl_data.getVertCol(i) == ckCol::FULL);
            ckAssert(isEqual(mdl_data.getVertU(i), 0.0f) && isEqual(mdl_data.getVertV(i), 0.0f));
            ckAssert(isEqual(mdl_data.getVertN(i), ckVec::Z_UNIT));
        }

        mdl_data.setVertPos(0, ckVec(10.0f, -2.0f, -3.0f));
        mdl_data.setVertPos(1, ckVec(-5.0f, 0.0f, 5.0f));

        mdl_data.setVertPos(2, ckVec(1.0f, 2.0f, 3.0f));
        ckAssert(isEqual(mdl_data.getVertPos(2), ckVec(1.0f, 2.0f, 3.0f)));

        mdl_data.setVertCol(2, ckCol(10, 20, 30, 40));
        ckAssert(mdl_data.getVertCol(2) == ckCol(10, 20, 30, 40));

        mdl_data.setVertUV(2, 0.11f, 0.22f);
        ckAssert(isEqual(mdl_data.getVertU(2), 0.11f) && isEqual(mdl_data.getVertV(2), 0.22f));

        mdl_data.setVertN(2, ckVec::X_UNIT);
        ckAssert(isEqual(mdl_data.getVertN(2), ckVec::X_UNIT));

        ckAssertThrow(mdl_data.getNodeClipBoundMinForReader(0), ckMdlData::ExceptionInvalidCall);
        ckAssertThrow(mdl_data.getNodeClipBoundMaxForReader(0), ckMdlData::ExceptionInvalidCall);
        ckAssertThrow(mdl_data.initAsWriter(0, 1, ckID::ZERO, false), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.initAsWriter(1, 0, ckID::ZERO, false), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.getNodeParentIndex(2), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.setNodeParentIndex(2, 0), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.setNodeParentIndex(0, 0), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.setNodeParentIndex_noParent(2), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.getNodeLocal(2), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.setNodeLocal(2, ckMat::UNIT), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.getNodePrimMode(2), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.setNodePrimMode(2, ckPrim::MODE_POINTS), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.getNodeBlendMode(2), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.setNodeBlendMode(2, ckPrim::BLEND_OFF), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.getNodeVertIndex(2), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.getNodeVertNum(2), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.setNodeVertInfo(2, 0, 0), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.setNodeVertInfo(0, 3, 0), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.setNodeVertInfo(0, 0, 4), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.getNodePrimData(2), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.getNodeNormalData(2), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.getVertPos(3), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.setVertPos(3, ckVec::ZERO), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.getVertCol(3), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.setVertCol(3, ckCol::FULL), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.getVertU(3), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.getVertV(3), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.setVertUV(3, 0.0f, 0.0f), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.getVertN(3), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.setVertN(3, ckVec::ZERO), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.registerAsResource(ckID::ZERO), ckMdlData::ExceptionInvalidArgument);

        mdl_data.registerAsResource(ckID_("test_mdl_data"));

        ckAssert(ckResMgr::hasResource(ckID_("test_mdl_data")));
        ckAssert(mdl_data.getMode() == ckMdlData::MODE_READ);

        mdl_data.initAsWriter(1, 2, ckID::ZERO, false);

        ckAssert(!mdl_data.hasNormal());

        ckAssertThrow(mdl_data.getNodeNormalData(0), ckMdlData::ExceptionInvalidCall);
        ckAssertThrow(mdl_data.getVertN(0), ckMdlData::ExceptionInvalidCall);
        ckAssertThrow(mdl_data.setVertN(0, ckVec::ZERO), ckMdlData::ExceptionInvalidCall);

        ckRes res = ckResMgr::getResource(ckID_("test_mdl_data"));

        mdl_data.initAsReader(res.getData<void>(), res.getDataSize());

        ckAssert(mdl_data.getMode() == ckMdlData::MODE_READ);
        ckAssert(mdl_data.getNodeNum() == 2);
        ckAssert(mdl_data.getVertNum() == 3);
        ckAssert(mdl_data.hasNormal());
        ckAssert(mdl_data.getTextureID() == ckID_("test"));
        ckAssert(mdl_data.getNodeParentIndex(0) == 0 && mdl_data.getNodeParentIndex(1) == 0);
        ckAssert(isEqual(mdl_data.getNodeLocal(0), ckMat::UNIT) && isEqual(mdl_data.getNodeLocal(1), ckMat::UNIT.translate(1.0f, 2.0f, 3.0f)));
        ckAssert(mdl_data.getNodePrimMode(0) == ckPrim::MODE_POINTS && mdl_data.getNodePrimMode(1) == ckPrim::MODE_LINE_STRIP);
        ckAssert(mdl_data.getNodeBlendMode(0) == ckPrim::BLEND_HALF && mdl_data.getNodeBlendMode(1) == ckPrim::BLEND_ADD);
        ckAssert(mdl_data.getNodeVertIndex(0) == 0 && mdl_data.getNodeVertIndex(1) == 2);
        ckAssert(mdl_data.getNodeVertNum(0) == 3 && mdl_data.getNodeVertNum(1) == 1);
        ckAssert(isEqual(mdl_data.getNodeClipBoundMinForReader(0), ckVec(-5.0f, -2.0f, -3.0f)));
        ckAssert(isEqual(mdl_data.getNodeClipBoundMinForReader(1), ckVec(1.0f, 2.0f, 3.0f)));
        ckAssert(isEqual(mdl_data.getNodeClipBoundMaxForReader(0), ckVec(10.0f, 2.0f, 5.0f)));
        ckAssert(isEqual(mdl_data.getNodeClipBoundMaxForReader(1), ckVec(1.0f, 2.0f, 3.0f)));
        ckAssert(mdl_data.getNodePrimData(0) && mdl_data.getNodePrimData(1));
        ckAssert(mdl_data.getNodeNormalData(0) && mdl_data.getNodeNormalData(1));
        ckAssert(isEqual(mdl_data.getVertPos(2), ckVec(1.0f, 2.0f, 3.0f)));
        ckAssert(mdl_data.getVertCol(2) == ckCol(10, 20, 30, 40));
        ckAssert(isEqual(mdl_data.getVertU(2), 0.11f) && isEqual(mdl_data.getVertV(2), 0.22f));
        ckAssert(isEqual(mdl_data.getVertN(2), ckVec::X_UNIT));

        ckAssertThrow(mdl_data.initAsReader(NULL, 100), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.initAsReader("dummy", 0), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.setNodeParentIndex(0, 1), ckMdlData::ExceptionInvalidCall);
        ckAssertThrow(mdl_data.setNodeParentIndex_noParent(0), ckMdlData::ExceptionInvalidCall);
        ckAssertThrow(mdl_data.setNodeLocal(0, ckMat::UNIT), ckMdlData::ExceptionInvalidCall);
        ckAssertThrow(mdl_data.setNodePrimMode(0, ckPrim::MODE_POINTS), ckMdlData::ExceptionInvalidCall);
        ckAssertThrow(mdl_data.setNodeBlendMode(0, ckPrim::BLEND_OFF), ckMdlData::ExceptionInvalidCall);
        ckAssertThrow(mdl_data.setNodeVertInfo(0, 0, 0), ckMdlData::ExceptionInvalidCall);
        ckAssertThrow(mdl_data.getNodeClipBoundMinForReader(2), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.getNodeClipBoundMaxForReader(2), ckMdlData::ExceptionInvalidArgument);
        ckAssertThrow(mdl_data.setVertPos(0, ckVec::ZERO), ckMdlData::ExceptionInvalidCall);
        ckAssertThrow(mdl_data.setVertCol(0, ckCol::FULL), ckMdlData::ExceptionInvalidCall);
        ckAssertThrow(mdl_data.setVertUV(0, 0.0f, 0.0f), ckMdlData::ExceptionInvalidCall);
        ckAssertThrow(mdl_data.setVertN(0, ckVec::ZERO), ckMdlData::ExceptionInvalidCall);
        ckAssertThrow(mdl_data.registerAsResource(ckID::genID()), ckMdlData::ExceptionInvalidCall);

        ckResMgr::removeResource(ckID_("test_mdl_data"));
    }

    /*
        void calcNormalAsTriangles(bool is_smoothing)
    */
    {
        ckMdlData mdl_data;

        ckAssertThrow(mdl_data.calcNormalAsTriangles(false), ckMdlData::ExceptionNotInitialized);

        mdl_data.initAsWriter(2, 7, ckID::ZERO, true);

        mdl_data.setVertPos(0, ckVec(0.0f, 0.0f, 0.0f));
        mdl_data.setVertPos(1, ckVec(0.0f, 0.0f, 10.0f));
        mdl_data.setVertPos(2, ckVec(10.0f, 0.0f, 10.0f));
        mdl_data.setVertPos(3, ckVec(10.0f, 0.0f, 10.0f));
        mdl_data.setVertPos(4, ckVec(0.0f, 0.0f, 10.0f));
        mdl_data.setVertPos(5, ckVec(30.0f, 30.0f, 10.0f));
        mdl_data.setVertPos(6, ckVec(30.0f, 30.0f, 10.0f));

        mdl_data.setNodeVertInfo(0, 0, 3);
        mdl_data.setNodeVertInfo(1, 3, 4);

        mdl_data.calcNormalAsTriangles(false);
        ckAssert(isEqual(mdl_data.getVertN(0), ckVec::Y_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(1), ckVec::Y_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(2), ckVec::Y_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(3), -ckVec::Z_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(4), -ckVec::Z_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(5), -ckVec::Z_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(6), ckVec::Z_UNIT));

        mdl_data.calcNormalAsTriangles(true);
        ckAssert(isEqual(mdl_data.getVertN(0), ckVec::Y_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(1), ckVec::Y_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(2), ckVec::Y_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(3), -ckVec::Z_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(4), -ckVec::Z_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(5), -ckVec::Z_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(6), ckVec::Z_UNIT));

        mdl_data.setNodeVertInfo(0, 0, 7);
        mdl_data.setNodeVertInfo(1, 0, 0);

        mdl_data.calcNormalAsTriangles(false);
        ckAssert(isEqual(mdl_data.getVertN(0), ckVec::Y_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(1), ckVec::Y_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(2), ckVec::Y_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(3), -ckVec::Z_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(4), -ckVec::Z_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(5), -ckVec::Z_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(6), ckVec::Z_UNIT));

        mdl_data.calcNormalAsTriangles(true);
        ckAssert(isEqual(mdl_data.getVertN(0), ckVec::Y_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(1), ckVec::Y_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(2), ckVec::Y_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(3), -ckVec::Z_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(4), -ckVec::Z_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(5), -ckVec::Z_UNIT));
        ckAssert(isEqual(mdl_data.getVertN(6), ckVec::Z_UNIT));

        mdl_data.registerAsResource(ckID_("test_mdl_data"));

        ckAssertThrow(mdl_data.calcNormalAsTriangles(false), ckMdlData::ExceptionInvalidCall);

        ckResMgr::removeResource(ckID_("test_mdl_data"));
    }
}
