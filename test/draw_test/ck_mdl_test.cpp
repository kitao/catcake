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


void ckMdlTest()
{
    /*
        ckMdl()
        ~ckMdl()
        void init(ckID mdl_data_id, ckID scr_id)
        void init(ckID mdl_data_id, ckDraw* parent)
        const ckMdlData* getModelData() const
        ckID getTextureID() const
        void setTextureID(ckID tex_id)
        ckID getLightSetID() const
        void setLightSetID(ckID lts_id)
        ckDraw* getRootDraw()
        ckDraw* getNodeDraw(u16 node_index)
    */
    {
        ckUtil::import3DS(TEST_DATA_DIR "valid.3ds", ckID_("rgb.png"), false, false, 1.0f);

        ckDrawMgr::createAfterRes();
        ckResMgr::loadResource(TEST_DATA_DIR "rgb.png", true);

        ckMdl mdl;

        ckAssertThrow(mdl.getModelData(), ckMdl::ExceptionNotInitialized);
        ckAssertThrow(mdl.getTextureID(), ckMdl::ExceptionNotInitialized);
        ckAssertThrow(mdl.setTextureID(ckID::ZERO), ckMdl::ExceptionNotInitialized);
        ckAssertThrow(mdl.getLightSetID(), ckMdl::ExceptionNotInitialized);
        ckAssertThrow(mdl.setLightSetID(ckID::ZERO), ckMdl::ExceptionNotInitialized);
        ckAssertThrow(mdl.getRootDraw(), ckMdl::ExceptionNotInitialized);
        ckAssertThrow(mdl.getNodeDraw(0), ckMdl::ExceptionNotInitialized);

        mdl.init(ckID_("valid.3ds"), ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        mdl.init(ckID_("valid.3ds"), ckDrawMgr::DEFAULT_3D_SCREEN_ID);

        ckAssert(mdl.getModelData()->getNodeNum() == 3);
        ckAssert(mdl.getTextureID() == ckID_("rgb.png"));
        ckAssert(mdl.getLightSetID() == ckID::ZERO);
        ckAssert(mdl.getRootDraw()->getType() == ckDraw::TYPE_NODE);

        ckAssert(mdl.getNodeDraw(0)->getType() == ckDraw::TYPE_PRIM);
        ckAssert(mdl.getNodeDraw(1)->getType() == ckDraw::TYPE_PRIM);
        ckAssert(mdl.getNodeDraw(2)->getType() == ckDraw::TYPE_PRIM);

        ckAssert(mdl.getNodeDraw(0)->getBlendMode() == ckDraw::BLEND_OFF);
        ckAssert(mdl.getNodeDraw(1)->getBlendMode() == ckDraw::BLEND_HALF);
        ckAssert(mdl.getNodeDraw(2)->getBlendMode() == ckDraw::BLEND_ADD);

        ckAssert(mdl.getRootDraw()->getTextureID() == ckID_("rgb.png"));
        ckAssert(mdl.getNodeDraw(0)->getTextureID() == ckID_("rgb.png"));
        ckAssert(mdl.getNodeDraw(1)->getTextureID() == ckID_("rgb.png"));
        ckAssert(mdl.getNodeDraw(2)->getTextureID() == ckID_("rgb.png"));

        mdl.setTextureID(ckID::ZERO);
        ckAssert(mdl.getRootDraw()->getTextureID() == ckID::ZERO);
        ckAssert(mdl.getNodeDraw(0)->getTextureID() == ckID::ZERO);
        ckAssert(mdl.getNodeDraw(1)->getTextureID() == ckID::ZERO);
        ckAssert(mdl.getNodeDraw(2)->getTextureID() == ckID::ZERO);

        mdl.setLightSetID(ckDrawMgr::DEFAULT_LIGHT_SET_ID);
        ckAssert(mdl.getLightSetID() == ckDrawMgr::DEFAULT_LIGHT_SET_ID);

        ckAssertThrow(mdl.getNodeDraw(3), ckMdl::ExceptionInvalidArgument);

        ckResMgr::removeResource(ckID_("rgb.png"));
        ckDrawMgr::destroyBeforeRes();

        ckResMgr::removeResource(ckID_("valid.3ds"));
    }
}
