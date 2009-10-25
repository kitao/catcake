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


static void assertDrawFlag(const pgDraw& draw, bool is_bound_clip, bool is_sort, //
    bool is_write_rgb, bool is_write_alpha, bool is_write_depth, bool is_backface_culling, bool is_bilinear)
{
    pgAssert(draw.isDrawFlag(pgDraw::FLAG_BOUND_CLIP) == is_bound_clip);
    pgAssert(draw.isDrawFlag(pgDraw::FLAG_SORT) == is_sort);
    pgAssert(draw.isDrawFlag(pgDraw::FLAG_WRITE_RGB) == is_write_rgb);
    pgAssert(draw.isDrawFlag(pgDraw::FLAG_WRITE_ALPHA) == is_write_alpha);
    pgAssert(draw.isDrawFlag(pgDraw::FLAG_WRITE_DEPTH) == is_write_depth);
    pgAssert(draw.isDrawFlag(pgDraw::FLAG_BACKFACE_CULLING) == is_backface_culling);
    pgAssert(draw.isDrawFlag(pgDraw::FLAG_BILINEAR) == is_bilinear);
}


void pgNodeTest()
{
    /*
        bool hasScreen() const
        pgID getScreenID() const
        void setScreenID(pgID scr_id)
        DrawType getType() const
        bool isVisible() const
        void setVisible(bool is_visible)
        pgCol getColor() const
        void setColor(pgCol col)
        DepthTest getDepthTest() const
        void setDepthTest(DepthTest depth_test)
        BlendMode getBlendMode() const
        void setBlendMode(BlendMode blend_mode)
        bool isDrawFlag(DrawFlag draw_flag) const
        void setDrawFlag(DrawFlag draw_flag, bool is_on)
        void clearDrawFlag()
        void copyDrawFlag(const pgDraw* src)
        const pgVec& getClipBoundMin() const
        const pgVec& getClipBoundMax() const
        void setClipBound(const pgVec& bound_min, const pgVec& bound_max)
        const pgVec& getSortCenter() const
        void setSortCenter(const pgVec& sort_center)
        r32 getSortOffset() const
        void setSortOffset(r32 sort_offset)
        pgID getTextureID() const
        void setTextureID(pgID tex_id)
        pgMat& local()

        pgNode()
        void init(pgID scr_id)
    */
    {
        pgDrawMgr::createAfterRes();
        pgDrawMgr::newTexture(pgID_("TEST"), 16, 16, pgTex::FORMAT_RGB);

        pgNode node1;

        pgAssertThrow(node1.hasScreen(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getScreenID(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.setScreenID(pgID::ZERO), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getType(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.isVisible(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.setVisible(true), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getColor(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.setColor(pgCol::ZERO), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getDepthTest(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.setDepthTest(pgDraw::DEPTH_TEST_ALWAYS), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getBlendMode(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.setBlendMode(pgDraw::BLEND_OFF), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.isDrawFlag(pgDraw::FLAG_BACKFACE_CULLING), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.setDrawFlag(pgDraw::FLAG_SORT, true), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.clearDrawFlag(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.copyDrawFlag(NULL), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getClipBoundMin(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getClipBoundMax(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.setClipBound(pgVec::ZERO, pgVec::ZERO), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getSortCenter(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.setSortCenter(pgVec::ZERO), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getSortOffset(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.setSortOffset(0.0f), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getTextureID(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.setTextureID(pgID::ZERO), pgDraw::ExceptionNotInitialized);

        node1.init(pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        node1.init(pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        pgAssert(node1.hasScreen() && node1.getScreenID() == pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        pgAssert(node1.getType() == pgDraw::TYPE_NODE);
        pgAssert(node1.isVisible());
        pgAssert(node1.getColor() == pgCol::FULL);
        pgAssert(node1.getDepthTest() == pgDraw::DEPTH_TEST_GEQUAL);
        pgAssert(node1.getBlendMode() == pgDraw::BLEND_OFF);
        assertDrawFlag(node1, false, false, true, false, true, false, true);
        pgAssert(isEqual(node1.getClipBoundMin(), pgVec::ZERO));
        pgAssert(isEqual(node1.getClipBoundMax(), pgVec::ZERO));
        pgAssert(isEqual(node1.getSortCenter(), pgVec::ZERO));
        pgAssert(isEqual(node1.getSortOffset(), 0.0f));
        pgAssert(node1.getTextureID() == pgID::ZERO);
        pgAssert(isEqual(node1.local(), pgMat::UNIT));

        node1.setScreenID(pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        pgAssert(node1.hasScreen() && node1.getScreenID() == pgDrawMgr::DEFAULT_2D_SCREEN_ID);

        node1.setVisible(false);
        pgAssert(!node1.isVisible());

        node1.setColor(pgCol::ZERO);
        pgAssert(node1.getColor() == pgCol::ZERO);

        node1.setDepthTest(pgDraw::DEPTH_TEST_LESS);
        pgAssert(node1.getDepthTest() == pgDraw::DEPTH_TEST_LESS);

        node1.setBlendMode(pgDraw::BLEND_ADD);
        pgAssert(node1.getBlendMode() == pgDraw::BLEND_ADD);

        node1.setDrawFlag(pgDraw::FLAG_WRITE_ALPHA, true);
        node1.setDrawFlag(pgDraw::FLAG_WRITE_RGB, false);
        pgAssert(node1.isDrawFlag(pgDraw::FLAG_WRITE_ALPHA) && !node1.isDrawFlag(pgDraw::FLAG_WRITE_RGB));

        pgNode node2;

        pgAssertThrow(node1.copyDrawFlag(&node2), pgDraw::ExceptionNotInitialized);

        node2.init(pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        assertDrawFlag(node2, false, false, true, false, true, false, true);

        node2.copyDrawFlag(&node1);
        assertDrawFlag(node2, false, false, false, true, true, false, true);

        node1.clearDrawFlag();
        assertDrawFlag(node1, false, false, false, false, false, false, false);

        node1.setClipBound(pgVec(-1.0f, -2.0f, -3.0f), pgVec(4.0f, 5.0f, 6.0f));
        pgAssert(isEqual(node1.getClipBoundMin(), pgVec(-1.0f, -2.0f, -3.0f)));
        pgAssert(isEqual(node1.getClipBoundMax(), pgVec(4.0f, 5.0f, 6.0f)));
        node1.setClipBound(pgVec::ZERO, pgVec::ZERO);

        node1.setSortCenter(pgVec(1.0f, 2.0f, 3.0f));
        pgAssert(isEqual(node1.getSortCenter(), pgVec(1.0f, 2.0f, 3.0f)));

        node1.setSortOffset(-10.0f);
        pgAssert(isEqual(node1.getSortOffset(), -10.0f));

        node1.setTextureID(pgID_("TEST"));
        pgAssert(node1.getTextureID() == pgID_("TEST"));

        node1.local() = pgMat::ZERO;
        pgAssert(isEqual(node1.local(), pgMat::ZERO));

        pgAssertThrow(node1.setScreenID(pgID::ZERO), pgDraw::ExceptionInvalidArgument);
        pgAssertThrow(node1.setScreenID(pgID::genID()), pgDrawMgr::ExceptionNotFound);
        pgAssertThrow(node1.copyDrawFlag(NULL), pgDraw::ExceptionInvalidArgument);
        pgAssertThrow(node1.setClipBound(pgVec(0.1f, 0.0f, 0.0f), pgVec::ZERO), pgDraw::ExceptionInvalidArgument);
        pgAssertThrow(node1.setClipBound(pgVec(0.0f, 0.1f, 0.0f), pgVec::ZERO), pgDraw::ExceptionInvalidArgument);
        pgAssertThrow(node1.setClipBound(pgVec(0.0f, 0.0f, 0.1f), pgVec::ZERO), pgDraw::ExceptionInvalidArgument);
        pgAssertThrow(node1.setTextureID(pgID::genID()), pgDrawMgr::ExceptionNotFound);

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        bool hasParent() const
        pgDraw* getParentN() const
        void setParent(pgDraw* parent)
        pgDraw* getPrevAllN() const
        pgDraw* getNextAllN() const
        pgDraw* getPrevSiblingN() const
        pgDraw* getNextSiblingN() const
        pgDraw* getLastDescendant() const
        bool hasChild() const
        pgDraw* getFirstChildN() const
        pgDraw* getLastChildN() const
        void moveFirst()
        void moveLast()
        void moveBefore(pgDraw* draw)
        void moveAfter(pgDraw* draw)
        pgMat calcWorld() const
        pgCol calcFinalColor() const

        void init(pgDraw* parent)
    */
    {
        pgDrawMgr::createAfterRes();

        pgNode node1, node2, node3;

        pgAssertThrow(node1.hasParent(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getParentN(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.setParent(NULL), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getPrevAllN(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getNextAllN(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getPrevSiblingN(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getNextSiblingN(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getLastDescendant(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.hasChild(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getFirstChildN(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.getLastChildN(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.moveFirst(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.moveLast(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.moveBefore(NULL), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.moveAfter(NULL), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.calcWorld(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node1.calcFinalColor(), pgDraw::ExceptionNotInitialized);

        node1.init(pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        node2.init(&node1);
        node2.init(&node1);
        node3.init(&node1);
        node3.init(&node1);

        pgAssertThrow(node2.getScreenID(), pgDraw::ExceptionInvalidCall);

        pgAssert(!node1.hasParent() && !node1.getParentN());
        pgAssert(!node1.getPrevAllN() && node1.getNextAllN() == &node2);
        pgAssert(!node1.getPrevSiblingN() && !node1.getNextSiblingN());
        pgAssert(node1.getLastDescendant() == &node3);
        pgAssert(node1.hasChild() && node1.getFirstChildN() == &node2 && node1.getLastChildN() == &node3);

        pgAssert(node2.hasParent() && node2.getParentN() == &node1);
        pgAssert(node2.getPrevAllN() == &node1 && node2.getNextAllN() == &node3);
        pgAssert(!node2.getPrevSiblingN() && node2.getNextSiblingN() == &node3);
        pgAssert(node2.getLastDescendant() == &node2);
        pgAssert(!node2.hasChild() && !node2.getFirstChildN() && !node2.getLastChildN());

        pgAssert(node3.hasParent() && node3.getParentN() == &node1);
        pgAssert(node3.getPrevAllN() == &node2 && !node3.getNextAllN());
        pgAssert(node3.getPrevSiblingN() == &node2 && !node3.getNextSiblingN());
        pgAssert(node3.getLastDescendant() == &node3);
        pgAssert(!node3.hasChild() && !node3.getFirstChildN() && !node3.getLastChildN());

        node2.setParent(&node3);

        pgAssert(!node1.hasParent() && !node1.getParentN());
        pgAssert(!node1.getPrevAllN() && node1.getNextAllN() == &node3);
        pgAssert(!node1.getPrevSiblingN() && !node1.getNextSiblingN());
        pgAssert(node1.getLastDescendant() == &node2);
        pgAssert(node1.hasChild() && node1.getFirstChildN() == &node3 && node1.getLastChildN() == &node3);

        pgAssert(node2.hasParent() && node2.getParentN() == &node3);
        pgAssert(node2.getPrevAllN() == &node3 && !node2.getNextAllN());
        pgAssert(!node2.getPrevSiblingN() && !node2.getNextSiblingN());
        pgAssert(node2.getLastDescendant() == &node2);
        pgAssert(!node2.hasChild() && !node2.getFirstChildN() && !node2.getLastChildN());

        pgAssert(node3.hasParent() && node3.getParentN() == &node1);
        pgAssert(node3.getPrevAllN() == &node1 && node3.getNextAllN() == &node2);
        pgAssert(!node3.getPrevSiblingN() && !node3.getNextSiblingN());
        pgAssert(node3.getLastDescendant() == &node2);
        pgAssert(node3.hasChild() && node3.getFirstChildN() == &node2 && node3.getLastChildN() == &node2);

        node2.setParent(&node1);

        pgAssert(!node1.hasParent() && !node1.getParentN());
        pgAssert(!node1.getPrevAllN() && node1.getNextAllN() == &node3);
        pgAssert(!node1.getPrevSiblingN() && !node1.getNextSiblingN());
        pgAssert(node1.getLastDescendant() == &node2);
        pgAssert(node1.hasChild() && node1.getFirstChildN() == &node3 && node1.getLastChildN() == &node2);

        pgAssert(node2.hasParent() && node2.getParentN() == &node1);
        pgAssert(node2.getPrevAllN() == &node3 && !node2.getNextAllN());
        pgAssert(node2.getPrevSiblingN() == &node3 && !node2.getNextSiblingN());
        pgAssert(node2.getLastDescendant() == &node2);
        pgAssert(!node2.hasChild() && !node2.getFirstChildN() && !node2.getLastChildN());

        pgAssert(node3.hasParent() && node3.getParentN() == &node1);
        pgAssert(node3.getPrevAllN() == &node1 && node3.getNextAllN() == &node2);
        pgAssert(!node3.getPrevSiblingN() && node3.getNextSiblingN() == &node2);
        pgAssert(node3.getLastDescendant() == &node3);
        pgAssert(!node3.hasChild() && !node3.getFirstChildN() && !node3.getLastChildN());

        node2.moveFirst();

        pgAssert(!node1.hasParent() && !node1.getParentN());
        pgAssert(!node1.getPrevAllN() && node1.getNextAllN() == &node2);
        pgAssert(!node1.getPrevSiblingN() && !node1.getNextSiblingN());
        pgAssert(node1.getLastDescendant() == &node3);
        pgAssert(node1.hasChild() && node1.getFirstChildN() == &node2 && node1.getLastChildN() == &node3);

        pgAssert(node2.hasParent() && node2.getParentN() == &node1);
        pgAssert(node2.getPrevAllN() == &node1 && node2.getNextAllN() == &node3);
        pgAssert(!node2.getPrevSiblingN() && node2.getNextSiblingN() == &node3);
        pgAssert(node2.getLastDescendant() == &node2);
        pgAssert(!node2.hasChild() && !node2.getFirstChildN() && !node2.getLastChildN());

        pgAssert(node3.hasParent() && node3.getParentN() == &node1);
        pgAssert(node3.getPrevAllN() == &node2 && !node3.getNextAllN());
        pgAssert(node3.getPrevSiblingN() == &node2 && !node3.getNextSiblingN());
        pgAssert(node3.getLastDescendant() == &node3);
        pgAssert(!node3.hasChild() && !node3.getFirstChildN() && !node3.getLastChildN());

        node2.moveLast();

        pgAssert(!node1.hasParent() && !node1.getParentN());
        pgAssert(!node1.getPrevAllN() && node1.getNextAllN() == &node3);
        pgAssert(!node1.getPrevSiblingN() && !node1.getNextSiblingN());
        pgAssert(node1.getLastDescendant() == &node2);
        pgAssert(node1.hasChild() && node1.getFirstChildN() == &node3 && node1.getLastChildN() == &node2);

        pgAssert(node2.hasParent() && node2.getParentN() == &node1);
        pgAssert(node2.getPrevAllN() == &node3 && !node2.getNextAllN());
        pgAssert(node2.getPrevSiblingN() == &node3 && !node2.getNextSiblingN());
        pgAssert(node2.getLastDescendant() == &node2);
        pgAssert(!node2.hasChild() && !node2.getFirstChildN() && !node2.getLastChildN());

        pgAssert(node3.hasParent() && node3.getParentN() == &node1);
        pgAssert(node3.getPrevAllN() == &node1 && node3.getNextAllN() == &node2);
        pgAssert(!node3.getPrevSiblingN() && node3.getNextSiblingN() == &node2);
        pgAssert(node3.getLastDescendant() == &node3);
        pgAssert(!node3.hasChild() && !node3.getFirstChildN() && !node3.getLastChildN());

        node2.moveBefore(&node3);

        pgAssert(!node1.hasParent() && !node1.getParentN());
        pgAssert(!node1.getPrevAllN() && node1.getNextAllN() == &node2);
        pgAssert(!node1.getPrevSiblingN() && !node1.getNextSiblingN());
        pgAssert(node1.getLastDescendant() == &node3);
        pgAssert(node1.hasChild() && node1.getFirstChildN() == &node2 && node1.getLastChildN() == &node3);

        pgAssert(node2.hasParent() && node2.getParentN() == &node1);
        pgAssert(node2.getPrevAllN() == &node1 && node2.getNextAllN() == &node3);
        pgAssert(!node2.getPrevSiblingN() && node2.getNextSiblingN() == &node3);
        pgAssert(node2.getLastDescendant() == &node2);
        pgAssert(!node2.hasChild() && !node2.getFirstChildN() && !node2.getLastChildN());

        pgAssert(node3.hasParent() && node3.getParentN() == &node1);
        pgAssert(node3.getPrevAllN() == &node2 && !node3.getNextAllN());
        pgAssert(node3.getPrevSiblingN() == &node2 && !node3.getNextSiblingN());
        pgAssert(node3.getLastDescendant() == &node3);
        pgAssert(!node3.hasChild() && !node3.getFirstChildN() && !node3.getLastChildN());

        node2.moveAfter(&node3);

        pgAssert(!node1.hasParent() && !node1.getParentN());
        pgAssert(!node1.getPrevAllN() && node1.getNextAllN() == &node3);
        pgAssert(!node1.getPrevSiblingN() && !node1.getNextSiblingN());
        pgAssert(node1.getLastDescendant() == &node2);
        pgAssert(node1.hasChild() && node1.getFirstChildN() == &node3 && node1.getLastChildN() == &node2);

        pgAssert(node2.hasParent() && node2.getParentN() == &node1);
        pgAssert(node2.getPrevAllN() == &node3 && !node2.getNextAllN());
        pgAssert(node2.getPrevSiblingN() == &node3 && !node2.getNextSiblingN());
        pgAssert(node2.getLastDescendant() == &node2);
        pgAssert(!node2.hasChild() && !node2.getFirstChildN() && !node2.getLastChildN());

        pgAssert(node3.hasParent() && node3.getParentN() == &node1);
        pgAssert(node3.getPrevAllN() == &node1 && node3.getNextAllN() == &node2);
        pgAssert(!node3.getPrevSiblingN() && node3.getNextSiblingN() == &node2);
        pgAssert(node3.getLastDescendant() == &node3);
        pgAssert(!node3.hasChild() && !node3.getFirstChildN() && !node3.getLastChildN());

        pgNode node4;
        node4.init(pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        pgAssertThrow(node1.setParent(NULL), pgDraw::ExceptionInvalidArgument);
        pgAssertThrow(node1.moveBefore(&node4), pgDraw::ExceptionInvalidArgument);
        pgAssertThrow(node2.moveBefore(NULL), pgDraw::ExceptionInvalidArgument);
        pgAssertThrow(node1.moveAfter(&node4), pgDraw::ExceptionInvalidArgument);
        pgAssertThrow(node2.moveAfter(NULL), pgDraw::ExceptionInvalidArgument);

        node3.setParent(&node2);

        node1.local().trans.set(0.0f, 100.0f, 0.0f);
        node1.local() = node1.local().rotateY_s32(90);
        node2.local().trans.set(100.0f, 0.0f, 0.0f);
        node2.local() = node2.local().rotateY_s32(90);
        node3.local().trans.set(10.0f, 20.0f, 30.0f);

        pgAssert(isEqual(node1.calcWorld(), pgMat(pgVec(0.0f, 0.0f, -1.0f), pgVec(0.0f, 1.0f, 0.0f), pgVec(1.0f, 0.0f, 0.0f), pgVec(0.0f, 100.0f, 0.0f))));
        pgAssert(isEqual(node2.calcWorld(), pgMat(pgVec(-1.0f, 0.0f, 0.0f), pgVec(0.0f, 1.0f, 0.0f), pgVec(0.0f, 0.0f, -1.0f), pgVec(0.0f, 100.0f, -100.0f))));
        pgAssert(isEqual(node3.calcWorld(), pgMat(pgVec(-1.0f, 0.0f, 0.0f), pgVec(0.0f, 1.0f, 0.0f), pgVec(0.0f, 0.0f, -1.0f), pgVec(-10.0f, 120.0f, -130.0f))));

        node1.setColor(pgCol(128, 255, 255, 128));
        node2.setColor(pgCol(128, 128, 255, 255));
        node3.setColor(pgCol(255, 255, 64, 0));

        pgAssert(node1.calcFinalColor() == pgCol(128, 255, 255, 128));
        pgAssert(node2.calcFinalColor() == pgCol(65, 128, 255, 128));
        pgAssert(node3.calcFinalColor() == pgCol(65, 128, 64, 0));

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        virtual ~pgDraw()

        ~pgNode()
    */
    {
        pgDrawMgr::createAfterRes();

        pgNode node2, node3;

        {
            pgNode node1;

            node1.init(pgDrawMgr::DEFAULT_3D_SCREEN_ID);
            node2.init(&node1);
            node3.init(&node1);

            pgAssert(!node2.hasScreen() && node2.getParentN() == &node1);
            pgAssert(!node3.hasScreen() && node3.getParentN() == &node1);
        }

        pgAssert(node2.hasScreen() && node2.getScreenID() == pgDrawMgr::INVISIBLE_SCREEN_ID);
        pgAssert(node3.hasScreen() && node3.getScreenID() == pgDrawMgr::INVISIBLE_SCREEN_ID);

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        void setPreset_defaultBlendOff()
        void setPreset_defaultBlendHalf()
        void setPreset_defaultBlendAdd()
    */
    {
        pgDrawMgr::createAfterRes();

        pgNode node;

        pgAssertThrow(node.setPreset_defaultBlendOff(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node.setPreset_defaultBlendHalf(), pgDraw::ExceptionNotInitialized);
        pgAssertThrow(node.setPreset_defaultBlendAdd(), pgDraw::ExceptionNotInitialized);

        node.init(pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        node.setPreset_defaultBlendOff();
        pgAssert(node.getDepthTest() == pgDraw::DEPTH_TEST_GEQUAL);
        pgAssert(node.getBlendMode() == pgDraw::BLEND_OFF);
        assertDrawFlag(node, false, false, true, false, true, false, true);

        node.setPreset_defaultBlendHalf();
        pgAssert(node.getDepthTest() == pgDraw::DEPTH_TEST_GEQUAL);
        pgAssert(node.getBlendMode() == pgDraw::BLEND_HALF);
        assertDrawFlag(node, false, true, true, false, false, false, true);

        node.setPreset_defaultBlendAdd();
        pgAssert(node.getDepthTest() == pgDraw::DEPTH_TEST_GEQUAL);
        pgAssert(node.getBlendMode() == pgDraw::BLEND_ADD);
        assertDrawFlag(node, false, true, true, false, false, false, true);

        pgDrawMgr::destroyBeforeRes();
    }
}
