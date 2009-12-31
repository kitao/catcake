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


static void assertDrawFlag(const ckDraw& draw, bool is_bound_clip, bool is_sort, //
    bool is_write_rgb, bool is_write_alpha, bool is_write_depth, bool is_backface_culling, bool is_bilinear)
{
    ckAssert(draw.isDrawFlag(ckDraw::FLAG_BOUND_CLIP) == is_bound_clip);
    ckAssert(draw.isDrawFlag(ckDraw::FLAG_SORT) == is_sort);
    ckAssert(draw.isDrawFlag(ckDraw::FLAG_WRITE_RGB) == is_write_rgb);
    ckAssert(draw.isDrawFlag(ckDraw::FLAG_WRITE_ALPHA) == is_write_alpha);
    ckAssert(draw.isDrawFlag(ckDraw::FLAG_WRITE_DEPTH) == is_write_depth);
    ckAssert(draw.isDrawFlag(ckDraw::FLAG_BACKFACE_CULLING) == is_backface_culling);
    ckAssert(draw.isDrawFlag(ckDraw::FLAG_BILINEAR) == is_bilinear);
}


void ckNodeTest()
{
    /*
        bool hasScreen() const
        ckID getScreenID() const
        void setScreenID(ckID scr_id)
        DrawType getType() const
        bool isVisible() const
        void setVisible(bool is_visible)
        ckCol getColor() const
        void setColor(ckCol col)
        DepthTest getDepthTest() const
        void setDepthTest(DepthTest depth_test)
        BlendMode getBlendMode() const
        void setBlendMode(BlendMode blend_mode, bool is_preset_setting)
        bool isDrawFlag(DrawFlag draw_flag) const
        void setDrawFlag(DrawFlag draw_flag, bool is_on)
        void clearDrawFlag()
        void copyDrawFlag(const ckDraw* src)
        const ckVec& getClipBoundMin() const
        const ckVec& getClipBoundMax() const
        void setClipBound(const ckVec& bound_min, const ckVec& bound_max)
        const ckVec& getSortCenter() const
        void setSortCenter(const ckVec& sort_center)
        r32 getSortOffset() const
        void setSortOffset(r32 sort_offset)
        ckID getTextureID() const
        void setTextureID(ckID tex_id)
        ckMat& local()

        ckNode()
        void init(ckID scr_id)
    */
    {
        ckDrawMgr::createAfterRes();
        ckDrawMgr::newTexture(ckID_("TEST"), 16, 16, ckTex::FORMAT_RGB);

        ckNode node1;

        ckAssertThrow(node1.hasScreen(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getScreenID(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.setScreenID(ckID::ZERO), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getType(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.isVisible(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.setVisible(true), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getColor(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.setColor(ckCol::ZERO), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getDepthTest(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.setDepthTest(ckDraw::DEPTH_TEST_ALWAYS), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getBlendMode(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.setBlendMode(ckDraw::BLEND_OFF, true), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.isDrawFlag(ckDraw::FLAG_BACKFACE_CULLING), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.setDrawFlag(ckDraw::FLAG_SORT, true), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.clearDrawFlag(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.copyDrawFlag(NULL), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getClipBoundMin(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getClipBoundMax(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.setClipBound(ckVec::ZERO, ckVec::ZERO), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getSortCenter(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.setSortCenter(ckVec::ZERO), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getSortOffset(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.setSortOffset(0.0f), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getTextureID(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.setTextureID(ckID::ZERO), ckDraw::ExceptionNotInitialized);

        node1.init(ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        node1.init(ckDrawMgr::DEFAULT_3D_SCREEN_ID);

        ckAssert(node1.hasScreen() && node1.getScreenID() == ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        ckAssert(node1.getType() == ckDraw::TYPE_NODE);
        ckAssert(node1.isVisible());
        ckAssert(node1.getColor() == ckCol::FULL);
        ckAssert(node1.getDepthTest() == ckDraw::DEPTH_TEST_GEQUAL);
        ckAssert(node1.getBlendMode() == ckDraw::BLEND_OFF);
        assertDrawFlag(node1, false, false, true, false, true, false, true);
        ckAssert(isEqual(node1.getClipBoundMin(), ckVec::ZERO));
        ckAssert(isEqual(node1.getClipBoundMax(), ckVec::ZERO));
        ckAssert(isEqual(node1.getSortCenter(), ckVec::ZERO));
        ckAssert(isEqual(node1.getSortOffset(), 0.0f));
        ckAssert(node1.getTextureID() == ckID::ZERO);
        ckAssert(isEqual(node1.local(), ckMat::UNIT));

        node1.setScreenID(ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        ckAssert(node1.hasScreen() && node1.getScreenID() == ckDrawMgr::DEFAULT_2D_SCREEN_ID);

        node1.setVisible(false);
        ckAssert(!node1.isVisible());

        node1.setColor(ckCol::ZERO);
        ckAssert(node1.getColor() == ckCol::ZERO);

        node1.setDepthTest(ckDraw::DEPTH_TEST_LESS);
        ckAssert(node1.getDepthTest() == ckDraw::DEPTH_TEST_LESS);

        node1.setBlendMode(ckDraw::BLEND_ADD, false);
        ckAssert(node1.getDepthTest() == ckDraw::DEPTH_TEST_LESS);
        ckAssert(node1.getBlendMode() == ckDraw::BLEND_ADD);
        assertDrawFlag(node1, false, false, true, false, true, false, true);

        node1.setBlendMode(ckDraw::BLEND_OFF, true);
        ckAssert(node1.getDepthTest() == ckDraw::DEPTH_TEST_GEQUAL);
        ckAssert(node1.getBlendMode() == ckDraw::BLEND_OFF);
        assertDrawFlag(node1, false, false, true, false, true, false, true);

        node1.setBlendMode(ckDraw::BLEND_HALF, true);
        ckAssert(node1.getDepthTest() == ckDraw::DEPTH_TEST_GEQUAL);
        ckAssert(node1.getBlendMode() == ckDraw::BLEND_HALF);
        assertDrawFlag(node1, false, true, true, false, false, false, true);

        node1.setBlendMode(ckDraw::BLEND_ADD, true);
        ckAssert(node1.getDepthTest() == ckDraw::DEPTH_TEST_GEQUAL);
        ckAssert(node1.getBlendMode() == ckDraw::BLEND_ADD);
        assertDrawFlag(node1, false, true, true, false, false, false, true);

        node1.setBlendMode(ckDraw::BLEND_DEST_ALPHA, true);
        ckAssert(node1.getDepthTest() == ckDraw::DEPTH_TEST_GEQUAL);
        ckAssert(node1.getBlendMode() == ckDraw::BLEND_DEST_ALPHA);
        assertDrawFlag(node1, false, false, true, false, true, false, true);

        node1.setDrawFlag(ckDraw::FLAG_WRITE_ALPHA, true);
        node1.setDrawFlag(ckDraw::FLAG_WRITE_RGB, false);
        ckAssert(node1.isDrawFlag(ckDraw::FLAG_WRITE_ALPHA) && !node1.isDrawFlag(ckDraw::FLAG_WRITE_RGB));

        ckNode node2;

        ckAssertThrow(node1.copyDrawFlag(&node2), ckDraw::ExceptionNotInitialized);

        node2.init(ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        assertDrawFlag(node2, false, false, true, false, true, false, true);

        node2.copyDrawFlag(&node1);
        assertDrawFlag(node2, false, false, false, true, true, false, true);

        node1.clearDrawFlag();
        assertDrawFlag(node1, false, false, false, false, false, false, false);

        node1.setClipBound(ckVec(-1.0f, -2.0f, -3.0f), ckVec(4.0f, 5.0f, 6.0f));
        ckAssert(isEqual(node1.getClipBoundMin(), ckVec(-1.0f, -2.0f, -3.0f)));
        ckAssert(isEqual(node1.getClipBoundMax(), ckVec(4.0f, 5.0f, 6.0f)));
        node1.setClipBound(ckVec::ZERO, ckVec::ZERO);

        node1.setSortCenter(ckVec(1.0f, 2.0f, 3.0f));
        ckAssert(isEqual(node1.getSortCenter(), ckVec(1.0f, 2.0f, 3.0f)));

        node1.setSortOffset(-10.0f);
        ckAssert(isEqual(node1.getSortOffset(), -10.0f));

        node1.setTextureID(ckID_("TEST"));
        ckAssert(node1.getTextureID() == ckID_("TEST"));

        node1.local() = ckMat::ZERO;
        ckAssert(isEqual(node1.local(), ckMat::ZERO));

        ckAssertThrow(node1.setScreenID(ckID::ZERO), ckDraw::ExceptionInvalidArgument);
        ckAssertThrow(node1.setScreenID(ckID::genID()), ckDrawMgr::ExceptionNotFound);
        ckAssertThrow(node1.copyDrawFlag(NULL), ckDraw::ExceptionInvalidArgument);
        ckAssertThrow(node1.setClipBound(ckVec(0.1f, 0.0f, 0.0f), ckVec::ZERO), ckDraw::ExceptionInvalidArgument);
        ckAssertThrow(node1.setClipBound(ckVec(0.0f, 0.1f, 0.0f), ckVec::ZERO), ckDraw::ExceptionInvalidArgument);
        ckAssertThrow(node1.setClipBound(ckVec(0.0f, 0.0f, 0.1f), ckVec::ZERO), ckDraw::ExceptionInvalidArgument);
        ckAssertThrow(node1.setTextureID(ckID::genID()), ckDrawMgr::ExceptionNotFound);

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        bool hasParent() const
        ckDraw* getParentN() const
        void setParent(ckDraw* parent)
        ckDraw* getPrevAllN() const
        ckDraw* getNextAllN() const
        ckDraw* getPrevSiblingN() const
        ckDraw* getNextSiblingN() const
        ckDraw* getLastDescendant() const
        bool hasChild() const
        ckDraw* getFirstChildN() const
        ckDraw* getLastChildN() const
        void moveFirst()
        void moveLast()
        void moveBefore(ckDraw* draw)
        void moveAfter(ckDraw* draw)
        ckMat calcWorld() const
        ckCol calcFinalColor() const

        void init(ckDraw* parent)
    */
    {
        ckDrawMgr::createAfterRes();

        ckNode node1, node2, node3;

        ckAssertThrow(node1.hasParent(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getParentN(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.setParent(NULL), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getPrevAllN(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getNextAllN(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getPrevSiblingN(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getNextSiblingN(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getLastDescendant(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.hasChild(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getFirstChildN(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.getLastChildN(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.moveFirst(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.moveLast(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.moveBefore(NULL), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.moveAfter(NULL), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.calcWorld(), ckDraw::ExceptionNotInitialized);
        ckAssertThrow(node1.calcFinalColor(), ckDraw::ExceptionNotInitialized);

        node1.init(ckDrawMgr::DEFAULT_3D_SCREEN_ID);

        node2.init(&node1);
        node2.init(&node1);
        node3.init(&node1);
        node3.init(&node1);

        ckAssertThrow(node2.getScreenID(), ckDraw::ExceptionInvalidCall);

        ckAssert(!node1.hasParent() && !node1.getParentN());
        ckAssert(!node1.getPrevAllN() && node1.getNextAllN() == &node2);
        ckAssert(!node1.getPrevSiblingN() && !node1.getNextSiblingN());
        ckAssert(node1.getLastDescendant() == &node3);
        ckAssert(node1.hasChild() && node1.getFirstChildN() == &node2 && node1.getLastChildN() == &node3);

        ckAssert(node2.hasParent() && node2.getParentN() == &node1);
        ckAssert(node2.getPrevAllN() == &node1 && node2.getNextAllN() == &node3);
        ckAssert(!node2.getPrevSiblingN() && node2.getNextSiblingN() == &node3);
        ckAssert(node2.getLastDescendant() == &node2);
        ckAssert(!node2.hasChild() && !node2.getFirstChildN() && !node2.getLastChildN());

        ckAssert(node3.hasParent() && node3.getParentN() == &node1);
        ckAssert(node3.getPrevAllN() == &node2 && !node3.getNextAllN());
        ckAssert(node3.getPrevSiblingN() == &node2 && !node3.getNextSiblingN());
        ckAssert(node3.getLastDescendant() == &node3);
        ckAssert(!node3.hasChild() && !node3.getFirstChildN() && !node3.getLastChildN());

        node2.setParent(&node3);

        ckAssert(!node1.hasParent() && !node1.getParentN());
        ckAssert(!node1.getPrevAllN() && node1.getNextAllN() == &node3);
        ckAssert(!node1.getPrevSiblingN() && !node1.getNextSiblingN());
        ckAssert(node1.getLastDescendant() == &node2);
        ckAssert(node1.hasChild() && node1.getFirstChildN() == &node3 && node1.getLastChildN() == &node3);

        ckAssert(node2.hasParent() && node2.getParentN() == &node3);
        ckAssert(node2.getPrevAllN() == &node3 && !node2.getNextAllN());
        ckAssert(!node2.getPrevSiblingN() && !node2.getNextSiblingN());
        ckAssert(node2.getLastDescendant() == &node2);
        ckAssert(!node2.hasChild() && !node2.getFirstChildN() && !node2.getLastChildN());

        ckAssert(node3.hasParent() && node3.getParentN() == &node1);
        ckAssert(node3.getPrevAllN() == &node1 && node3.getNextAllN() == &node2);
        ckAssert(!node3.getPrevSiblingN() && !node3.getNextSiblingN());
        ckAssert(node3.getLastDescendant() == &node2);
        ckAssert(node3.hasChild() && node3.getFirstChildN() == &node2 && node3.getLastChildN() == &node2);

        node2.setParent(&node1);

        ckAssert(!node1.hasParent() && !node1.getParentN());
        ckAssert(!node1.getPrevAllN() && node1.getNextAllN() == &node3);
        ckAssert(!node1.getPrevSiblingN() && !node1.getNextSiblingN());
        ckAssert(node1.getLastDescendant() == &node2);
        ckAssert(node1.hasChild() && node1.getFirstChildN() == &node3 && node1.getLastChildN() == &node2);

        ckAssert(node2.hasParent() && node2.getParentN() == &node1);
        ckAssert(node2.getPrevAllN() == &node3 && !node2.getNextAllN());
        ckAssert(node2.getPrevSiblingN() == &node3 && !node2.getNextSiblingN());
        ckAssert(node2.getLastDescendant() == &node2);
        ckAssert(!node2.hasChild() && !node2.getFirstChildN() && !node2.getLastChildN());

        ckAssert(node3.hasParent() && node3.getParentN() == &node1);
        ckAssert(node3.getPrevAllN() == &node1 && node3.getNextAllN() == &node2);
        ckAssert(!node3.getPrevSiblingN() && node3.getNextSiblingN() == &node2);
        ckAssert(node3.getLastDescendant() == &node3);
        ckAssert(!node3.hasChild() && !node3.getFirstChildN() && !node3.getLastChildN());

        node2.moveFirst();

        ckAssert(!node1.hasParent() && !node1.getParentN());
        ckAssert(!node1.getPrevAllN() && node1.getNextAllN() == &node2);
        ckAssert(!node1.getPrevSiblingN() && !node1.getNextSiblingN());
        ckAssert(node1.getLastDescendant() == &node3);
        ckAssert(node1.hasChild() && node1.getFirstChildN() == &node2 && node1.getLastChildN() == &node3);

        ckAssert(node2.hasParent() && node2.getParentN() == &node1);
        ckAssert(node2.getPrevAllN() == &node1 && node2.getNextAllN() == &node3);
        ckAssert(!node2.getPrevSiblingN() && node2.getNextSiblingN() == &node3);
        ckAssert(node2.getLastDescendant() == &node2);
        ckAssert(!node2.hasChild() && !node2.getFirstChildN() && !node2.getLastChildN());

        ckAssert(node3.hasParent() && node3.getParentN() == &node1);
        ckAssert(node3.getPrevAllN() == &node2 && !node3.getNextAllN());
        ckAssert(node3.getPrevSiblingN() == &node2 && !node3.getNextSiblingN());
        ckAssert(node3.getLastDescendant() == &node3);
        ckAssert(!node3.hasChild() && !node3.getFirstChildN() && !node3.getLastChildN());

        node2.moveLast();

        ckAssert(!node1.hasParent() && !node1.getParentN());
        ckAssert(!node1.getPrevAllN() && node1.getNextAllN() == &node3);
        ckAssert(!node1.getPrevSiblingN() && !node1.getNextSiblingN());
        ckAssert(node1.getLastDescendant() == &node2);
        ckAssert(node1.hasChild() && node1.getFirstChildN() == &node3 && node1.getLastChildN() == &node2);

        ckAssert(node2.hasParent() && node2.getParentN() == &node1);
        ckAssert(node2.getPrevAllN() == &node3 && !node2.getNextAllN());
        ckAssert(node2.getPrevSiblingN() == &node3 && !node2.getNextSiblingN());
        ckAssert(node2.getLastDescendant() == &node2);
        ckAssert(!node2.hasChild() && !node2.getFirstChildN() && !node2.getLastChildN());

        ckAssert(node3.hasParent() && node3.getParentN() == &node1);
        ckAssert(node3.getPrevAllN() == &node1 && node3.getNextAllN() == &node2);
        ckAssert(!node3.getPrevSiblingN() && node3.getNextSiblingN() == &node2);
        ckAssert(node3.getLastDescendant() == &node3);
        ckAssert(!node3.hasChild() && !node3.getFirstChildN() && !node3.getLastChildN());

        node2.moveBefore(&node3);

        ckAssert(!node1.hasParent() && !node1.getParentN());
        ckAssert(!node1.getPrevAllN() && node1.getNextAllN() == &node2);
        ckAssert(!node1.getPrevSiblingN() && !node1.getNextSiblingN());
        ckAssert(node1.getLastDescendant() == &node3);
        ckAssert(node1.hasChild() && node1.getFirstChildN() == &node2 && node1.getLastChildN() == &node3);

        ckAssert(node2.hasParent() && node2.getParentN() == &node1);
        ckAssert(node2.getPrevAllN() == &node1 && node2.getNextAllN() == &node3);
        ckAssert(!node2.getPrevSiblingN() && node2.getNextSiblingN() == &node3);
        ckAssert(node2.getLastDescendant() == &node2);
        ckAssert(!node2.hasChild() && !node2.getFirstChildN() && !node2.getLastChildN());

        ckAssert(node3.hasParent() && node3.getParentN() == &node1);
        ckAssert(node3.getPrevAllN() == &node2 && !node3.getNextAllN());
        ckAssert(node3.getPrevSiblingN() == &node2 && !node3.getNextSiblingN());
        ckAssert(node3.getLastDescendant() == &node3);
        ckAssert(!node3.hasChild() && !node3.getFirstChildN() && !node3.getLastChildN());

        node2.moveAfter(&node3);

        ckAssert(!node1.hasParent() && !node1.getParentN());
        ckAssert(!node1.getPrevAllN() && node1.getNextAllN() == &node3);
        ckAssert(!node1.getPrevSiblingN() && !node1.getNextSiblingN());
        ckAssert(node1.getLastDescendant() == &node2);
        ckAssert(node1.hasChild() && node1.getFirstChildN() == &node3 && node1.getLastChildN() == &node2);

        ckAssert(node2.hasParent() && node2.getParentN() == &node1);
        ckAssert(node2.getPrevAllN() == &node3 && !node2.getNextAllN());
        ckAssert(node2.getPrevSiblingN() == &node3 && !node2.getNextSiblingN());
        ckAssert(node2.getLastDescendant() == &node2);
        ckAssert(!node2.hasChild() && !node2.getFirstChildN() && !node2.getLastChildN());

        ckAssert(node3.hasParent() && node3.getParentN() == &node1);
        ckAssert(node3.getPrevAllN() == &node1 && node3.getNextAllN() == &node2);
        ckAssert(!node3.getPrevSiblingN() && node3.getNextSiblingN() == &node2);
        ckAssert(node3.getLastDescendant() == &node3);
        ckAssert(!node3.hasChild() && !node3.getFirstChildN() && !node3.getLastChildN());

        ckNode node4;
        node4.init(ckDrawMgr::DEFAULT_3D_SCREEN_ID);

        ckAssertThrow(node1.setParent(NULL), ckDraw::ExceptionInvalidArgument);
        ckAssertThrow(node1.moveBefore(&node4), ckDraw::ExceptionInvalidArgument);
        ckAssertThrow(node2.moveBefore(NULL), ckDraw::ExceptionInvalidArgument);
        ckAssertThrow(node1.moveAfter(&node4), ckDraw::ExceptionInvalidArgument);
        ckAssertThrow(node2.moveAfter(NULL), ckDraw::ExceptionInvalidArgument);

        node3.setParent(&node2);

        node1.local().trans.set(0.0f, 100.0f, 0.0f);
        node1.local() = node1.local().rotateY_s32(90);
        node2.local().trans.set(100.0f, 0.0f, 0.0f);
        node2.local() = node2.local().rotateY_s32(90);
        node3.local().trans.set(10.0f, 20.0f, 30.0f);

        ckAssert(isEqual(node1.calcWorld(), ckMat(ckVec(0.0f, 0.0f, -1.0f), ckVec(0.0f, 1.0f, 0.0f), ckVec(1.0f, 0.0f, 0.0f), ckVec(0.0f, 100.0f, 0.0f))));
        ckAssert(isEqual(node2.calcWorld(), ckMat(ckVec(-1.0f, 0.0f, 0.0f), ckVec(0.0f, 1.0f, 0.0f), ckVec(0.0f, 0.0f, -1.0f), ckVec(0.0f, 100.0f, -100.0f))));
        ckAssert(isEqual(node3.calcWorld(), ckMat(ckVec(-1.0f, 0.0f, 0.0f), ckVec(0.0f, 1.0f, 0.0f), ckVec(0.0f, 0.0f, -1.0f), ckVec(-10.0f, 120.0f, -130.0f))));

        node1.setColor(ckCol(128, 255, 255, 128));
        node2.setColor(ckCol(128, 128, 255, 255));
        node3.setColor(ckCol(255, 255, 64, 0));

        ckAssert(node1.calcFinalColor() == ckCol(128, 255, 255, 128));
        ckAssert(node2.calcFinalColor() == ckCol(65, 128, 255, 128));
        ckAssert(node3.calcFinalColor() == ckCol(65, 128, 64, 0));

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        virtual ~ckDraw()

        ~ckNode()
    */
    {
        ckDrawMgr::createAfterRes();

        ckNode node2, node3;

        {
            ckNode node1;

            node1.init(ckDrawMgr::DEFAULT_3D_SCREEN_ID);
            node2.init(&node1);
            node3.init(&node1);

            ckAssert(!node2.hasScreen() && node2.getParentN() == &node1);
            ckAssert(!node3.hasScreen() && node3.getParentN() == &node1);
        }

        ckAssert(node2.hasScreen() && node2.getScreenID() == ckDrawMgr::INVISIBLE_SCREEN_ID);
        ckAssert(node3.hasScreen() && node3.getScreenID() == ckDrawMgr::INVISIBLE_SCREEN_ID);

        ckDrawMgr::destroyBeforeRes();
    }
}
