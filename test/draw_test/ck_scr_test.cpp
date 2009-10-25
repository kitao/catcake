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


void pgScrTest()
{
    /*
        static const u32 DEFAULT_FOCUS_DIST
        static const u32 DEFAULT_NEAR_CLIP_DIST
        static const u32 DEFAULT_FAR_CLIP_DIST
    */
    {
        pgAssert(pgScr::DEFAULT_FOCUS_DIST == 1000);
        pgAssert(pgScr::DEFAULT_NEAR_CLIP_DIST == 10);
        pgAssert(pgScr::DEFAULT_FAR_CLIP_DIST == 100000);
    }

    /*
        pgScr* getPrevN() const
        pgScr* getNextN() const
        void moveFirst()
        void moveLast()
        void moveBefore(pgID id)
        void moveAfter(pgID id)
        pgID getID() const
    */
    {
        pgDrawMgr::createAfterRes();

        pgID id1 = pgDrawMgr::INVISIBLE_SCREEN_ID;
        pgID id2 = pgDrawMgr::DEFAULT_3D_SCREEN_ID;
        pgID id3 = pgDrawMgr::DEFAULT_2D_SCREEN_ID;

        pgScr* scr1 = pgDrawMgr::getScreen(id1);
        pgScr* scr2 = pgDrawMgr::getScreen(id2);
        pgScr* scr3 = pgDrawMgr::getScreen(id3);

        pgAssert(scr1->getID() == id1);
        pgAssert(scr2->getID() == id2);
        pgAssert(scr3->getID() == id3);

        pgAssert(pgDrawMgr::getFirstScreenN() == scr1 && pgDrawMgr::getLastScreenN() == scr3);
        pgAssert(!scr1->getPrevN() && scr1->getNextN() == scr2);
        pgAssert(scr2->getPrevN() == scr1 && scr2->getNextN() == scr3);
        pgAssert(scr3->getPrevN() == scr2 && !scr3->getNextN());

        scr2->moveFirst();

        pgAssert(pgDrawMgr::getFirstScreenN() == scr2 && pgDrawMgr::getLastScreenN() == scr3);
        pgAssert(!scr2->getPrevN() && scr2->getNextN() == scr1);
        pgAssert(scr1->getPrevN() == scr2 && scr1->getNextN() == scr3);
        pgAssert(scr3->getPrevN() == scr1 && !scr3->getNextN());

        scr1->moveLast();

        pgAssert(pgDrawMgr::getFirstScreenN() == scr2 && pgDrawMgr::getLastScreenN() == scr1);
        pgAssert(!scr2->getPrevN() && scr2->getNextN() == scr3);
        pgAssert(scr3->getPrevN() == scr2 && scr3->getNextN() == scr1);
        pgAssert(scr1->getPrevN() == scr3 && !scr1->getNextN());

        scr3->moveBefore(id2);

        pgAssert(pgDrawMgr::getFirstScreenN() == scr3 && pgDrawMgr::getLastScreenN() == scr1);
        pgAssert(!scr3->getPrevN() && scr3->getNextN() == scr2);
        pgAssert(scr2->getPrevN() == scr3 && scr2->getNextN() == scr1);
        pgAssert(scr1->getPrevN() == scr2 && !scr1->getNextN());

        scr2->moveAfter(id1);

        pgAssert(pgDrawMgr::getFirstScreenN() == scr3 && pgDrawMgr::getLastScreenN() == scr2);
        pgAssert(!scr3->getPrevN() && scr3->getNextN() == scr1);
        pgAssert(scr1->getPrevN() == scr3 && scr1->getNextN() == scr2);
        pgAssert(scr2->getPrevN() == scr1 && !scr2->getNextN());

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        pgDraw* getFirstDrawN()
        pgDraw* getLastDrawN()
    */
    {
        pgDrawMgr::createAfterRes();

        pgScr* scr = pgDrawMgr::getScreen(pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        pgAssert(!scr->getFirstDrawN() && !scr->getLastDrawN());

        pgNode node2, node3;

        {
            pgNode node1;

            node1.init(scr->getID());
            pgAssert(scr->getFirstDrawN() == &node1 && scr->getLastDrawN() == &node1);

            node2.init(scr->getID());
            pgAssert(scr->getFirstDrawN() == &node1 && scr->getLastDrawN() == &node2);

            node3.init(&node2);
            pgAssert(scr->getFirstDrawN() == &node1 && scr->getLastDrawN() == &node3);
        }

        pgAssert(scr->getFirstDrawN() == &node2 && scr->getLastDrawN() == &node3);

        pgDrawMgr::destroyBeforeRes();
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
        pgCol getClearColor() const
        void setClearColor(pgCol col)
        bool isPerspective() const
        void setPerspective(bool is_perspective)
        r32 getFocusDist() const
        void setFocusDist(r32 focus_dist)
        r32 getNearClipDist() const
        r32 getFarClipDist() const
        void setClipDist(r32 near, r32 far)
        pgMat& view()
    */
    {
        pgDrawMgr::createAfterRes();

        pgScr* scr = pgDrawMgr::newScreen(pgID::genID());

        pgAssert(scr->getLeftInFramebuffer() == 0 && scr->getTopInFramebuffer() == 0 && //
            scr->getWidthInFramebuffer() == pgSysMgr::getFramebufferWidth() && //
            scr->getHeightInFramebuffer() == pgSysMgr::getFramebufferHeight());
        pgAssert(isEqual(scr->getViewWidth(), pgSysMgr::getFramebufferWidth()) && //
            isEqual(scr->getViewHeight(), pgSysMgr::getFramebufferHeight()));
        pgAssert(scr->isActive());
        pgAssert(scr->isClearColor() && scr->isClearDepth());
        pgAssert(scr->getClearColor() == pgCol(255, 0, 0, 0));
        pgAssert(scr->isPerspective());
        pgAssert(isEqual(scr->getFocusDist(), static_cast<r32>(pgScr::DEFAULT_FOCUS_DIST)));
        pgAssert(isEqual(scr->getNearClipDist(), static_cast<r32>(pgScr::DEFAULT_NEAR_CLIP_DIST)) && //
            isEqual(scr->getFarClipDist(), static_cast<r32>(pgScr::DEFAULT_FAR_CLIP_DIST)));
        pgAssert(isEqual(scr->view(), pgMat::UNIT.translate(0.0f, 0.0f, static_cast<r32>(pgScr::DEFAULT_FOCUS_DIST))));

        scr->setAreaInFramebuffer(-10, -20, 30, 40);
        pgAssert(scr->getLeftInFramebuffer() == -10 && scr->getTopInFramebuffer() == -20 && //
            scr->getWidthInFramebuffer() == 30 && scr->getHeightInFramebuffer() == 40);

        scr->setViewSize(123.0f, 456.0f);
        pgAssert(isEqual(scr->getViewWidth(), 123.0f) && isEqual(scr->getViewHeight(), 456.0f));

        scr->setActive(false);
        pgAssert(!scr->isActive());

        scr->setClearMode(false, true);
        pgAssert(!scr->isClearColor() && scr->isClearDepth());

        scr->setClearMode(true, false);
        pgAssert(scr->isClearColor() && !scr->isClearDepth());

        scr->setClearColor(pgCol::FULL);
        pgAssert(scr->getClearColor() == pgCol::FULL);

        scr->setPerspective(false);
        pgAssert(!scr->isPerspective());

        scr->setFocusDist(1234.0f);
        pgAssert(isEqual(scr->getFocusDist(), 1234.0f));

        scr->setClipDist(123.0f, 456.0f);
        pgAssert(isEqual(scr->getNearClipDist(), 123.0f) && isEqual(scr->getFarClipDist(), 456.0f));

        scr->view() = pgMat::ZERO;
        pgAssert(isEqual(scr->view(), pgMat::ZERO));

        pgDrawMgr::renderForSystem();

        pgAssertThrow(scr->setAreaInFramebuffer(123, 456, 0, 456), pgScr::ExceptionInvalidArgument);
        pgAssertThrow(scr->setAreaInFramebuffer(123, 456, 123, 0), pgScr::ExceptionInvalidArgument);
        pgAssertThrow(scr->setFocusDist(0.0f), pgScr::ExceptionInvalidArgument);
        pgAssertThrow(scr->setClipDist(0.0f, 123.0f), pgScr::ExceptionInvalidArgument);
        pgAssertThrow(scr->setClipDist(123.0f, 123.0f), pgScr::ExceptionInvalidArgument);

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        bool hasScreenTexture() const
        pgID getScreenTextureID() const
        void attachScreenTexture(pgTex::TexFormat format)
        void detachScreenTexture()
        void updateScreenTexture(bool is_frame_skip_reset)
    */
    {
        pgDrawMgr::createAfterRes();

        pgScr* scr = pgDrawMgr::newScreen(pgID::genID());
        pgAssert(!scr->hasScreenTexture());

        pgAssertThrow(scr->getScreenTextureID(), pgScr::ExceptionInvalidCall);

        u32 used_memory_size = pgMemMgr::getCurUsedMemorySize();

        pgAssertThrow(scr->attachScreenTexture(pgTex::FORMAT_ALPHA), pgScr::ExceptionInvalidArgument);

        scr->setAreaInFramebuffer(0, 0, pgDrawMgr::getMaxTextureLength() + 1, 10);
        pgAssertThrow(scr->attachScreenTexture(pgTex::FORMAT_RGB), pgScr::ExceptionInvalidCall);

        scr->setAreaInFramebuffer(0, 0, 10, pgDrawMgr::getMaxTextureLength() + 1);
        pgAssertThrow(scr->attachScreenTexture(pgTex::FORMAT_RGB), pgScr::ExceptionInvalidCall);

        scr->setAreaInFramebuffer(10, 20, 30, 40);
        scr->attachScreenTexture(pgTex::FORMAT_RGB);
        pgAssert(scr->hasScreenTexture());

        scr->attachScreenTexture(pgTex::FORMAT_RGBA);

        pgID tex_id = scr->getScreenTextureID();
        pgTex* tex = pgDrawMgr::getTexture(tex_id);

        pgAssert(tex->getID() == tex_id);
        pgAssert(tex->getWidth() == scr->getWidthInFramebuffer() && tex->getHeight() == scr->getHeightInFramebuffer());
        pgAssert(tex->getFormat() == pgTex::FORMAT_RGBA);
        pgAssert(tex->getMode() == pgTex::MODE_FRAMEBUFFER);

        pgAssert(pgMemMgr::getCurUsedMemorySize() > used_memory_size);

        pgDrawMgr::renderForSystem();

        scr->updateScreenTexture(true);
        scr->updateScreenTexture(false);

        pgDrawMgr::renderForSystem();

        scr->detachScreenTexture();
        pgAssertThrow(pgDrawMgr::getTexture(tex_id), pgDrawMgr::ExceptionNotFound);
        pgAssertThrow(scr->updateScreenTexture(false), pgScr::ExceptionInvalidCall);

        scr->detachScreenTexture();

        pgAssert(pgMemMgr::getCurUsedMemorySize() == used_memory_size);

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        pgID getGuestScreenID(u8 index) const
        void setGuestScreenID(u8 index, pgID scr_id)
    */
    {
        pgDrawMgr::createAfterRes();

        pgScr* scr = pgDrawMgr::newScreen(pgID::genID());
        pgAssert(scr->getGuestScreenID(0) == pgID::ZERO && scr->getGuestScreenID(1) == pgID::ZERO && //
            scr->getGuestScreenID(2) == pgID::ZERO && scr->getGuestScreenID(3) == pgID::ZERO);

        scr->setGuestScreenID(0, pgID_("test1"));
        scr->setGuestScreenID(1, pgID_("test2"));
        scr->setGuestScreenID(2, pgID_("test3"));
        scr->setGuestScreenID(3, pgID_("test4"));
        pgAssert(scr->getGuestScreenID(0) == pgID_("test1") && scr->getGuestScreenID(1) == pgID_("test2") && //
            scr->getGuestScreenID(2) == pgID_("test3") && scr->getGuestScreenID(3) == pgID_("test4"));

        pgAssertThrow(scr->getGuestScreenID(4), pgScr::ExceptionInvalidArgument);

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        r32 screenXToFramebufferX(r32 x_in_screen) const
        r32 screenYToFramebufferY(r32 y_in_screen) const
        r32 framebufferXToScreenX(r32 x_in_framebuffer) const
        r32 framebufferYToScreenY(r32 y_in_framebuffer) const
        pgVec worldToScreen(const pgVec& pos_in_world)
        pgVec worldToFramebuffer(const pgVec& pos_in_world)
        pgVec screenToPlane(r32 x_in_screen, r32 y_in_screen, const pgMat& xy_plane) const
        pgVec framebufferToPlane(r32 x_in_framebuffer, r32 y_in_framebuffer, const pgMat& xy_plane) const
        bool isInScreen(const pgVec& pos_in_world)
        bool canBoundClip(const pgMat& world, const pgVec& bound_max, const pgVec& bound_min)
    */
    {
        pgDrawMgr::createAfterRes();

        pgScr* scr = pgDrawMgr::getScreen(pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        scr->setAreaInFramebuffer(-100, -200, 400, 300);
        scr->setViewSize(600.0f, 400.0f);
        scr->setFocusDist(200.0f);
        scr->setClipDist(100.0f, 300.0f);
        scr->view() = pgMat::UNIT.translate(10.0f, 20.0f, 30.0f).rotateY_s32(90);

        pgAssert(isEqual(scr->screenXToFramebufferX(0.0f), 100.0f));
        pgAssert(isEqual(scr->screenXToFramebufferX(-300.0f), -100.0f));
        pgAssert(isEqual(scr->screenXToFramebufferX(300.0f), 300.0f));

        pgAssert(isEqual(scr->screenYToFramebufferY(0.0f), -50.0f));
        pgAssert(isEqual(scr->screenYToFramebufferY(200.0f), -200.0f));
        pgAssert(isEqual(scr->screenYToFramebufferY(-200.0f), 100.0f));

        pgAssert(isEqual(scr->framebufferXToScreenX(100.0f), 0.0f));
        pgAssert(isEqual(scr->framebufferXToScreenX(-100.0f), -300.0f));
        pgAssert(isEqual(scr->framebufferXToScreenX(300.0f), 300.0f));

        pgAssert(isEqual(scr->framebufferYToScreenY(-50.0f), 0.0f));
        pgAssert(isEqual(scr->framebufferYToScreenY(-200.0f), 200.0f));
        pgAssert(isEqual(scr->framebufferYToScreenY(100.0f), -200.0f));

        pgAssert(isEqual(scr->worldToScreen(pgVec(-90.0f, 20.0f, 30.0f)), pgVec(0.0, 0.0f, 1.0f)));
        pgAssert(isEqual(scr->worldToScreen(pgVec(-290.0f, 20.0f, 30.0f)), pgVec(0.0f, 0.0f, -1.0f)));
        pgAssert(isEqual(scr->worldToScreen(pgVec(-190.0f, 220.0f, -270.0f)), pgVec(300.0f, 200.0f, -0.5f)));
        pgAssert(isEqual(scr->worldToScreen(pgVec(-190.0f, -180.0f, 330.0f)), pgVec(-300.0f, -200.0f, -0.5f)));

        pgAssert(isEqual(scr->worldToScreen(pgVec(-90.0f, 20.0f, 30.0f)), pgVec(0.0, 0.0f, 1.0f)));
        pgAssert(isEqual(scr->worldToScreen(pgVec(-290.0f, 20.0f, 30.0f)), pgVec(0.0f, 0.0f, -1.0f)));
        pgAssert(isEqual(scr->worldToScreen(pgVec(-190.0f, 220.0f, -270.0f)), pgVec(300.0f, 200.0f, -0.5f)));
        pgAssert(isEqual(scr->worldToScreen(pgVec(-190.0f, -180.0f, 330.0f)), pgVec(-300.0f, -200.0f, -0.5f)));

        pgAssert(isEqual(scr->worldToFramebuffer(pgVec(-90.0f, 20.0f, 30.0f)), pgVec(100.0f, -50.0f, 1.0f)));
        pgAssert(isEqual(scr->worldToFramebuffer(pgVec(-290.0f, 20.0f, 30.0f)), pgVec(100.0f, -50.0f, -1.0f)));
        pgAssert(isEqual(scr->worldToFramebuffer(pgVec(-190.0f, 220.0f, -270.0f)), pgVec(300.0f, -200.0f, -0.5f)));
        pgAssert(isEqual(scr->worldToFramebuffer(pgVec(-190.0f, -180.0f, 330.0f)), pgVec(-100.0f, 100.0f, -0.5f)));

        pgMat plane = pgMat::UNIT.translate(-200.0f, 30.0f, 40.0f).rotateX_s32(10).rotateY_s32(10).rotateZ_s32(10);

        pgVec vec1 = scr->screenToPlane(12.0f, 34.0f, plane);
        pgAssert(isEqual(vec1.z, 0.0f));
        vec1 = scr->worldToScreen(vec1.toGlobalFrom(plane));
        pgAssert(isEqual(vec1.x, 12.0f) && isEqual(vec1.y, 34.0f));

        pgVec vec2 = scr->framebufferToPlane(123.0f, 456.0f, plane);
        pgAssert(isEqual(vec2.z, 0.0f));
        vec2 = scr->worldToFramebuffer(vec2.toGlobalFrom(plane));
        pgAssert(isEqual(vec2.x, 123.0f) && isEqual(vec2.y, 456.0f));

        pgAssert(scr->isInScreen(pgVec(-90.0f, 20.0f, 30.0f)));
        pgAssert(!scr->isInScreen(pgVec(-89.9f, 20.0f, 30.0f)));
        pgAssert(scr->isInScreen(pgVec(-290.0f, 20.0f, 30.0f)));
        pgAssert(!scr->isInScreen(pgVec(-290.1f, 20.0f, 30.0f)));
        pgAssert(scr->isInScreen(pgVec(-190.0f, 220.0f, -270.0f)));
        pgAssert(!scr->isInScreen(pgVec(-190.0f, 220.0f, -270.1f)));
        pgAssert(!scr->isInScreen(pgVec(-190.0f, 220.1f, -270.0f)));
        pgAssert(scr->isInScreen(pgVec(-190.0f, -180.0f, 330.0f)));
        pgAssert(!scr->isInScreen(pgVec(-190.0f, -180.0f, 330.1f)));
        pgAssert(!scr->isInScreen(pgVec(-190.0f, -180.1f, 330.0f)));

        pgVec min(-10.0f, -20.0f, -30.0f);
        pgVec max(40.0f, 50.0f, 60.0f);
        pgMat center = pgMat::UNIT.toGlobalFrom(scr->view());

        pgAssert(scr->canBoundClip(center.translate(355.1f, 0.0f, -200.0f), max, min));
        pgAssert(!scr->canBoundClip(center.translate(354.9f, 0.0f, -200.0f), max, min));
        pgAssert(scr->canBoundClip(center.translate(-385.1f, 0.0f, -200.0f), max, min));
        pgAssert(!scr->canBoundClip(center.translate(-384.9f, 0.0f, -200.0f), max, min));
        pgAssert(scr->canBoundClip(center.translate(0.0f, 250.1f, -200.0f), max, min));
        pgAssert(!scr->canBoundClip(center.translate(0.0f, 249.9f, -200.0f), max, min));
        pgAssert(scr->canBoundClip(center.translate(0.0f, -280.1f, -200.0f), max, min));
        pgAssert(!scr->canBoundClip(center.translate(0.0f, -279.9f, -200.0f), max, min));
        pgAssert(scr->canBoundClip(center.translate(0.0f, 0.0f, -69.9f), max, min));
        pgAssert(!scr->canBoundClip(center.translate(0.0f, 0.0f, -70.1f), max, min));
        pgAssert(scr->canBoundClip(center.translate(0.0f, 0.0f, -360.1f), max, min));
        pgAssert(!scr->canBoundClip(center.translate(0.0f, 0.0f, -359.9f), max, min));

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        pgVec calcVisibleVector(const pgVec& pos1, const pgVec& pos2)
    */
    {
        pgDrawMgr::createAfterRes();

        pgScr* scr = pgDrawMgr::getScreen(pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        pgVec pos1, pos2;

        scr->view() = pgMat::UNIT;

        pos1.set(100.0f, 0.0f, 300.0f);
        pos2.set(200.0f, 0.0f, 400.0f);
        pgAssert(isEqual(scr->calcVisibleVector(pos1, pos2), -pgVec::Y_UNIT));

        pos1.set(0.0f, 100.0f, 300.0f);
        pos2.set(0.0f, 200.0f, 400.0f);
        pgAssert(isEqual(scr->calcVisibleVector(pos1, pos2), pgVec::X_UNIT));

        pos1 = pos2 = pgVec::ZERO;
        pgAssert(isEqual(scr->calcVisibleVector(pos1, pos2), pgVec::X_UNIT));

        scr->view() = pgMat::UNIT.rotateY_s32(90);

        pos1.set(100.0f, 0.0f, 300.0f);
        pos2.set(200.0f, 0.0f, 400.0f);
        pgAssert(isEqual(scr->calcVisibleVector(pos1, pos2), -pgVec::Y_UNIT));

        pos1.set(100.0f, -300.0f, 0.0f);
        pos2.set(200.0f, -400.0f, 0.0f);
        pgAssert(isEqual(scr->calcVisibleVector(pos1, pos2), -pgVec::Z_UNIT));

        pos1 = pos2 = pgVec::ZERO;
        pgAssert(isEqual(scr->calcVisibleVector(pos1, pos2), -pgVec::Z_UNIT));

        scr->view() = pgMat::UNIT.rotateX_s32(90);

        pos1.set(100.0f, -300.0f, 0.0f);
        pos2.set(200.0f, -400.0f, 0.0f);
        pgAssert(isEqual(scr->calcVisibleVector(pos1, pos2), -pgVec::Z_UNIT));

        pos1.set(0.0f, 100.0f, 300.0f);
        pos2.set(0.0f, 200.0f, 400.0f);
        pgAssert(isEqual(scr->calcVisibleVector(pos1, pos2), pgVec::X_UNIT));

        pos1 = pos2 = pgVec::ZERO;
        pgAssert(isEqual(scr->calcVisibleVector(pos1, pos2), pgVec::X_UNIT));

        pgDrawMgr::destroyBeforeRes();
    }
}
