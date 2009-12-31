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


void ckScrTest()
{
    /*
        static const u32 DEFAULT_FOCUS_DIST
        static const u32 DEFAULT_NEAR_CLIP_DIST
        static const u32 DEFAULT_FAR_CLIP_DIST
    */
    {
        ckAssert(ckScr::DEFAULT_FOCUS_DIST == 1000);
        ckAssert(ckScr::DEFAULT_NEAR_CLIP_DIST == 10);
        ckAssert(ckScr::DEFAULT_FAR_CLIP_DIST == 100000);
    }

    /*
        ckScr* getPrevN() const
        ckScr* getNextN() const
        void moveFirst()
        void moveLast()
        void moveBefore(ckID id)
        void moveAfter(ckID id)
        ckID getID() const
    */
    {
        ckDrawMgr::createAfterRes();

        ckID id1 = ckDrawMgr::INVISIBLE_SCREEN_ID;
        ckID id2 = ckDrawMgr::DEFAULT_3D_SCREEN_ID;
        ckID id3 = ckDrawMgr::DEFAULT_2D_SCREEN_ID;

        ckScr* scr1 = ckDrawMgr::getScreen(id1);
        ckScr* scr2 = ckDrawMgr::getScreen(id2);
        ckScr* scr3 = ckDrawMgr::getScreen(id3);

        ckAssert(scr1->getID() == id1);
        ckAssert(scr2->getID() == id2);
        ckAssert(scr3->getID() == id3);

        ckAssert(ckDrawMgr::getFirstScreenN() == scr1 && ckDrawMgr::getLastScreenN() == scr3);
        ckAssert(!scr1->getPrevN() && scr1->getNextN() == scr2);
        ckAssert(scr2->getPrevN() == scr1 && scr2->getNextN() == scr3);
        ckAssert(scr3->getPrevN() == scr2 && !scr3->getNextN());

        scr2->moveFirst();

        ckAssert(ckDrawMgr::getFirstScreenN() == scr2 && ckDrawMgr::getLastScreenN() == scr3);
        ckAssert(!scr2->getPrevN() && scr2->getNextN() == scr1);
        ckAssert(scr1->getPrevN() == scr2 && scr1->getNextN() == scr3);
        ckAssert(scr3->getPrevN() == scr1 && !scr3->getNextN());

        scr1->moveLast();

        ckAssert(ckDrawMgr::getFirstScreenN() == scr2 && ckDrawMgr::getLastScreenN() == scr1);
        ckAssert(!scr2->getPrevN() && scr2->getNextN() == scr3);
        ckAssert(scr3->getPrevN() == scr2 && scr3->getNextN() == scr1);
        ckAssert(scr1->getPrevN() == scr3 && !scr1->getNextN());

        scr3->moveBefore(id2);

        ckAssert(ckDrawMgr::getFirstScreenN() == scr3 && ckDrawMgr::getLastScreenN() == scr1);
        ckAssert(!scr3->getPrevN() && scr3->getNextN() == scr2);
        ckAssert(scr2->getPrevN() == scr3 && scr2->getNextN() == scr1);
        ckAssert(scr1->getPrevN() == scr2 && !scr1->getNextN());

        scr2->moveAfter(id1);

        ckAssert(ckDrawMgr::getFirstScreenN() == scr3 && ckDrawMgr::getLastScreenN() == scr2);
        ckAssert(!scr3->getPrevN() && scr3->getNextN() == scr1);
        ckAssert(scr1->getPrevN() == scr3 && scr1->getNextN() == scr2);
        ckAssert(scr2->getPrevN() == scr1 && !scr2->getNextN());

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        ckDraw* getFirstDrawN()
        ckDraw* getLastDrawN()
    */
    {
        ckDrawMgr::createAfterRes();

        ckScr* scr = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        ckAssert(!scr->getFirstDrawN() && !scr->getLastDrawN());

        ckNode node2, node3;

        {
            ckNode node1;

            node1.init(scr->getID());
            ckAssert(scr->getFirstDrawN() == &node1 && scr->getLastDrawN() == &node1);

            node2.init(scr->getID());
            ckAssert(scr->getFirstDrawN() == &node1 && scr->getLastDrawN() == &node2);

            node3.init(&node2);
            ckAssert(scr->getFirstDrawN() == &node1 && scr->getLastDrawN() == &node3);
        }

        ckAssert(scr->getFirstDrawN() == &node2 && scr->getLastDrawN() == &node3);

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        s16 getLeftInFramebuffer() const
        s16 getTopInFramebuffer() const
        s16 getWidthInFramebuffer() const
        s16 getHeightInFramebuffer() const
        void setAreaInFramebuffer(s16 left, s16 top, u16 width, u16 height)
        r32 getViewWidth() const
        r32 getViewHeight() const
        void setViewSize(r32 width, r32 height)
        bool isActive() const
        void setActive(bool is_active)
        bool isClearColor() const
        bool isClearDepth() const
        void setClearMode(bool is_clear_color, bool is_clear_depth)
        ckCol getClearColor() const
        void setClearColor(ckCol col)
        bool isPerspective() const
        void setPerspective(bool is_perspective)
        r32 getFocusDist() const
        void setFocusDist(r32 focus_dist)
        r32 getNearClipDist() const
        r32 getFarClipDist() const
        void setClipDist(r32 near, r32 far)
        ckMat& view()
    */
    {
        ckDrawMgr::createAfterRes();

        ckScr* scr = ckDrawMgr::newScreen(ckID::genID());

        ckAssert(scr->getLeftInFramebuffer() == 0 && scr->getTopInFramebuffer() == 0 && //
            scr->getWidthInFramebuffer() == ckSysMgr::getFramebufferWidth() && //
            scr->getHeightInFramebuffer() == ckSysMgr::getFramebufferHeight());
        ckAssert(isEqual(scr->getViewWidth(), ckSysMgr::getFramebufferWidth()) && //
            isEqual(scr->getViewHeight(), ckSysMgr::getFramebufferHeight()));
        ckAssert(scr->isActive());
        ckAssert(scr->isClearColor() && scr->isClearDepth());
        ckAssert(scr->getClearColor() == ckCol(255, 0, 0, 0));
        ckAssert(scr->isPerspective());
        ckAssert(isEqual(scr->getFocusDist(), static_cast<r32>(ckScr::DEFAULT_FOCUS_DIST)));
        ckAssert(isEqual(scr->getNearClipDist(), static_cast<r32>(ckScr::DEFAULT_NEAR_CLIP_DIST)) && //
            isEqual(scr->getFarClipDist(), static_cast<r32>(ckScr::DEFAULT_FAR_CLIP_DIST)));
        ckAssert(isEqual(scr->view(), ckMat::UNIT.translate(0.0f, 0.0f, static_cast<r32>(ckScr::DEFAULT_FOCUS_DIST))));

        scr->setAreaInFramebuffer(-10, -20, 30, 40);
        ckAssert(scr->getLeftInFramebuffer() == -10 && scr->getTopInFramebuffer() == -20 && //
            scr->getWidthInFramebuffer() == 30 && scr->getHeightInFramebuffer() == 40);

        scr->setViewSize(123.0f, 456.0f);
        ckAssert(isEqual(scr->getViewWidth(), 123.0f) && isEqual(scr->getViewHeight(), 456.0f));

        scr->setActive(false);
        ckAssert(!scr->isActive());

        scr->setClearMode(false, true);
        ckAssert(!scr->isClearColor() && scr->isClearDepth());

        scr->setClearMode(true, false);
        ckAssert(scr->isClearColor() && !scr->isClearDepth());

        scr->setClearColor(ckCol::FULL);
        ckAssert(scr->getClearColor() == ckCol::FULL);

        scr->setPerspective(false);
        ckAssert(!scr->isPerspective());

        scr->setFocusDist(1234.0f);
        ckAssert(isEqual(scr->getFocusDist(), 1234.0f));

        scr->setClipDist(123.0f, 456.0f);
        ckAssert(isEqual(scr->getNearClipDist(), 123.0f) && isEqual(scr->getFarClipDist(), 456.0f));

        scr->view() = ckMat::ZERO;
        ckAssert(isEqual(scr->view(), ckMat::ZERO));

        ckDrawMgr::renderForSystem();

        ckAssertThrow(scr->setAreaInFramebuffer(123, 456, 0, 456), ckScr::ExceptionInvalidArgument);
        ckAssertThrow(scr->setAreaInFramebuffer(123, 456, 123, 0), ckScr::ExceptionInvalidArgument);
        ckAssertThrow(scr->setFocusDist(0.0f), ckScr::ExceptionInvalidArgument);
        ckAssertThrow(scr->setClipDist(0.0f, 123.0f), ckScr::ExceptionInvalidArgument);
        ckAssertThrow(scr->setClipDist(123.0f, 123.0f), ckScr::ExceptionInvalidArgument);

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        bool hasScreenTexture() const
        ckID getScreenTextureID() const
        void attachScreenTexture(ckTex::TexFormat format)
        void detachScreenTexture()
        void updateScreenTexture(bool is_frame_skip_reset)
    */
    {
        ckDrawMgr::createAfterRes();

        ckScr* scr = ckDrawMgr::newScreen(ckID::genID());
        ckAssert(!scr->hasScreenTexture());

        ckAssertThrow(scr->getScreenTextureID(), ckScr::ExceptionInvalidCall);

        u32 used_memory_size = ckMemMgr::getCurUsedMemorySize();

        ckAssertThrow(scr->attachScreenTexture(ckTex::FORMAT_ALPHA), ckScr::ExceptionInvalidArgument);

        scr->setAreaInFramebuffer(0, 0, ckDrawMgr::getMaxTextureLength() + 1, 10);
        ckAssertThrow(scr->attachScreenTexture(ckTex::FORMAT_RGB), ckScr::ExceptionInvalidCall);

        scr->setAreaInFramebuffer(0, 0, 10, ckDrawMgr::getMaxTextureLength() + 1);
        ckAssertThrow(scr->attachScreenTexture(ckTex::FORMAT_RGB), ckScr::ExceptionInvalidCall);

        scr->setAreaInFramebuffer(10, 20, 30, 40);
        scr->attachScreenTexture(ckTex::FORMAT_RGB);
        ckAssert(scr->hasScreenTexture());

        scr->attachScreenTexture(ckTex::FORMAT_RGBA);

        ckID tex_id = scr->getScreenTextureID();
        ckTex* tex = ckDrawMgr::getTexture(tex_id);

        ckAssert(tex->getID() == tex_id);
        ckAssert(tex->getWidth() == scr->getWidthInFramebuffer() && tex->getHeight() == scr->getHeightInFramebuffer());
        ckAssert(tex->getFormat() == ckTex::FORMAT_RGBA);
        ckAssert(tex->getMode() == ckTex::MODE_FRAMEBUFFER);

        ckAssert(ckMemMgr::getCurUsedMemorySize() > used_memory_size);

        ckDrawMgr::renderForSystem();

        scr->updateScreenTexture(true);
        scr->updateScreenTexture(false);

        ckDrawMgr::renderForSystem();

        scr->detachScreenTexture();
        ckAssertThrow(ckDrawMgr::getTexture(tex_id), ckDrawMgr::ExceptionNotFound);
        ckAssertThrow(scr->updateScreenTexture(false), ckScr::ExceptionInvalidCall);

        scr->detachScreenTexture();

        ckAssert(ckMemMgr::getCurUsedMemorySize() == used_memory_size);

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        ckID getGuestScreenID(u8 index) const
        void setGuestScreenID(u8 index, ckID scr_id)
    */
    {
        ckDrawMgr::createAfterRes();

        ckScr* scr = ckDrawMgr::newScreen(ckID::genID());
        ckAssert(scr->getGuestScreenID(0) == ckID::ZERO && scr->getGuestScreenID(1) == ckID::ZERO && //
            scr->getGuestScreenID(2) == ckID::ZERO && scr->getGuestScreenID(3) == ckID::ZERO);

        scr->setGuestScreenID(0, ckID_("test1"));
        scr->setGuestScreenID(1, ckID_("test2"));
        scr->setGuestScreenID(2, ckID_("test3"));
        scr->setGuestScreenID(3, ckID_("test4"));
        ckAssert(scr->getGuestScreenID(0) == ckID_("test1") && scr->getGuestScreenID(1) == ckID_("test2") && //
            scr->getGuestScreenID(2) == ckID_("test3") && scr->getGuestScreenID(3) == ckID_("test4"));

        ckAssertThrow(scr->getGuestScreenID(4), ckScr::ExceptionInvalidArgument);

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        r32 screenXToFramebufferX(r32 x_in_screen) const
        r32 screenYToFramebufferY(r32 y_in_screen) const
        r32 framebufferXToScreenX(r32 x_in_framebuffer) const
        r32 framebufferYToScreenY(r32 y_in_framebuffer) const
        ckVec worldToScreen(const ckVec& pos_in_world)
        ckVec worldToFramebuffer(const ckVec& pos_in_world)
        ckVec screenToPlane(r32 x_in_screen, r32 y_in_screen, const ckMat& xy_plane) const
        ckVec framebufferToPlane(r32 x_in_framebuffer, r32 y_in_framebuffer, const ckMat& xy_plane) const
        bool isInScreen(const ckVec& pos_in_world)
        bool canBoundClip(const ckMat& world, const ckVec& bound_max, const ckVec& bound_min)
    */
    {
        ckDrawMgr::createAfterRes();

        ckScr* scr = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);

        scr->setAreaInFramebuffer(-100, -200, 400, 300);
        scr->setViewSize(600.0f, 400.0f);
        scr->setFocusDist(200.0f);
        scr->setClipDist(100.0f, 300.0f);
        scr->view() = ckMat::UNIT.translate(10.0f, 20.0f, 30.0f).rotateY_s32(90);

        ckAssert(isEqual(scr->screenXToFramebufferX(0.0f), 100.0f));
        ckAssert(isEqual(scr->screenXToFramebufferX(-300.0f), -100.0f));
        ckAssert(isEqual(scr->screenXToFramebufferX(300.0f), 300.0f));

        ckAssert(isEqual(scr->screenYToFramebufferY(0.0f), -50.0f));
        ckAssert(isEqual(scr->screenYToFramebufferY(200.0f), -200.0f));
        ckAssert(isEqual(scr->screenYToFramebufferY(-200.0f), 100.0f));

        ckAssert(isEqual(scr->framebufferXToScreenX(100.0f), 0.0f));
        ckAssert(isEqual(scr->framebufferXToScreenX(-100.0f), -300.0f));
        ckAssert(isEqual(scr->framebufferXToScreenX(300.0f), 300.0f));

        ckAssert(isEqual(scr->framebufferYToScreenY(-50.0f), 0.0f));
        ckAssert(isEqual(scr->framebufferYToScreenY(-200.0f), 200.0f));
        ckAssert(isEqual(scr->framebufferYToScreenY(100.0f), -200.0f));

        ckAssert(isEqual(scr->worldToScreen(ckVec(-90.0f, 20.0f, 30.0f)), ckVec(0.0, 0.0f, 1.0f)));
        ckAssert(isEqual(scr->worldToScreen(ckVec(-290.0f, 20.0f, 30.0f)), ckVec(0.0f, 0.0f, -1.0f)));
        ckAssert(isEqual(scr->worldToScreen(ckVec(-190.0f, 220.0f, -270.0f)), ckVec(300.0f, 200.0f, -0.5f)));
        ckAssert(isEqual(scr->worldToScreen(ckVec(-190.0f, -180.0f, 330.0f)), ckVec(-300.0f, -200.0f, -0.5f)));

        ckAssert(isEqual(scr->worldToScreen(ckVec(-90.0f, 20.0f, 30.0f)), ckVec(0.0, 0.0f, 1.0f)));
        ckAssert(isEqual(scr->worldToScreen(ckVec(-290.0f, 20.0f, 30.0f)), ckVec(0.0f, 0.0f, -1.0f)));
        ckAssert(isEqual(scr->worldToScreen(ckVec(-190.0f, 220.0f, -270.0f)), ckVec(300.0f, 200.0f, -0.5f)));
        ckAssert(isEqual(scr->worldToScreen(ckVec(-190.0f, -180.0f, 330.0f)), ckVec(-300.0f, -200.0f, -0.5f)));

        ckAssert(isEqual(scr->worldToFramebuffer(ckVec(-90.0f, 20.0f, 30.0f)), ckVec(100.0f, -50.0f, 1.0f)));
        ckAssert(isEqual(scr->worldToFramebuffer(ckVec(-290.0f, 20.0f, 30.0f)), ckVec(100.0f, -50.0f, -1.0f)));
        ckAssert(isEqual(scr->worldToFramebuffer(ckVec(-190.0f, 220.0f, -270.0f)), ckVec(300.0f, -200.0f, -0.5f)));
        ckAssert(isEqual(scr->worldToFramebuffer(ckVec(-190.0f, -180.0f, 330.0f)), ckVec(-100.0f, 100.0f, -0.5f)));

        ckMat plane = ckMat::UNIT.translate(-200.0f, 30.0f, 40.0f).rotateX_s32(10).rotateY_s32(10).rotateZ_s32(10);

        ckVec vec1 = scr->screenToPlane(12.0f, 34.0f, plane);
        ckAssert(isEqual(vec1.z, 0.0f));
        vec1 = scr->worldToScreen(vec1.toGlobalFrom(plane));
        ckAssert(isEqual(vec1.x, 12.0f) && isEqual(vec1.y, 34.0f));

        ckVec vec2 = scr->framebufferToPlane(123.0f, 456.0f, plane);
        ckAssert(isEqual(vec2.z, 0.0f));
        vec2 = scr->worldToFramebuffer(vec2.toGlobalFrom(plane));
        ckAssert(isEqual(vec2.x, 123.0f) && isEqual(vec2.y, 456.0f));

        ckAssert(scr->isInScreen(ckVec(-90.0f, 20.0f, 30.0f)));
        ckAssert(!scr->isInScreen(ckVec(-89.9f, 20.0f, 30.0f)));
        ckAssert(scr->isInScreen(ckVec(-290.0f, 20.0f, 30.0f)));
        ckAssert(!scr->isInScreen(ckVec(-290.1f, 20.0f, 30.0f)));
        ckAssert(scr->isInScreen(ckVec(-190.0f, 220.0f, -270.0f)));
        ckAssert(!scr->isInScreen(ckVec(-190.0f, 220.0f, -270.1f)));
        ckAssert(!scr->isInScreen(ckVec(-190.0f, 220.1f, -270.0f)));
        ckAssert(scr->isInScreen(ckVec(-190.0f, -180.0f, 330.0f)));
        ckAssert(!scr->isInScreen(ckVec(-190.0f, -180.0f, 330.1f)));
        ckAssert(!scr->isInScreen(ckVec(-190.0f, -180.1f, 330.0f)));

        ckVec min(-10.0f, -20.0f, -30.0f);
        ckVec max(40.0f, 50.0f, 60.0f);
        ckMat center = ckMat::UNIT.toGlobalFrom(scr->view());

        ckAssert(scr->canBoundClip(center.translate(355.1f, 0.0f, -200.0f), max, min));
        ckAssert(!scr->canBoundClip(center.translate(354.9f, 0.0f, -200.0f), max, min));
        ckAssert(scr->canBoundClip(center.translate(-385.1f, 0.0f, -200.0f), max, min));
        ckAssert(!scr->canBoundClip(center.translate(-384.9f, 0.0f, -200.0f), max, min));
        ckAssert(scr->canBoundClip(center.translate(0.0f, 250.1f, -200.0f), max, min));
        ckAssert(!scr->canBoundClip(center.translate(0.0f, 249.9f, -200.0f), max, min));
        ckAssert(scr->canBoundClip(center.translate(0.0f, -280.1f, -200.0f), max, min));
        ckAssert(!scr->canBoundClip(center.translate(0.0f, -279.9f, -200.0f), max, min));
        ckAssert(scr->canBoundClip(center.translate(0.0f, 0.0f, -69.9f), max, min));
        ckAssert(!scr->canBoundClip(center.translate(0.0f, 0.0f, -70.1f), max, min));
        ckAssert(scr->canBoundClip(center.translate(0.0f, 0.0f, -360.1f), max, min));
        ckAssert(!scr->canBoundClip(center.translate(0.0f, 0.0f, -359.9f), max, min));

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        ckVec calcVisibleVector(const ckVec& pos1, const ckVec& pos2)
    */
    {
        ckDrawMgr::createAfterRes();

        ckScr* scr = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        ckVec pos1, pos2;

        scr->view() = ckMat::UNIT;

        pos1.set(100.0f, 0.0f, 300.0f);
        pos2.set(200.0f, 0.0f, 400.0f);
        ckAssert(isEqual(scr->calcVisibleVector(pos1, pos2), -ckVec::Y_UNIT));

        pos1.set(0.0f, 100.0f, 300.0f);
        pos2.set(0.0f, 200.0f, 400.0f);
        ckAssert(isEqual(scr->calcVisibleVector(pos1, pos2), ckVec::X_UNIT));

        pos1 = pos2 = ckVec::ZERO;
        ckAssert(isEqual(scr->calcVisibleVector(pos1, pos2), ckVec::X_UNIT));

        scr->view() = ckMat::UNIT.rotateY_s32(90);

        pos1.set(100.0f, 0.0f, 300.0f);
        pos2.set(200.0f, 0.0f, 400.0f);
        ckAssert(isEqual(scr->calcVisibleVector(pos1, pos2), -ckVec::Y_UNIT));

        pos1.set(100.0f, -300.0f, 0.0f);
        pos2.set(200.0f, -400.0f, 0.0f);
        ckAssert(isEqual(scr->calcVisibleVector(pos1, pos2), -ckVec::Z_UNIT));

        pos1 = pos2 = ckVec::ZERO;
        ckAssert(isEqual(scr->calcVisibleVector(pos1, pos2), -ckVec::Z_UNIT));

        scr->view() = ckMat::UNIT.rotateX_s32(90);

        pos1.set(100.0f, -300.0f, 0.0f);
        pos2.set(200.0f, -400.0f, 0.0f);
        ckAssert(isEqual(scr->calcVisibleVector(pos1, pos2), -ckVec::Z_UNIT));

        pos1.set(0.0f, 100.0f, 300.0f);
        pos2.set(0.0f, 200.0f, 400.0f);
        ckAssert(isEqual(scr->calcVisibleVector(pos1, pos2), ckVec::X_UNIT));

        pos1 = pos2 = ckVec::ZERO;
        ckAssert(isEqual(scr->calcVisibleVector(pos1, pos2), ckVec::X_UNIT));

        ckDrawMgr::destroyBeforeRes();
    }
}
