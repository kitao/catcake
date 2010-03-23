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
        ckUtil::import3DSModel(TEST_DATA_DIR "valid.3ds", ckID_("rgb.png"), false, false, 1.0f);

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
