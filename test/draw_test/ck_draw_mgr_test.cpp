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


void ckDrawMgrTest()
{
    /*
        static const ckID INVISIBLE_SCREEN_ID
        static const ckID DEFAULT_3D_SCREEN_ID
        static const ckID DEFAULT_2D_SCREEN_ID
    */
    {
        ckAssert(ckDrawMgr::INVISIBLE_SCREEN_ID == ckID_("INVISIBLE_SCREEN"));
        ckAssert(ckDrawMgr::DEFAULT_3D_SCREEN_ID == ckID_("DEFAULT_3D_SCREEN"));
        ckAssert(ckDrawMgr::DEFAULT_2D_SCREEN_ID == ckID_("DEFAULT_2D_SCREEN"));

        ckDrawMgr::createAfterRes();

        ckScr* scr0d = ckDrawMgr::getScreen(ckDrawMgr::INVISIBLE_SCREEN_ID);

        ckAssert(scr0d->getID() == ckDrawMgr::INVISIBLE_SCREEN_ID);
        ckAssert(scr0d->getLeftInFramebuffer() == 0 && scr0d->getTopInFramebuffer() == 0 && //
            scr0d->getWidthInFramebuffer() == ckSysMgr::getFramebufferWidth() && //
            scr0d->getHeightInFramebuffer() == ckSysMgr::getFramebufferHeight());
        ckAssert(isEqual(scr0d->getViewWidth(), ckSysMgr::getFramebufferWidth()) && //
            isEqual(scr0d->getViewHeight(), ckSysMgr::getFramebufferHeight()));
        ckAssert(!scr0d->isActive());
        ckAssert(scr0d->isClearColor() && scr0d->isClearDepth());
        ckAssert(scr0d->getClearColor() == ckCol(255, 0, 0, 0));
        ckAssert(scr0d->isPerspective());
        ckAssert(isEqual(scr0d->getFocusDist(), static_cast<r32>(ckScr::DEFAULT_FOCUS_DIST)));
        ckAssert(isEqual(scr0d->getNearClipDist(), static_cast<r32>(ckScr::DEFAULT_NEAR_CLIP_DIST)) && //
            isEqual(scr0d->getFarClipDist(), static_cast<r32>(ckScr::DEFAULT_FAR_CLIP_DIST)));
        ckAssert(isEqual(scr0d->view(), ckMat::UNIT.translate(0, 0, static_cast<r32>(ckScr::DEFAULT_FOCUS_DIST))));
        ckAssert(!scr0d->hasScreenTexture());
        ckAssert(scr0d->getGuestScreenID(0) == ckID::ZERO && scr0d->getGuestScreenID(1) == ckID::ZERO && //
            scr0d->getGuestScreenID(2) == ckID::ZERO && scr0d->getGuestScreenID(3) == ckID::ZERO);

        ckScr* scr3d = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);

        ckAssert(scr3d->getID() == ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        ckAssert(scr3d->getLeftInFramebuffer() == 0 && scr3d->getTopInFramebuffer() == 0 && //
            scr3d->getWidthInFramebuffer() == ckSysMgr::getFramebufferWidth() && //
            scr3d->getHeightInFramebuffer() == ckSysMgr::getFramebufferHeight());
        ckAssert(isEqual(scr3d->getViewWidth(), ckSysMgr::getFramebufferWidth()) && //
            isEqual(scr3d->getViewHeight(), ckSysMgr::getFramebufferHeight()));
        ckAssert(scr3d->isActive());
        ckAssert(scr3d->isClearColor() && scr3d->isClearDepth());
        ckAssert(scr3d->getClearColor() == ckCol::ZERO);
        ckAssert(scr3d->isPerspective());
        ckAssert(isEqual(scr3d->getFocusDist(), static_cast<r32>(ckScr::DEFAULT_FOCUS_DIST)));
        ckAssert(isEqual(scr3d->getNearClipDist(), static_cast<r32>(ckScr::DEFAULT_NEAR_CLIP_DIST)) && //
            isEqual(scr3d->getFarClipDist(), static_cast<r32>(ckScr::DEFAULT_FAR_CLIP_DIST)));
        ckAssert(isEqual(scr3d->view(), ckMat::UNIT.translate(0, 0, static_cast<r32>(ckScr::DEFAULT_FOCUS_DIST))));
        ckAssert(!scr3d->hasScreenTexture());
        ckAssert(scr3d->getGuestScreenID(0) == ckID::ZERO && scr3d->getGuestScreenID(1) == ckID::ZERO && //
            scr3d->getGuestScreenID(2) == ckID::ZERO && scr3d->getGuestScreenID(3) == ckID::ZERO);

        ckScr* scr2d = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_2D_SCREEN_ID);

        ckAssert(scr2d->getID() == ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        ckAssert(scr2d->getLeftInFramebuffer() == 0 && scr2d->getTopInFramebuffer() == 0 && //
            scr2d->getWidthInFramebuffer() == ckSysMgr::getFramebufferWidth() && //
            scr2d->getHeightInFramebuffer() == ckSysMgr::getFramebufferHeight());
        ckAssert(isEqual(scr2d->getViewWidth(), ckSysMgr::getFramebufferWidth()) && //
            isEqual(scr2d->getViewHeight(), ckSysMgr::getFramebufferHeight()));
        ckAssert(scr2d->isActive());
        ckAssert(!scr2d->isClearColor() && scr2d->isClearDepth());
        ckAssert(scr2d->getClearColor() == ckCol(255, 0, 0, 0));
        ckAssert(!scr2d->isPerspective());
        ckAssert(isEqual(scr2d->getFocusDist(), static_cast<r32>(ckScr::DEFAULT_FOCUS_DIST)));
        ckAssert(isEqual(scr2d->getNearClipDist(), static_cast<r32>(ckScr::DEFAULT_NEAR_CLIP_DIST)) && //
            isEqual(scr2d->getFarClipDist(), static_cast<r32>(ckScr::DEFAULT_FAR_CLIP_DIST)));
        ckAssert(isEqual(scr2d->view(), ckMat::UNIT.translate(0, 0, static_cast<r32>(ckScr::DEFAULT_FOCUS_DIST))));
        ckAssert(!scr3d->hasScreenTexture());
        ckAssert(scr2d->getGuestScreenID(0) == ckID::ZERO && scr2d->getGuestScreenID(1) == ckID::ZERO && //
            scr2d->getGuestScreenID(2) == ckID::ZERO && scr2d->getGuestScreenID(3) == ckID::ZERO);

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        static const ckID DEFAULT_LIGHT_SET_ID
    */
    {
        ckAssert(ckDrawMgr::DEFAULT_LIGHT_SET_ID == ckID_("DEFAULT_LIGHT_SET"));

        ckDrawMgr::createAfterRes();

        ckLts* lts = ckDrawMgr::getLightSet(ckDrawMgr::DEFAULT_LIGHT_SET_ID);

        ckAssert(lts->getID() == ckDrawMgr::DEFAULT_LIGHT_SET_ID);
        ckAssert(lts->getAmbientColor() == ckCol::ZERO);
        ckAssert(!lts->isParaLightActive(0) && !lts->isParaLightActive(1) && !lts->isParaLightActive(2));
        ckAssert(lts->getPointLightNum() == 0);

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        static const ckID DEFAULT_NO_TEXTURE_SHADER_ID
        static const ckID DEFAULT_RGB_TEXTURE_SHADER_ID
        static const ckID DEFAULT_RGBA_TEXTURE_SHADER_ID
        static const ckID DEFAULT_ALPHA_TEXTURE_SHADER_ID
    */
    {
        ckAssert(ckDrawMgr::DEFAULT_NO_TEXTURE_SHADER_ID == ckID_("DEFAULT_NO_TEXTURE_SHADER"));
        ckAssert(ckDrawMgr::DEFAULT_RGB_TEXTURE_SHADER_ID == ckID_("DEFAULT_RGB_TEXTURE_SHADER"));
        ckAssert(ckDrawMgr::DEFAULT_RGBA_TEXTURE_SHADER_ID == ckID_("DEFAULT_RGBA_TEXTURE_SHADER"));
        ckAssert(ckDrawMgr::DEFAULT_ALPHA_TEXTURE_SHADER_ID == ckID_("DEFAULT_ALPHA_TEXTURE_SHADER"));

        ckDrawMgr::createAfterRes();

        ckShd* shd;
        
        shd = ckDrawMgr::getShader(ckDrawMgr::DEFAULT_NO_TEXTURE_SHADER_ID);
        ckAssert(shd->getID() == ckDrawMgr::DEFAULT_NO_TEXTURE_SHADER_ID);
        ckAssert(shd->getUniformNum() == 8);
        ckAssert(shd->getAttribNum() == 0);
        ckAssert(shd->getTextureNum() == 0);
        ckAssert(ckDrawMgr::isShaderAvailable() == shd->isValid());

        shd = ckDrawMgr::getShader(ckDrawMgr::DEFAULT_RGB_TEXTURE_SHADER_ID);
        ckAssert(shd->getID() == ckDrawMgr::DEFAULT_RGB_TEXTURE_SHADER_ID);
        ckAssert(shd->getUniformNum() == 8);
        ckAssert(shd->getAttribNum() == 0);
        ckAssert(shd->getTextureNum() == 1);
        ckAssert(ckDrawMgr::isShaderAvailable() == shd->isValid());

        shd = ckDrawMgr::getShader(ckDrawMgr::DEFAULT_RGBA_TEXTURE_SHADER_ID);
        ckAssert(shd->getID() == ckDrawMgr::DEFAULT_RGBA_TEXTURE_SHADER_ID);
        ckAssert(shd->getUniformNum() == 8);
        ckAssert(shd->getAttribNum() == 0);
        ckAssert(shd->getTextureNum() == 1);
        ckAssert(ckDrawMgr::isShaderAvailable() == shd->isValid());

        shd = ckDrawMgr::getShader(ckDrawMgr::DEFAULT_ALPHA_TEXTURE_SHADER_ID);
        ckAssert(shd->getID() == ckDrawMgr::DEFAULT_ALPHA_TEXTURE_SHADER_ID);
        ckAssert(shd->getUniformNum() == 8);
        ckAssert(shd->getAttribNum() == 0);
        ckAssert(shd->getTextureNum() == 1);
        ckAssert(ckDrawMgr::isShaderAvailable() == shd->isValid());

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        static const u32 MAX_RENDER_INTERVAL_MSEC_TIME
    */
    {
        ckAssert(ckDrawMgr::MAX_RENDER_INTERVAL_MSEC_TIME == 100);
    }

    /*
        static bool isShaderAvailable()
    */
    {
        ckAssert(ckDrawMgr::isShaderAvailable() == ckDrawMgr::isShaderAvailable());
    }

    /*
        static u16 getMaxTextureLength()
    */
    {
        ckAssert(ckDrawMgr::getMaxTextureLength() == 2048);
    }

    /*
        static u16 getValidTextureLength(u16 length)
    */
    {
        ckAssert(ckDrawMgr::getValidTextureLength(0) == 8);
        ckAssert(ckDrawMgr::getValidTextureLength(4) == 8);
        ckAssert(ckDrawMgr::getValidTextureLength(8) == 8);
        ckAssert(ckDrawMgr::getValidTextureLength(9) == 16);
        ckAssert(ckDrawMgr::getValidTextureLength(32) == 32);
        ckAssert(ckDrawMgr::getValidTextureLength(33) == 64);
        ckAssert(ckDrawMgr::getValidTextureLength(64) == 64);
        ckAssert(ckDrawMgr::getValidTextureLength(65) == 128);
        ckAssert(ckDrawMgr::getValidTextureLength(128) == 128);
        ckAssert(ckDrawMgr::getValidTextureLength(129) == 256);
        ckAssert(ckDrawMgr::getValidTextureLength(256) == 256);
        ckAssert(ckDrawMgr::getValidTextureLength(257) == 512);
        ckAssert(ckDrawMgr::getValidTextureLength(512) == 512);
        ckAssert(ckDrawMgr::getValidTextureLength(513) == 1024);
        ckAssert(ckDrawMgr::getValidTextureLength(1024) == 1024);
        ckAssert(ckDrawMgr::getValidTextureLength(1025) == 2048);
        ckAssert(ckDrawMgr::getValidTextureLength(2048) == 2048);
        ckAssert(ckDrawMgr::getValidTextureLength(2049) == 2048);
        ckAssert(ckDrawMgr::getValidTextureLength(10000) == 2048);
    }

    /*
        static u16 getTexturePixelSize(ckTex::TexFormat format)
    */
    {
        ckAssert(ckDrawMgr::getTexturePixelSize(ckTex::FORMAT_RGB) == 3);
        ckAssert(ckDrawMgr::getTexturePixelSize(ckTex::FORMAT_RGBA) == 4);
        ckAssert(ckDrawMgr::getTexturePixelSize(ckTex::FORMAT_ALPHA) == 1);
        ckAssert(ckDrawMgr::getTexturePixelSize(ckTex::FORMAT_PNG_RGB) == 3);
        ckAssert(ckDrawMgr::getTexturePixelSize(ckTex::FORMAT_PNG_RGBA) == 4);
        ckAssert(ckDrawMgr::getTexturePixelSize(ckTex::FORMAT_PNG_ALPHA) == 1);
    }

    /*
        static bool isCreated()
        static void createAfterRes()
        static void destroyBeforeRes()
    */
    {
        ckAssert(!ckDrawMgr::isCreated());

        ckDrawMgr::createAfterRes();
        ckDrawMgr::createAfterRes();

        ckAssert(ckDrawMgr::isCreated());

        ckDrawMgr::destroyBeforeRes();
        ckDrawMgr::destroyBeforeRes();

        ckAssert(!ckDrawMgr::isCreated());
    }

    /*
        static bool isRender()
        static void setRender(bool is_render)
        static void deleteAllVramObjForSystem()
        static bool renderForSystem()
    */
    {
        ckAssertThrow(ckDrawMgr::setRender(false), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::isRender(), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::deleteAllVramObjForSystem(), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::renderForSystem(), ckDrawMgr::ExceptionNotInitialized);

        ckDrawMgr::createAfterRes();

        ckDrawMgr::deleteAllVramObjForSystem();

        ckAssert(ckDrawMgr::isRender());

        ckDrawMgr::setRender(false);
        ckAssert(!ckDrawMgr::isRender());

        ckDrawMgr::renderForSystem();

        ckDrawMgr::deleteScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        ckDrawMgr::deleteScreen(ckDrawMgr::DEFAULT_2D_SCREEN_ID);

        ckDrawMgr::setRender(true);
        ckAssert(ckDrawMgr::isRender());

        ckDrawMgr::renderForSystem();

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        static bool hasScreen(ckID scr_id)
        static ckScr* getScreen(ckID scr_id)
        static ckScr* newScreen(ckID scr_id)
        static void deleteScreen(ckID scr_id)
        static ckScr* getFirstScreenN()
        static ckScr* getLastScreenN()
    */
    {
        ckAssertThrow(ckDrawMgr::hasScreen(ckID::ZERO), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::getScreen(ckID::ZERO), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::newScreen(ckID::ZERO), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::deleteScreen(ckID::ZERO), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::getFirstScreenN(), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::getLastScreenN(), ckDrawMgr::ExceptionNotInitialized);

        u32 used_memory_size = ckMemMgr::getCurUsedMemorySize();

        ckDrawMgr::createAfterRes();

        ckScr* scr0d = ckDrawMgr::getScreen(ckDrawMgr::INVISIBLE_SCREEN_ID);
        ckAssert(scr0d->getID() == ckDrawMgr::INVISIBLE_SCREEN_ID);
        ckAssert(ckDrawMgr::hasScreen(ckDrawMgr::INVISIBLE_SCREEN_ID));

        ckScr* scr3d = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        ckAssert(scr3d->getID() == ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        ckAssert(ckDrawMgr::hasScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID));

        ckScr* scr2d = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        ckAssert(scr2d->getID() == ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        ckAssert(ckDrawMgr::hasScreen(ckDrawMgr::DEFAULT_2D_SCREEN_ID));

        ckAssertThrow(ckDrawMgr::getScreen(ckID::genID()), ckDrawMgr::ExceptionNotFound);

        ckID id = ckID_("TEST_SCREEN");
        ckScr* scr = ckDrawMgr::newScreen(id);
        ckAssert(scr->getID() == id);
        ckAssert(ckDrawMgr::hasScreen(id));
        ckAssert(ckDrawMgr::getFirstScreenN() == scr0d && ckDrawMgr::getLastScreenN() == scr);

        ckAssertThrow(ckDrawMgr::newScreen(id), ckDrawMgr::ExceptionSameIDExists);

        ckNode node;
        node.init(id);

        ckDrawMgr::deleteScreen(id);
        ckAssert(!ckDrawMgr::hasScreen(id));
        ckAssert(ckDrawMgr::getFirstScreenN() == scr0d && ckDrawMgr::getLastScreenN() == scr2d);

        ckAssert(node.getScreenID() == ckDrawMgr::INVISIBLE_SCREEN_ID);

        ckAssertThrow(ckDrawMgr::deleteScreen(id), ckDrawMgr::ExceptionNotFound);

        ckDrawMgr::deleteScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        ckAssert(!ckDrawMgr::hasScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID));
        ckAssert(ckDrawMgr::getFirstScreenN() == scr0d && ckDrawMgr::getLastScreenN() == scr2d);

        ckDrawMgr::deleteScreen(ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        ckAssert(!ckDrawMgr::hasScreen(ckDrawMgr::DEFAULT_2D_SCREEN_ID));
        ckAssert(ckDrawMgr::getFirstScreenN() == scr0d && ckDrawMgr::getLastScreenN() == scr0d);

        ckAssertThrow(ckDrawMgr::hasScreen(ckID::ZERO), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::getScreen(ckID::ZERO), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newScreen(ckID::ZERO), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::deleteScreen(ckID::ZERO), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::deleteScreen(ckDrawMgr::INVISIBLE_SCREEN_ID), ckDrawMgr::ExceptionInvalidArgument);

        ckDrawMgr::newScreen(id);

        ckDrawMgr::destroyBeforeRes();

        ckAssert(ckMemMgr::getCurUsedMemorySize() == used_memory_size);
    }

    /*
        static bool hasTexture(ckID tex_id)
        static ckTex* getTexture(ckID tex_id)
        static ckTex* newTexture(ckID tex_id, u16 width, u16 height, ckTex::TexFormat format)
        static ckTex* newTexture(ckID tex_id, u16 width, u16 height, ckTex::TexFormat format, const void* image, u32 image_size)
        static void deleteTexture(ckID tex_id)
        static ckTex* getFirstTextureN()
        static ckTex* getLastTextureN()
    */
    {
        ckAssertThrow(ckDrawMgr::hasTexture(ckID::ZERO), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::getTexture(ckID::ZERO), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::ZERO, 0, 0, ckTex::FORMAT_RGB), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::ZERO, 0, 0, ckTex::FORMAT_RGB, NULL, 0), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::deleteTexture(ckID::ZERO), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::getFirstTextureN(), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::getLastTextureN(), ckDrawMgr::ExceptionNotInitialized);

        u32 used_memory_size = ckMemMgr::getCurUsedMemorySize();

        ckDrawMgr::createAfterRes();

        ckID id1 = ckID::genID();
        ckAssertThrow(ckDrawMgr::getTexture(id1), ckDrawMgr::ExceptionNotFound);

        ckAssert(!ckDrawMgr::getFirstTextureN() && !ckDrawMgr::getLastTextureN());

        ckTex* tex1 = ckDrawMgr::newTexture(id1, 10, 10, ckTex::FORMAT_RGB);
        ckAssert(tex1->getID() == id1);
        ckAssert(ckDrawMgr::hasTexture(id1));
        ckAssert(ckDrawMgr::getFirstTextureN() == tex1 && ckDrawMgr::getLastTextureN() == tex1);

        ckID id2 = ckID::genID();
        ckTex* tex2 = ckDrawMgr::newTexture(id2, 100, 100, ckTex::FORMAT_RGBA, "dummy", 40000);
        ckAssert(tex2->getID() == id2);
        ckAssert(ckDrawMgr::hasTexture(id2));
        ckAssert(ckDrawMgr::getFirstTextureN() == tex1 && ckDrawMgr::getLastTextureN() == tex2);

        ckDrawMgr::deleteTexture(id1);
        ckAssert(!ckDrawMgr::hasTexture(id1));
        ckAssert(ckDrawMgr::getFirstTextureN() == tex2 && ckDrawMgr::getLastTextureN() == tex2);

        ckAssertThrow(ckDrawMgr::deleteTexture(id1), ckDrawMgr::ExceptionNotFound);

        ckDrawMgr::deleteTexture(id2);
        ckAssert(!ckDrawMgr::hasTexture(id2));
        ckAssert(!ckDrawMgr::getFirstTextureN() && !ckDrawMgr::getLastTextureN());

        ckDrawMgr::newTexture(id1, 10, 20, ckTex::FORMAT_ALPHA);
        ckDrawMgr::newTexture(id2, 30, 40, ckTex::FORMAT_RGB, "dummy", 3600);

        ckAssertThrow(ckDrawMgr::hasTexture(ckID::ZERO), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::getTexture(ckID::ZERO), ckDrawMgr::ExceptionInvalidArgument);

        ckAssertThrow(ckDrawMgr::newTexture(ckID::ZERO, 100, 100, ckTex::FORMAT_RGB), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::genID(), 0, 100, ckTex::FORMAT_RGB), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::genID(), 100, 0, ckTex::FORMAT_RGB), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::genID(), 2049, 100, ckTex::FORMAT_RGB), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::genID(), 100, 2049, ckTex::FORMAT_RGB), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::genID(), 100, 100, ckTex::FORMAT_PNG_RGB), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::genID(), 100, 100, ckTex::FORMAT_PNG_RGBA), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::genID(), 100, 100, ckTex::FORMAT_PNG_ALPHA), ckDrawMgr::ExceptionInvalidArgument);

        ckAssertThrow(ckDrawMgr::newTexture(ckID::ZERO, 100, 100, ckTex::FORMAT_RGB, "dummy", 30000), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::genID(), 0, 100, ckTex::FORMAT_RGB, "dummy", 3000), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::genID(), 100, 0, ckTex::FORMAT_RGB, "dummy", 3000), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::genID(), 2049, 100, ckTex::FORMAT_RGB, "dummy", 3000), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::genID(), 100, 2049, ckTex::FORMAT_RGB, "dummy", 3000), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::genID(), 100, 100, ckTex::FORMAT_PNG_RGB, "dummy", 3000), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::genID(), 100, 100, ckTex::FORMAT_PNG_RGBA, "dummy", 3000), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::genID(), 100, 100, ckTex::FORMAT_PNG_ALPHA, "dummy", 3000), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::genID(), 100, 100, ckTex::FORMAT_RGB, NULL, 3000), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newTexture(ckID::genID(), 100, 100, ckTex::FORMAT_RGB, "dummy", 1), ckDrawMgr::ExceptionInvalidArgument);

        ckAssertThrow(ckDrawMgr::deleteTexture(ckID::ZERO), ckDrawMgr::ExceptionInvalidArgument);

        ckDrawMgr::destroyBeforeRes();

        ckAssert(ckMemMgr::getCurUsedMemorySize() == used_memory_size);
    }

    /*
        static u16 getFontIndexNum(ckID font_id)
        static ckID getFontID()
        static u16 getFontIndex()
        static void setFont(ckID font_id, u16 font_index)
        static u16 getFontSize()
        static u16 setFontSize(u16 font_size)
        static u16 calcFontDrawWidth(const char* str, ...)
        static u16 calcFontDrawWidth(const wchar_t* str, ...)
    */
    {
        ckAssertThrow(ckDrawMgr::getFontIndexNum(ckID::ZERO), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::getFontID(), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::getFontIndex(), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::setFont(ckID::ZERO, 0), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::getFontSize(), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::setFontSize(0), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::calcFontDrawWidth(static_cast<const char*>(NULL)), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::calcFontDrawWidth(static_cast<const wchar_t*>(NULL)), ckDrawMgr::ExceptionNotInitialized);

        ckDrawMgr::createAfterRes();

        ckResMgr::loadResource(TEST_DATA_DIR "adler.ttf", true);
        ckResMgr::loadResource(TEST_DATA_DIR "adler.ttc", true);
        ckResMgr::loadResource(TEST_DATA_DIR "adler.otf", true);

        ckAssert(ckDrawMgr::getFontIndexNum(ckID_("adler.ttf")) == 1);
        ckAssert(ckDrawMgr::getFontIndexNum(ckID_("adler.ttc")) == 3);
        ckAssert(ckDrawMgr::getFontIndexNum(ckID_("adler.otf")) == 1);

        ckAssert(ckDrawMgr::getFontID() == ckID::ZERO);
        ckAssert(ckDrawMgr::getFontIndex() == 0);
        ckAssert(ckDrawMgr::getFontSize() == 16);

        ckAssertThrow(ckDrawMgr::calcFontDrawWidth("TEST %s %d", "MESSAGE", 123), ckDrawMgr::ExceptionInvalidCall);
        ckAssertThrow(ckDrawMgr::calcFontDrawWidth(L"TEST %s %d", L"MESSAGE", 123), ckDrawMgr::ExceptionInvalidCall);

        ckDrawMgr::setFont(ckID_("adler.ttc"), 2);
        ckAssert(ckDrawMgr::getFontID() == ckID_("adler.ttc"));
        ckAssert(ckDrawMgr::getFontIndex() == 2);
        ckAssert(ckDrawMgr::getFontSize() == 16);

        ckDrawMgr::setFontSize(123);
        ckAssert(ckDrawMgr::getFontSize() == 123);

        ckAssert(ckDrawMgr::calcFontDrawWidth("TEST") > 40);
        ckAssert(ckDrawMgr::calcFontDrawWidth(L"TEST") > 40);
        ckAssert(ckDrawMgr::calcFontDrawWidth("TEST") == ckDrawMgr::calcFontDrawWidth(L"TEST"));

        ckAssertThrow(ckDrawMgr::getFontIndexNum(ckID::ZERO), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::getFontIndexNum(ckID_("DUMMY")), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::setFont(ckID::ZERO, 0), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::setFont(ckID_("adler.ttc"), 3), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::setFontSize(0), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::calcFontDrawWidth(static_cast<const char*>(NULL)), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::calcFontDrawWidth(static_cast<const wchar_t*>(NULL)), ckDrawMgr::ExceptionInvalidArgument);

        ckResMgr::removeResource(ckID_("adler.ttf"));
        ckResMgr::removeResource(ckID_("adler.ttc"));

        ckDrawMgr::destroyBeforeRes();

        ckResMgr::removeResource(ckID_("adler.otf"));
    }

    /*
        static bool hasShader(ckID shd_id)
        static ckShd* getShader(ckID shd_id)
        static ckShd* newShader(ckID shd_id, const char* vert_code, const char* frag_code, u8 uni_num, u8 att_num, u8 tex_num)
        static void deleteShader(ckID shd_id)
        static ckShd* getFirstShaderN()
        static ckShd* getLastShaderN()
    */
    {
        ckAssertThrow(ckDrawMgr::hasShader(ckID::ZERO), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::getShader(ckID::ZERO), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::newShader(ckID::ZERO, NULL, NULL, 0, 0, 0), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::deleteShader(ckID::ZERO), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::getFirstShaderN(), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::getLastShaderN(), ckDrawMgr::ExceptionNotInitialized);

        u32 used_memory_size = ckMemMgr::getCurUsedMemorySize();

        ckDrawMgr::createAfterRes();

        ckShd* def_shd1 = ckDrawMgr::getShader(ckDrawMgr::DEFAULT_NO_TEXTURE_SHADER_ID);
        ckShd* def_shd2 = ckDrawMgr::getShader(ckDrawMgr::DEFAULT_ALPHA_TEXTURE_SHADER_ID);
        ckAssert(def_shd1->getID() == ckDrawMgr::DEFAULT_NO_TEXTURE_SHADER_ID && def_shd2->getID() == ckDrawMgr::DEFAULT_ALPHA_TEXTURE_SHADER_ID);
        ckAssert(ckDrawMgr::hasShader(ckDrawMgr::DEFAULT_NO_TEXTURE_SHADER_ID) && ckDrawMgr::hasShader(ckDrawMgr::DEFAULT_ALPHA_TEXTURE_SHADER_ID));
        ckAssert(ckDrawMgr::getFirstShaderN() == def_shd1 && ckDrawMgr::getLastShaderN() == def_shd2);

        ckID id = ckID_("TEST_SHADER");
        ckShd* shd = ckDrawMgr::newShader(id, "dummy", "dummy", 100, 100, 3);
        ckAssert(shd->getID() == id);
        ckAssert(ckDrawMgr::hasShader(id));
        ckAssert(ckDrawMgr::getFirstShaderN() == def_shd1 && ckDrawMgr::getLastShaderN() == shd);

        ckAssertThrow(ckDrawMgr::newShader(id, "dummy", "dummy", 0, 0, 0), ckDrawMgr::ExceptionSameIDExists);

        ckDrawMgr::deleteShader(id);
        ckAssert(!ckDrawMgr::hasShader(id));
        ckAssert(ckDrawMgr::getFirstShaderN() == def_shd1 && ckDrawMgr::getLastShaderN() == def_shd2);

        ckAssertThrow(ckDrawMgr::deleteShader(id), ckDrawMgr::ExceptionNotFound);

        ckDrawMgr::deleteShader(ckDrawMgr::DEFAULT_NO_TEXTURE_SHADER_ID);
        ckDrawMgr::deleteShader(ckDrawMgr::DEFAULT_RGB_TEXTURE_SHADER_ID);
        ckDrawMgr::deleteShader(ckDrawMgr::DEFAULT_RGBA_TEXTURE_SHADER_ID);
        ckDrawMgr::deleteShader(ckDrawMgr::DEFAULT_ALPHA_TEXTURE_SHADER_ID);
        ckAssert(!ckDrawMgr::hasShader(ckDrawMgr::DEFAULT_NO_TEXTURE_SHADER_ID));
        ckAssert(!ckDrawMgr::getFirstShaderN() && !ckDrawMgr::getLastShaderN());

        ckAssertThrow(ckDrawMgr::hasShader(ckID::ZERO), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::getShader(ckID::ZERO), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newShader(ckID::ZERO, "dummy", "dummy", 0, 0, 0), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newShader(ckID::genID(), NULL, "dummy", 0, 0, 0), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newShader(ckID::genID(), "dummy", NULL, 0, 0, 0), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newShader(ckID::genID(), "dummy", "dummy", 101, 0, 0), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newShader(ckID::genID(), "dummy", "dummy", 0, 101, 0), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newShader(ckID::genID(), "dummy", "dummy", 0, 0, 4), ckDrawMgr::ExceptionInvalidArgument);

        ckDrawMgr::destroyBeforeRes();

        ckAssert(ckMemMgr::getCurUsedMemorySize() == used_memory_size);
    }

    /*
        static bool hasLightSet(ckID lts_id)
        static ckLts* getLightSet(ckID lts_id)
        static ckLts* newLightSet(ckID lts_id)
        static void deleteLightSet(ckID lts_id)
        static ckLts* getFirstLightSetN()
        static ckLts* getLastLightSetN()
    */
    {
        ckAssertThrow(ckDrawMgr::hasLightSet(ckID::ZERO), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::getLightSet(ckID::ZERO), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::newLightSet(ckID::ZERO), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::deleteLightSet(ckID::ZERO), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::getFirstLightSetN(), ckDrawMgr::ExceptionNotInitialized);
        ckAssertThrow(ckDrawMgr::getLastLightSetN(), ckDrawMgr::ExceptionNotInitialized);

        u32 used_memory_size = ckMemMgr::getCurUsedMemorySize();

        ckDrawMgr::createAfterRes();

        ckLts* lts0 = ckDrawMgr::getLightSet(ckDrawMgr::DEFAULT_LIGHT_SET_ID);
        ckAssert(lts0->getID() == ckDrawMgr::DEFAULT_LIGHT_SET_ID);
        ckAssert(ckDrawMgr::hasLightSet(ckDrawMgr::DEFAULT_LIGHT_SET_ID));
        ckAssert(ckDrawMgr::getFirstLightSetN() == lts0 && ckDrawMgr::getLastLightSetN() == lts0);

        ckID id = ckID_("TEST_LIGHT_SET");
        ckLts* lts = ckDrawMgr::newLightSet(id);
        ckAssert(lts->getID() == id);
        ckAssert(ckDrawMgr::hasLightSet(id));
        ckAssert(ckDrawMgr::getFirstLightSetN() == lts0 && ckDrawMgr::getLastLightSetN() == lts);

        ckAssertThrow(ckDrawMgr::newLightSet(id), ckDrawMgr::ExceptionSameIDExists);

        ckDrawMgr::deleteLightSet(id);
        ckAssert(!ckDrawMgr::hasLightSet(id));
        ckAssert(ckDrawMgr::getFirstLightSetN() == lts0 && ckDrawMgr::getLastLightSetN() == lts0);

        ckAssertThrow(ckDrawMgr::deleteLightSet(id), ckDrawMgr::ExceptionNotFound);

        ckDrawMgr::deleteLightSet(ckDrawMgr::DEFAULT_LIGHT_SET_ID);
        ckAssert(!ckDrawMgr::hasLightSet(ckDrawMgr::DEFAULT_LIGHT_SET_ID));
        ckAssert(!ckDrawMgr::getFirstLightSetN() && !ckDrawMgr::getLastLightSetN());

        ckAssertThrow(ckDrawMgr::hasLightSet(ckID::ZERO), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::getLightSet(ckID::ZERO), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::newLightSet(ckID::ZERO), ckDrawMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDrawMgr::deleteLightSet(ckID::ZERO), ckDrawMgr::ExceptionInvalidArgument);

        ckDrawMgr::newLightSet(ckID::genID());
        ckDrawMgr::newLightSet(ckID::genID());

        ckDrawMgr::destroyBeforeRes();

        ckAssert(ckMemMgr::getCurUsedMemorySize() == used_memory_size);
    }
}
