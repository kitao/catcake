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


#include "catcake.h"


class BlurredScreen : public ckTask
{
public:
    BlurredScreen();
    virtual ~BlurredScreen();

private:
    virtual void onUpdate();

    ckScr* m_blurred_scr;

    ckPrim m_tetra_prim;
    ckPrim m_blur_prim;

    ckNode m_cat_node;
    ckPrim m_mask_prim;
    ckPrim m_scr_prim;
};


void newBlurredScreen()
{
    ckNewTask(BlurredScreen);
}


BlurredScreen::BlurredScreen() : ckTask(ORDER_ZERO)
{
    /*
        set up m_blurred_scr
    */
    m_blurred_scr = ckDrawMgr::newScreen(ckID_("blurred_screen"));
    m_blurred_scr->moveFirst();
    m_blurred_scr->setAreaInFramebuffer(0, 0, 512, 300);
    m_blurred_scr->setViewSize(512.0f, 300.0f);
    m_blurred_scr->setClearColor(ckCol(32, 32, 32));
    m_blurred_scr->attachScreenTexture(ckTex::FORMAT_RGBA); // ckTex::FORMAT_RGB is also OK except on iPhone

    /*
        set up m_tetra_prim
    */
    m_tetra_prim.init(ckPrim::MODE_TRIANGLES, 12, m_blurred_scr->getID());

    ckVec v1(0.0f, 61.5f, 0.0f), v2(58.0f, -24.0f, 0.0f), v3(-29.0f, -24.0f, 50.0f), v4(-29.0f, -24.0f, -50.0f);

    m_tetra_prim.dataPos(0) = v1;
    m_tetra_prim.dataPos(1) = v2;
    m_tetra_prim.dataPos(2) = v3;
    m_tetra_prim.dataPos(3) = v1;
    m_tetra_prim.dataPos(4) = v3;
    m_tetra_prim.dataPos(5) = v4;
    m_tetra_prim.dataPos(6) = v1;
    m_tetra_prim.dataPos(7) = v4;
    m_tetra_prim.dataPos(8) = v2;
    m_tetra_prim.dataPos(9) = v2;
    m_tetra_prim.dataPos(10) = v4;
    m_tetra_prim.dataPos(11) = v3;

    m_tetra_prim.dataCol(0) = m_tetra_prim.dataCol(1) = m_tetra_prim.dataCol(2) = ckCol(255, 64, 64);
    m_tetra_prim.dataCol(3) = m_tetra_prim.dataCol(4) = m_tetra_prim.dataCol(5) = ckCol(64, 255, 64);
    m_tetra_prim.dataCol(6) = m_tetra_prim.dataCol(7) = m_tetra_prim.dataCol(8) = ckCol(64, 64, 255);
    m_tetra_prim.dataCol(9) = m_tetra_prim.dataCol(10) = m_tetra_prim.dataCol(11) = ckCol(255, 255, 64);

    m_tetra_prim.local().trans.set(-50.0f, -10.0f, 0.0f);

    /*
        set up m_blur_prim
    */
    m_blur_prim.init(ckPrim::MODE_TRIANGLE_FAN, 4, m_blurred_scr->getID());
    m_blur_prim.setTextureID(m_blurred_scr->getScreenTextureID());
    m_blur_prim.setBlendMode(ckDraw::BLEND_HALF, true);
    m_blur_prim.setSortOffset(10000.0f);
    m_blur_prim.setDepthTest(ckDraw::DEPTH_TEST_ALWAYS);
    m_blur_prim.setDataRect(0, ckVec::ZERO, 512.0f, 300.0f, ckCol(255, 255, 255, 232), 0.033f, 0.06f, 0.95f, 0.95f);

    /*
        set up m_cat_node, m_mask_prim and m_scr_prim
    */
    m_cat_node.init(ckDrawMgr::DEFAULT_3D_SCREEN_ID);

    m_mask_prim.init(ckPrim::MODE_TRIANGLE_FAN, 4, &m_cat_node);
    m_mask_prim.setTextureID(ckID_("mask_512x300.png"));
    m_mask_prim.setBlendMode(ckDraw::BLEND_HALF, false);
    m_mask_prim.setDrawFlag(ckDraw::FLAG_WRITE_RGB, false);
    m_mask_prim.setDrawFlag(ckDraw::FLAG_WRITE_ALPHA, true);
    m_mask_prim.setDataRect(0, ckVec(50.0f, 20.0f, 0.0f), 512.0f, 300.0f, ckCol::FULL, 0.0f, 0.0f, 1.0f, 1.0f);

    m_scr_prim.init(ckPrim::MODE_TRIANGLE_FAN, 4, &m_cat_node);
    m_scr_prim.setTextureID(m_blurred_scr->getScreenTextureID());
    m_scr_prim.setDepthTest(ckDraw::DEPTH_TEST_ALWAYS);
    m_scr_prim.setBlendMode(ckDraw::BLEND_DEST_ALPHA, false);
    m_scr_prim.setDrawFlag(ckDraw::FLAG_WRITE_ALPHA, true);
    m_scr_prim.setDataRect(0, ckVec(50.0f, 20.0f, 0.0f), 512.0f, 300.0f, ckCol::FULL, 0.0f, 0.0f, 1.0f, 1.0f);
}


BlurredScreen::~BlurredScreen()
{
    ckDrawMgr::deleteScreen(ckID_("blurred_screen"));
}


void BlurredScreen::onUpdate()
{
    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_Q))
    {
        ckEndCatcake();
    }

    m_blurred_scr->updateScreenTexture(false);

    m_tetra_prim.local() = m_tetra_prim.local().rotateX_r32(2.0f).rotateY_r32(1.5f);
    m_cat_node.local() = m_cat_node.local().rotateY_r32(0.5f);

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_B))
    {
        m_blur_prim.setVisible(!m_blur_prim.isVisible());
    }
}
