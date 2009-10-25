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


void pgDrawMgrTest()
{
    /*
        static const pgID INVISIBLE_SCREEN_ID
        static const pgID DEFAULT_3D_SCREEN_ID
        static const pgID DEFAULT_2D_SCREEN_ID
    */
    {
        pgAssert(pgDrawMgr::INVISIBLE_SCREEN_ID == pgID_("INVISIBLE_SCREEN"));
        pgAssert(pgDrawMgr::DEFAULT_3D_SCREEN_ID == pgID_("DEFAULT_3D_SCREEN"));
        pgAssert(pgDrawMgr::DEFAULT_2D_SCREEN_ID == pgID_("DEFAULT_2D_SCREEN"));

        pgDrawMgr::createAfterRes();

        pgScr* scr0d = pgDrawMgr::getScreen(pgDrawMgr::INVISIBLE_SCREEN_ID);

        pgAssert(scr0d->getID() == pgDrawMgr::INVISIBLE_SCREEN_ID);
        pgAssert(scr0d->getLeftInFramebuffer() == 0 && scr0d->getTopInFramebuffer() == 0 && //
            scr0d->getWidthInFramebuffer() == pgSysMgr::getFramebufferWidth() && //
            scr0d->getHeightInFramebuffer() == pgSysMgr::getFramebufferHeight());
        pgAssert(isEqual(scr0d->getViewWidth(), pgSysMgr::getFramebufferWidth()) && //
            isEqual(scr0d->getViewHeight(), pgSysMgr::getFramebufferHeight()));
        pgAssert(!scr0d->isActive());
        pgAssert(scr0d->isClearColor() && scr0d->isClearDepth());
        pgAssert(scr0d->getClearColor() == pgCol(255, 0, 0, 0));
        pgAssert(scr0d->isPerspective());
        pgAssert(isEqual(scr0d->getFocusDist(), static_cast<r32>(pgScr::DEFAULT_FOCUS_DIST)));
        pgAssert(isEqual(scr0d->getNearClipDist(), static_cast<r32>(pgScr::DEFAULT_NEAR_CLIP_DIST)) && //
            isEqual(scr0d->getFarClipDist(), static_cast<r32>(pgScr::DEFAULT_FAR_CLIP_DIST)));
        pgAssert(isEqual(scr0d->view(), pgMat::UNIT.translate(0, 0, static_cast<r32>(pgScr::DEFAULT_FOCUS_DIST))));
        pgAssert(!scr0d->hasScreenTexture());
        pgAssert(scr0d->getGuestScreenID(0) == pgID::ZERO && scr0d->getGuestScreenID(1) == pgID::ZERO && //
            scr0d->getGuestScreenID(2) == pgID::ZERO && scr0d->getGuestScreenID(3) == pgID::ZERO);

        pgScr* scr3d = pgDrawMgr::getScreen(pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        pgAssert(scr3d->getID() == pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        pgAssert(scr3d->getLeftInFramebuffer() == 0 && scr3d->getTopInFramebuffer() == 0 && //
            scr3d->getWidthInFramebuffer() == pgSysMgr::getFramebufferWidth() && //
            scr3d->getHeightInFramebuffer() == pgSysMgr::getFramebufferHeight());
        pgAssert(isEqual(scr3d->getViewWidth(), pgSysMgr::getFramebufferWidth()) && //
            isEqual(scr3d->getViewHeight(), pgSysMgr::getFramebufferHeight()));
        pgAssert(scr3d->isActive());
        pgAssert(scr3d->isClearColor() && scr3d->isClearDepth());
        pgAssert(scr3d->getClearColor() == pgCol::ZERO);
        pgAssert(scr3d->isPerspective());
        pgAssert(isEqual(scr3d->getFocusDist(), static_cast<r32>(pgScr::DEFAULT_FOCUS_DIST)));
        pgAssert(isEqual(scr3d->getNearClipDist(), static_cast<r32>(pgScr::DEFAULT_NEAR_CLIP_DIST)) && //
            isEqual(scr3d->getFarClipDist(), static_cast<r32>(pgScr::DEFAULT_FAR_CLIP_DIST)));
        pgAssert(isEqual(scr3d->view(), pgMat::UNIT.translate(0, 0, static_cast<r32>(pgScr::DEFAULT_FOCUS_DIST))));
        pgAssert(!scr3d->hasScreenTexture());
        pgAssert(scr3d->getGuestScreenID(0) == pgID::ZERO && scr3d->getGuestScreenID(1) == pgID::ZERO && //
            scr3d->getGuestScreenID(2) == pgID::ZERO && scr3d->getGuestScreenID(3) == pgID::ZERO);

        pgScr* scr2d = pgDrawMgr::getScreen(pgDrawMgr::DEFAULT_2D_SCREEN_ID);

        pgAssert(scr2d->getID() == pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        pgAssert(scr2d->getLeftInFramebuffer() == 0 && scr2d->getTopInFramebuffer() == 0 && //
            scr2d->getWidthInFramebuffer() == pgSysMgr::getFramebufferWidth() && //
            scr2d->getHeightInFramebuffer() == pgSysMgr::getFramebufferHeight());
        pgAssert(isEqual(scr2d->getViewWidth(), pgSysMgr::getFramebufferWidth()) && //
            isEqual(scr2d->getViewHeight(), pgSysMgr::getFramebufferHeight()));
        pgAssert(scr2d->isActive());
        pgAssert(!scr2d->isClearColor() && scr2d->isClearDepth());
        pgAssert(scr2d->getClearColor() == pgCol(255, 0, 0, 0));
        pgAssert(!scr2d->isPerspective());
        pgAssert(isEqual(scr2d->getFocusDist(), static_cast<r32>(pgScr::DEFAULT_FOCUS_DIST)));
        pgAssert(isEqual(scr2d->getNearClipDist(), static_cast<r32>(pgScr::DEFAULT_NEAR_CLIP_DIST)) && //
            isEqual(scr2d->getFarClipDist(), static_cast<r32>(pgScr::DEFAULT_FAR_CLIP_DIST)));
        pgAssert(isEqual(scr2d->view(), pgMat::UNIT.translate(0, 0, static_cast<r32>(pgScr::DEFAULT_FOCUS_DIST))));
        pgAssert(!scr3d->hasScreenTexture());
        pgAssert(scr2d->getGuestScreenID(0) == pgID::ZERO && scr2d->getGuestScreenID(1) == pgID::ZERO && //
            scr2d->getGuestScreenID(2) == pgID::ZERO && scr2d->getGuestScreenID(3) == pgID::ZERO);

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        static const pgID DEFAULT_LIGHT_SET_ID
    */
    {
        pgAssert(pgDrawMgr::DEFAULT_LIGHT_SET_ID == pgID_("DEFAULT_LIGHT_SET"));

        pgDrawMgr::createAfterRes();

        pgLts* lts = pgDrawMgr::getLightSet(pgDrawMgr::DEFAULT_LIGHT_SET_ID);

        pgAssert(lts->getID() == pgDrawMgr::DEFAULT_LIGHT_SET_ID);
        pgAssert(lts->getAmbientColor() == pgCol::ZERO);
        pgAssert(!lts->isParaLightActive(0) && !lts->isParaLightActive(1) && !lts->isParaLightActive(2));
        pgAssert(lts->getPointLightNum() == 0);

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        static const pgID DEFAULT_SHADER_ID
    */
    {
        pgAssert(pgDrawMgr::DEFAULT_SHADER_ID == pgID_("DEFAULT_SHADER"));

        pgDrawMgr::createAfterRes();

        pgShd* shd = pgDrawMgr::getShader(pgDrawMgr::DEFAULT_SHADER_ID);

        pgAssert(shd->getID() == pgDrawMgr::DEFAULT_SHADER_ID);
        pgAssert(shd->getUniformNum() == 9);
        pgAssert(shd->getAttribNum() == 0);
        pgAssert(shd->getTextureNum() == 1);
        pgAssert(pgDrawMgr::isShaderAvailable() == shd->isValid());

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        static const u32 MAX_RENDER_INTERVAL_MSEC_TIME
    */
    {
        pgAssert(pgDrawMgr::MAX_RENDER_INTERVAL_MSEC_TIME == 100);
    }

    /*
        static bool isShaderAvailable()
    */
    {
        pgAssert(pgDrawMgr::isShaderAvailable() == pgDrawMgr::isShaderAvailable());
    }

    /*
        static u16 getMaxTextureLength()
    */
    {
        pgAssert(pgDrawMgr::getMaxTextureLength() == 2048);
    }

    /*
        static u16 getValidTextureLength(u16 length)
    */
    {
        pgAssert(pgDrawMgr::getValidTextureLength(0) == 8);
        pgAssert(pgDrawMgr::getValidTextureLength(4) == 8);
        pgAssert(pgDrawMgr::getValidTextureLength(8) == 8);
        pgAssert(pgDrawMgr::getValidTextureLength(9) == 16);
        pgAssert(pgDrawMgr::getValidTextureLength(32) == 32);
        pgAssert(pgDrawMgr::getValidTextureLength(33) == 64);
        pgAssert(pgDrawMgr::getValidTextureLength(64) == 64);
        pgAssert(pgDrawMgr::getValidTextureLength(65) == 128);
        pgAssert(pgDrawMgr::getValidTextureLength(128) == 128);
        pgAssert(pgDrawMgr::getValidTextureLength(129) == 256);
        pgAssert(pgDrawMgr::getValidTextureLength(256) == 256);
        pgAssert(pgDrawMgr::getValidTextureLength(257) == 512);
        pgAssert(pgDrawMgr::getValidTextureLength(512) == 512);
        pgAssert(pgDrawMgr::getValidTextureLength(513) == 1024);
        pgAssert(pgDrawMgr::getValidTextureLength(1024) == 1024);
        pgAssert(pgDrawMgr::getValidTextureLength(1025) == 2048);
        pgAssert(pgDrawMgr::getValidTextureLength(2048) == 2048);
        pgAssert(pgDrawMgr::getValidTextureLength(2049) == 2048);
        pgAssert(pgDrawMgr::getValidTextureLength(10000) == 2048);
    }

    /*
        static u16 getTexturePixelSize(pgTex::TexFormat format)
    */
    {
        pgAssert(pgDrawMgr::getTexturePixelSize(pgTex::FORMAT_RGB) == 3);
        pgAssert(pgDrawMgr::getTexturePixelSize(pgTex::FORMAT_RGBA) == 4);
        pgAssert(pgDrawMgr::getTexturePixelSize(pgTex::FORMAT_ALPHA) == 1);
        pgAssert(pgDrawMgr::getTexturePixelSize(pgTex::FORMAT_PNG_RGB) == 3);
        pgAssert(pgDrawMgr::getTexturePixelSize(pgTex::FORMAT_PNG_RGBA) == 4);
        pgAssert(pgDrawMgr::getTexturePixelSize(pgTex::FORMAT_PNG_ALPHA) == 1);
    }

    /*
        static bool isCreated()
        static void createAfterRes()
        static void destroyBeforeRes()
    */
    {
        pgAssert(!pgDrawMgr::isCreated());

        pgDrawMgr::createAfterRes();
        pgDrawMgr::createAfterRes();

        pgAssert(pgDrawMgr::isCreated());

        pgDrawMgr::destroyBeforeRes();
        pgDrawMgr::destroyBeforeRes();

        pgAssert(!pgDrawMgr::isCreated());
    }

    /*
        static bool isRender()
        static void setRender(bool is_render)
        static void deleteAllVramObjForSystem()
        static bool renderForSystem()
    */
    {
        pgAssertThrow(pgDrawMgr::setRender(false), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::isRender(), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::deleteAllVramObjForSystem(), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::renderForSystem(), pgDrawMgr::ExceptionNotInitialized);

        pgDrawMgr::createAfterRes();

        pgDrawMgr::deleteAllVramObjForSystem();

        pgAssert(pgDrawMgr::isRender());

        pgDrawMgr::setRender(false);
        pgAssert(!pgDrawMgr::isRender());

        pgDrawMgr::renderForSystem();

        pgDrawMgr::deleteScreen(pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        pgDrawMgr::deleteScreen(pgDrawMgr::DEFAULT_2D_SCREEN_ID);

        pgDrawMgr::setRender(true);
        pgAssert(pgDrawMgr::isRender());

        pgDrawMgr::renderForSystem();

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        static bool hasScreen(pgID scr_id)
        static pgScr* getScreen(pgID scr_id)
        static pgScr* newScreen(pgID scr_id)
        static void deleteScreen(pgID scr_id)
        static pgScr* getFirstScreenN()
        static pgScr* getLastScreenN()
    */
    {
        pgAssertThrow(pgDrawMgr::hasScreen(pgID::ZERO), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::getScreen(pgID::ZERO), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::newScreen(pgID::ZERO), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::deleteScreen(pgID::ZERO), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::getFirstScreenN(), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::getLastScreenN(), pgDrawMgr::ExceptionNotInitialized);

        u32 used_memory_size = pgMemMgr::getCurUsedMemorySize();

        pgDrawMgr::createAfterRes();

        pgScr* scr0d = pgDrawMgr::getScreen(pgDrawMgr::INVISIBLE_SCREEN_ID);
        pgAssert(scr0d->getID() == pgDrawMgr::INVISIBLE_SCREEN_ID);
        pgAssert(pgDrawMgr::hasScreen(pgDrawMgr::INVISIBLE_SCREEN_ID));

        pgScr* scr3d = pgDrawMgr::getScreen(pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        pgAssert(scr3d->getID() == pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        pgAssert(pgDrawMgr::hasScreen(pgDrawMgr::DEFAULT_3D_SCREEN_ID));

        pgScr* scr2d = pgDrawMgr::getScreen(pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        pgAssert(scr2d->getID() == pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        pgAssert(pgDrawMgr::hasScreen(pgDrawMgr::DEFAULT_2D_SCREEN_ID));

        pgAssertThrow(pgDrawMgr::getScreen(pgID::genID()), pgDrawMgr::ExceptionNotFound);

        pgID id = pgID_("TEST_SCREEN");
        pgScr* scr = pgDrawMgr::newScreen(id);
        pgAssert(scr->getID() == id);
        pgAssert(pgDrawMgr::hasScreen(id));
        pgAssert(pgDrawMgr::getFirstScreenN() == scr0d && pgDrawMgr::getLastScreenN() == scr);

        pgAssertThrow(pgDrawMgr::newScreen(id), pgDrawMgr::ExceptionSameIDExists);

        pgNode node;
        node.init(id);

        pgDrawMgr::deleteScreen(id);
        pgAssert(!pgDrawMgr::hasScreen(id));
        pgAssert(pgDrawMgr::getFirstScreenN() == scr0d && pgDrawMgr::getLastScreenN() == scr2d);

        pgAssert(node.getScreenID() == pgDrawMgr::INVISIBLE_SCREEN_ID);

        pgAssertThrow(pgDrawMgr::deleteScreen(id), pgDrawMgr::ExceptionNotFound);

        pgDrawMgr::deleteScreen(pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        pgAssert(!pgDrawMgr::hasScreen(pgDrawMgr::DEFAULT_3D_SCREEN_ID));
        pgAssert(pgDrawMgr::getFirstScreenN() == scr0d && pgDrawMgr::getLastScreenN() == scr2d);

        pgDrawMgr::deleteScreen(pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        pgAssert(!pgDrawMgr::hasScreen(pgDrawMgr::DEFAULT_2D_SCREEN_ID));
        pgAssert(pgDrawMgr::getFirstScreenN() == scr0d && pgDrawMgr::getLastScreenN() == scr0d);

        pgAssertThrow(pgDrawMgr::hasScreen(pgID::ZERO), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::getScreen(pgID::ZERO), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newScreen(pgID::ZERO), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::deleteScreen(pgID::ZERO), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::deleteScreen(pgDrawMgr::INVISIBLE_SCREEN_ID), pgDrawMgr::ExceptionInvalidArgument);

        pgDrawMgr::newScreen(id);

        pgDrawMgr::destroyBeforeRes();

        pgAssert(pgMemMgr::getCurUsedMemorySize() == used_memory_size);
    }

    /*
        static bool hasTexture(pgID tex_id)
        static pgTex* getTexture(pgID tex_id)
        static pgTex* newTexture(pgID tex_id, u16 width, u16 height, pgTex::TexFormat format)
        static pgTex* newTexture(pgID tex_id, u16 width, u16 height, pgTex::TexFormat format, const void* image, u32 image_size)
        static void deleteTexture(pgID tex_id)
        static pgTex* getFirstTextureN()
        static pgTex* getLastTextureN()
    */
    {
        pgAssertThrow(pgDrawMgr::hasTexture(pgID::ZERO), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::getTexture(pgID::ZERO), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::ZERO, 0, 0, pgTex::FORMAT_RGB), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::ZERO, 0, 0, pgTex::FORMAT_RGB, NULL, 0), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::deleteTexture(pgID::ZERO), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::getFirstTextureN(), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::getLastTextureN(), pgDrawMgr::ExceptionNotInitialized);

        u32 used_memory_size = pgMemMgr::getCurUsedMemorySize();

        pgDrawMgr::createAfterRes();

        pgID id1 = pgID::genID();
        pgAssertThrow(pgDrawMgr::getTexture(id1), pgDrawMgr::ExceptionNotFound);

        pgAssert(!pgDrawMgr::getFirstTextureN() && !pgDrawMgr::getLastTextureN());

        pgTex* tex1 = pgDrawMgr::newTexture(id1, 10, 10, pgTex::FORMAT_RGB);
        pgAssert(tex1->getID() == id1);
        pgAssert(pgDrawMgr::hasTexture(id1));
        pgAssert(pgDrawMgr::getFirstTextureN() == tex1 && pgDrawMgr::getLastTextureN() == tex1);

        pgID id2 = pgID::genID();
        pgTex* tex2 = pgDrawMgr::newTexture(id2, 100, 100, pgTex::FORMAT_RGBA, "dummy", 40000);
        pgAssert(tex2->getID() == id2);
        pgAssert(pgDrawMgr::hasTexture(id2));
        pgAssert(pgDrawMgr::getFirstTextureN() == tex1 && pgDrawMgr::getLastTextureN() == tex2);

        pgDrawMgr::deleteTexture(id1);
        pgAssert(!pgDrawMgr::hasTexture(id1));
        pgAssert(pgDrawMgr::getFirstTextureN() == tex2 && pgDrawMgr::getLastTextureN() == tex2);

        pgAssertThrow(pgDrawMgr::deleteTexture(id1), pgDrawMgr::ExceptionNotFound);

        pgDrawMgr::deleteTexture(id2);
        pgAssert(!pgDrawMgr::hasTexture(id2));
        pgAssert(!pgDrawMgr::getFirstTextureN() && !pgDrawMgr::getLastTextureN());

        pgDrawMgr::newTexture(id1, 10, 20, pgTex::FORMAT_ALPHA);
        pgDrawMgr::newTexture(id2, 30, 40, pgTex::FORMAT_RGB, "dummy", 3600);

        pgAssertThrow(pgDrawMgr::hasTexture(pgID::ZERO), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::getTexture(pgID::ZERO), pgDrawMgr::ExceptionInvalidArgument);

        pgAssertThrow(pgDrawMgr::newTexture(pgID::ZERO, 100, 100, pgTex::FORMAT_RGB), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::genID(), 0, 100, pgTex::FORMAT_RGB), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::genID(), 100, 0, pgTex::FORMAT_RGB), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::genID(), 2049, 100, pgTex::FORMAT_RGB), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::genID(), 100, 2049, pgTex::FORMAT_RGB), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::genID(), 100, 100, pgTex::FORMAT_PNG_RGB), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::genID(), 100, 100, pgTex::FORMAT_PNG_RGBA), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::genID(), 100, 100, pgTex::FORMAT_PNG_ALPHA), pgDrawMgr::ExceptionInvalidArgument);

        pgAssertThrow(pgDrawMgr::newTexture(pgID::ZERO, 100, 100, pgTex::FORMAT_RGB, "dummy", 30000), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::genID(), 0, 100, pgTex::FORMAT_RGB, "dummy", 3000), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::genID(), 100, 0, pgTex::FORMAT_RGB, "dummy", 3000), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::genID(), 2049, 100, pgTex::FORMAT_RGB, "dummy", 3000), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::genID(), 100, 2049, pgTex::FORMAT_RGB, "dummy", 3000), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::genID(), 100, 100, pgTex::FORMAT_PNG_RGB, "dummy", 3000), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::genID(), 100, 100, pgTex::FORMAT_PNG_RGBA, "dummy", 3000), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::genID(), 100, 100, pgTex::FORMAT_PNG_ALPHA, "dummy", 3000), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::genID(), 100, 100, pgTex::FORMAT_RGB, NULL, 3000), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newTexture(pgID::genID(), 100, 100, pgTex::FORMAT_RGB, "dummy", 1), pgDrawMgr::ExceptionInvalidArgument);

        pgAssertThrow(pgDrawMgr::deleteTexture(pgID::ZERO), pgDrawMgr::ExceptionInvalidArgument);

        pgDrawMgr::destroyBeforeRes();

        pgAssert(pgMemMgr::getCurUsedMemorySize() == used_memory_size);
    }

    /*
        static u16 getFontIndexNum(pgID font_id)
        static pgID getFontID()
        static u16 getFontIndex()
        static void setFont(pgID font_id, u16 font_index)
        static u16 getFontSize()
        static u16 setFontSize(u16 font_size)
        static u16 calcFontDrawWidth(const char* str, ...)
        static u16 calcFontDrawWidth(const wchar_t* str, ...)
    */
    {
        pgAssertThrow(pgDrawMgr::getFontIndexNum(pgID::ZERO), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::getFontID(), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::getFontIndex(), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::setFont(pgID::ZERO, 0), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::getFontSize(), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::setFontSize(0), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::calcFontDrawWidth(static_cast<const char*>(NULL)), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::calcFontDrawWidth(static_cast<const wchar_t*>(NULL)), pgDrawMgr::ExceptionNotInitialized);

        pgDrawMgr::createAfterRes();

        pgResMgr::loadResource(TEST_DATA_DIR "adler.ttf", true);
        pgResMgr::loadResource(TEST_DATA_DIR "adler.ttc", true);
        pgResMgr::loadResource(TEST_DATA_DIR "adler.otf", true);

        pgAssert(pgDrawMgr::getFontIndexNum(pgID_("adler.ttf")) == 1);
        pgAssert(pgDrawMgr::getFontIndexNum(pgID_("adler.ttc")) == 3);
        pgAssert(pgDrawMgr::getFontIndexNum(pgID_("adler.otf")) == 1);

        pgAssert(pgDrawMgr::getFontID() == pgID::ZERO);
        pgAssert(pgDrawMgr::getFontIndex() == 0);
        pgAssert(pgDrawMgr::getFontSize() == 16);

        pgAssertThrow(pgDrawMgr::calcFontDrawWidth("TEST %s %d", "MESSAGE", 123), pgDrawMgr::ExceptionInvalidCall);
        pgAssertThrow(pgDrawMgr::calcFontDrawWidth(L"TEST %s %d", L"MESSAGE", 123), pgDrawMgr::ExceptionInvalidCall);

        pgDrawMgr::setFont(pgID_("adler.ttc"), 2);
        pgAssert(pgDrawMgr::getFontID() == pgID_("adler.ttc"));
        pgAssert(pgDrawMgr::getFontIndex() == 2);
        pgAssert(pgDrawMgr::getFontSize() == 16);

        pgDrawMgr::setFontSize(123);
        pgAssert(pgDrawMgr::getFontSize() == 123);

        pgAssert(pgDrawMgr::calcFontDrawWidth("TEST") > 40);
        pgAssert(pgDrawMgr::calcFontDrawWidth(L"TEST") > 40);
        pgAssert(pgDrawMgr::calcFontDrawWidth("TEST") == pgDrawMgr::calcFontDrawWidth(L"TEST"));

        pgAssertThrow(pgDrawMgr::getFontIndexNum(pgID::ZERO), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::getFontIndexNum(pgID_("DUMMY")), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::setFont(pgID::ZERO, 0), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::setFont(pgID_("adler.ttc"), 3), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::setFontSize(0), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::calcFontDrawWidth(static_cast<const char*>(NULL)), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::calcFontDrawWidth(static_cast<const wchar_t*>(NULL)), pgDrawMgr::ExceptionInvalidArgument);

        pgResMgr::removeResource(pgID_("adler.ttf"));
        pgResMgr::removeResource(pgID_("adler.ttc"));

        pgDrawMgr::destroyBeforeRes();

        pgResMgr::removeResource(pgID_("adler.otf"));
    }

    /*
        static bool hasShader(pgID shd_id)
        static pgShd* getShader(pgID shd_id)
        static pgShd* newShader(pgID shd_id, const char* vert_code, const char* frag_code, u8 uni_num, u8 att_num, u8 tex_num)
        static void deleteShader(pgID shd_id)
        static pgShd* getFirstShaderN()
        static pgShd* getLastShaderN()
    */
    {
        pgAssertThrow(pgDrawMgr::hasShader(pgID::ZERO), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::getShader(pgID::ZERO), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::newShader(pgID::ZERO, NULL, NULL, 0, 0, 0), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::deleteShader(pgID::ZERO), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::getFirstShaderN(), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::getLastShaderN(), pgDrawMgr::ExceptionNotInitialized);

        u32 used_memory_size = pgMemMgr::getCurUsedMemorySize();

        pgDrawMgr::createAfterRes();

        pgShd* def_shd = pgDrawMgr::getShader(pgDrawMgr::DEFAULT_SHADER_ID);
        pgAssert(def_shd->getID() == pgDrawMgr::DEFAULT_SHADER_ID);
        pgAssert(pgDrawMgr::hasShader(pgDrawMgr::DEFAULT_SHADER_ID));
        pgAssert(pgDrawMgr::getFirstShaderN() == def_shd && pgDrawMgr::getLastShaderN() == def_shd);

        pgID id = pgID_("TEST_SHADER");
        pgShd* shd = pgDrawMgr::newShader(id, "dummy", "dummy", 100, 100, 3);
        pgAssert(shd->getID() == id);
        pgAssert(pgDrawMgr::hasShader(id));
        pgAssert(pgDrawMgr::getFirstShaderN() == def_shd && pgDrawMgr::getLastShaderN() == shd);

        pgAssertThrow(pgDrawMgr::newShader(id, "dummy", "dummy", 0, 0, 0), pgDrawMgr::ExceptionSameIDExists);

        pgDrawMgr::deleteShader(id);
        pgAssert(!pgDrawMgr::hasShader(id));
        pgAssert(pgDrawMgr::getFirstShaderN() == def_shd && pgDrawMgr::getLastShaderN() == def_shd);

        pgAssertThrow(pgDrawMgr::deleteShader(id), pgDrawMgr::ExceptionNotFound);

        pgDrawMgr::deleteShader(pgDrawMgr::DEFAULT_SHADER_ID);
        pgAssert(!pgDrawMgr::hasShader(pgDrawMgr::DEFAULT_SHADER_ID));
        pgAssert(!pgDrawMgr::getFirstShaderN() && !pgDrawMgr::getLastShaderN());

        pgAssertThrow(pgDrawMgr::hasShader(pgID::ZERO), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::getShader(pgID::ZERO), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newShader(pgID::ZERO, "dummy", "dummy", 0, 0, 0), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newShader(pgID::genID(), NULL, "dummy", 0, 0, 0), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newShader(pgID::genID(), "dummy", NULL, 0, 0, 0), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newShader(pgID::genID(), "dummy", "dummy", 101, 0, 0), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newShader(pgID::genID(), "dummy", "dummy", 0, 101, 0), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newShader(pgID::genID(), "dummy", "dummy", 0, 0, 4), pgDrawMgr::ExceptionInvalidArgument);

        pgDrawMgr::destroyBeforeRes();

        pgAssert(pgMemMgr::getCurUsedMemorySize() == used_memory_size);
    }

    /*
        static bool hasLightSet(pgID lts_id)
        static pgLts* getLightSet(pgID lts_id)
        static pgLts* newLightSet(pgID lts_id)
        static void deleteLightSet(pgID lts_id)
        static pgLts* getFirstLightSetN()
        static pgLts* getLastLightSetN()
    */
    {
        pgAssertThrow(pgDrawMgr::hasLightSet(pgID::ZERO), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::getLightSet(pgID::ZERO), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::newLightSet(pgID::ZERO), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::deleteLightSet(pgID::ZERO), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::getFirstLightSetN(), pgDrawMgr::ExceptionNotInitialized);
        pgAssertThrow(pgDrawMgr::getLastLightSetN(), pgDrawMgr::ExceptionNotInitialized);

        u32 used_memory_size = pgMemMgr::getCurUsedMemorySize();

        pgDrawMgr::createAfterRes();

        pgLts* lts0 = pgDrawMgr::getLightSet(pgDrawMgr::DEFAULT_LIGHT_SET_ID);
        pgAssert(lts0->getID() == pgDrawMgr::DEFAULT_LIGHT_SET_ID);
        pgAssert(pgDrawMgr::hasLightSet(pgDrawMgr::DEFAULT_LIGHT_SET_ID));
        pgAssert(pgDrawMgr::getFirstLightSetN() == lts0 && pgDrawMgr::getLastLightSetN() == lts0);

        pgID id = pgID_("TEST_LIGHT_SET");
        pgLts* lts = pgDrawMgr::newLightSet(id);
        pgAssert(lts->getID() == id);
        pgAssert(pgDrawMgr::hasLightSet(id));
        pgAssert(pgDrawMgr::getFirstLightSetN() == lts0 && pgDrawMgr::getLastLightSetN() == lts);

        pgAssertThrow(pgDrawMgr::newLightSet(id), pgDrawMgr::ExceptionSameIDExists);

        pgDrawMgr::deleteLightSet(id);
        pgAssert(!pgDrawMgr::hasLightSet(id));
        pgAssert(pgDrawMgr::getFirstLightSetN() == lts0 && pgDrawMgr::getLastLightSetN() == lts0);

        pgAssertThrow(pgDrawMgr::deleteLightSet(id), pgDrawMgr::ExceptionNotFound);

        pgDrawMgr::deleteLightSet(pgDrawMgr::DEFAULT_LIGHT_SET_ID);
        pgAssert(!pgDrawMgr::hasLightSet(pgDrawMgr::DEFAULT_LIGHT_SET_ID));
        pgAssert(!pgDrawMgr::getFirstLightSetN() && !pgDrawMgr::getLastLightSetN());

        pgAssertThrow(pgDrawMgr::hasLightSet(pgID::ZERO), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::getLightSet(pgID::ZERO), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::newLightSet(pgID::ZERO), pgDrawMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDrawMgr::deleteLightSet(pgID::ZERO), pgDrawMgr::ExceptionInvalidArgument);

        pgDrawMgr::newLightSet(pgID::genID());
        pgDrawMgr::newLightSet(pgID::genID());

        pgDrawMgr::destroyBeforeRes();

        pgAssert(pgMemMgr::getCurUsedMemorySize() == used_memory_size);
    }
}
