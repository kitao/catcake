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


void pgMdlTest()
{
    /*
        pgMdl()
        ~pgMdl()
        void init(pgID mdl_data_id, pgID scr_id)
        void init(pgID mdl_data_id, pgDraw* parent)
        const pgMdlData* getModelData() const
        pgID getTextureID() const
        void setTextureID(pgID tex_id)
        pgID getLightSetID() const
        void setLightSetID(pgID lts_id)
        pgDraw* getRootDraw()
        pgDraw* getNodeDraw(u16 node_index)
    */
    {
        pgUtil::import3DS(TEST_DATA_DIR "valid.3ds", pgID_("rgb.png"), false, false, 1.0f);

        pgDrawMgr::createAfterRes();
        pgResMgr::loadResource(TEST_DATA_DIR "rgb.png", true);

        pgMdl mdl;

        pgAssertThrow(mdl.getModelData(), pgMdl::ExceptionNotInitialized);
        pgAssertThrow(mdl.getTextureID(), pgMdl::ExceptionNotInitialized);
        pgAssertThrow(mdl.setTextureID(pgID::ZERO), pgMdl::ExceptionNotInitialized);
        pgAssertThrow(mdl.getLightSetID(), pgMdl::ExceptionNotInitialized);
        pgAssertThrow(mdl.setLightSetID(pgID::ZERO), pgMdl::ExceptionNotInitialized);
        pgAssertThrow(mdl.getRootDraw(), pgMdl::ExceptionNotInitialized);
        pgAssertThrow(mdl.getNodeDraw(0), pgMdl::ExceptionNotInitialized);

        mdl.init(pgID_("valid.3ds"), pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        mdl.init(pgID_("valid.3ds"), pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        pgAssert(mdl.getModelData()->getNodeNum() == 3);
        pgAssert(mdl.getTextureID() == pgID_("rgb.png"));
        pgAssert(mdl.getLightSetID() == pgID::ZERO);
        pgAssert(mdl.getRootDraw()->getType() == pgDraw::TYPE_NODE);

        pgAssert(mdl.getNodeDraw(0)->getType() == pgDraw::TYPE_PRIM);
        pgAssert(mdl.getNodeDraw(1)->getType() == pgDraw::TYPE_PRIM);
        pgAssert(mdl.getNodeDraw(2)->getType() == pgDraw::TYPE_PRIM);

        pgAssert(mdl.getNodeDraw(0)->getBlendMode() == pgDraw::BLEND_OFF);
        pgAssert(mdl.getNodeDraw(1)->getBlendMode() == pgDraw::BLEND_HALF);
        pgAssert(mdl.getNodeDraw(2)->getBlendMode() == pgDraw::BLEND_ADD);

        pgAssert(mdl.getRootDraw()->getTextureID() == pgID_("rgb.png"));
        pgAssert(mdl.getNodeDraw(0)->getTextureID() == pgID_("rgb.png"));
        pgAssert(mdl.getNodeDraw(1)->getTextureID() == pgID_("rgb.png"));
        pgAssert(mdl.getNodeDraw(2)->getTextureID() == pgID_("rgb.png"));

        mdl.setTextureID(pgID::ZERO);
        pgAssert(mdl.getRootDraw()->getTextureID() == pgID::ZERO);
        pgAssert(mdl.getNodeDraw(0)->getTextureID() == pgID::ZERO);
        pgAssert(mdl.getNodeDraw(1)->getTextureID() == pgID::ZERO);
        pgAssert(mdl.getNodeDraw(2)->getTextureID() == pgID::ZERO);

        mdl.setLightSetID(pgDrawMgr::DEFAULT_LIGHT_SET_ID);
        pgAssert(mdl.getLightSetID() == pgDrawMgr::DEFAULT_LIGHT_SET_ID);

        pgAssertThrow(mdl.getNodeDraw(3), pgMdl::ExceptionInvalidArgument);

        pgResMgr::removeResource(pgID_("rgb.png"));
        pgDrawMgr::destroyBeforeRes();

        pgResMgr::removeResource(pgID_("valid.3ds"));
    }
}
