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


#include "pogolyn.h"


class BlurredScreen : public pgTask
{
public:
    BlurredScreen();
    virtual ~BlurredScreen();

private:
    virtual void onUpdate();

    pgScr* m_blurred_scr;

    pgPrim m_tetra_prim;
    pgPrim m_blur_prim;

    pgNode m_cat_node;
    pgPrim m_mask_prim;
    pgPrim m_scr_prim;
};


void newBlurredScreen()
{
    pgNewTask(BlurredScreen);
}


BlurredScreen::BlurredScreen() : pgTask(ORDER_ZERO)
{
    /*
        set up m_blurred_scr
    */
    m_blurred_scr = pgDrawMgr::newScreen(pgID_("blurred_screen"));
    m_blurred_scr->moveFirst();
    m_blurred_scr->setAreaInFramebuffer(0, 0, 512, 300);
    m_blurred_scr->setViewSize(512.0f, 300.0f);
    m_blurred_scr->setClearColor(pgCol(32, 32, 32));
    m_blurred_scr->attachScreenTexture(pgTex::FORMAT_RGBA); // pgTex::FORMAT_RGB is also OK except on iPhone

    /*
        set up m_tetra_prim
    */
    m_tetra_prim.init(pgPrim::MODE_TRIANGLES, 12, m_blurred_scr->getID());

    pgVec v1(0.0f, 61.5f, 0.0f), v2(58.0f, -24.0f, 0.0f), v3(-29.0f, -24.0f, 50.0f), v4(-29.0f, -24.0f, -50.0f);

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

    m_tetra_prim.dataCol(0) = m_tetra_prim.dataCol(1) = m_tetra_prim.dataCol(2) = pgCol(255, 64, 64);
    m_tetra_prim.dataCol(3) = m_tetra_prim.dataCol(4) = m_tetra_prim.dataCol(5) = pgCol(64, 255, 64);
    m_tetra_prim.dataCol(6) = m_tetra_prim.dataCol(7) = m_tetra_prim.dataCol(8) = pgCol(64, 64, 255);
    m_tetra_prim.dataCol(9) = m_tetra_prim.dataCol(10) = m_tetra_prim.dataCol(11) = pgCol(255, 255, 64);

    m_tetra_prim.local().trans.set(-50.0f, -10.0f, 0.0f);

    /*
        set up m_blur_prim
    */
    m_blur_prim.init(pgPrim::MODE_TRIANGLE_FAN, 4, m_blurred_scr->getID());
    m_blur_prim.setTextureID(m_blurred_scr->getScreenTextureID());
    m_blur_prim.setPreset_defaultBlendHalf();
    m_blur_prim.setSortOffset(10000.0f);
    m_blur_prim.setDepthTest(pgDraw::DEPTH_TEST_ALWAYS);
    m_blur_prim.setDataRect(0, pgVec::ZERO, 512.0f, 300.0f, pgCol(255, 255, 255, 232), 0.033f, 0.06f, 0.95f, 0.95f);

    /*
        set up m_cat_node, m_mask_prim and m_scr_prim
    */
    m_cat_node.init(pgDrawMgr::DEFAULT_3D_SCREEN_ID);

    m_mask_prim.init(pgPrim::MODE_TRIANGLE_FAN, 4, &m_cat_node);
    m_mask_prim.setTextureID(pgID_("mask_512x300.png"));
    m_mask_prim.setBlendMode(pgDraw::BLEND_HALF);
    m_mask_prim.setDrawFlag(pgDraw::FLAG_WRITE_RGB, false);
    m_mask_prim.setDrawFlag(pgDraw::FLAG_WRITE_ALPHA, true);
    m_mask_prim.setDataRect(0, pgVec(50.0f, 20.0f, 0.0f), 512.0f, 300.0f, pgCol::FULL, 0.0f, 0.0f, 1.0f, 1.0f);

    m_scr_prim.init(pgPrim::MODE_TRIANGLE_FAN, 4, &m_cat_node);
    m_scr_prim.setTextureID(m_blurred_scr->getScreenTextureID());
    m_scr_prim.setDepthTest(pgDraw::DEPTH_TEST_ALWAYS);
    m_scr_prim.setBlendMode(pgDraw::BLEND_DEST_ALPHA);
    m_scr_prim.setDrawFlag(pgDraw::FLAG_WRITE_ALPHA, true);
    m_scr_prim.setDataRect(0, pgVec(50.0f, 20.0f, 0.0f), 512.0f, 300.0f, pgCol::FULL, 0.0f, 0.0f, 1.0f, 1.0f);
}


BlurredScreen::~BlurredScreen()
{
    pgDrawMgr::deleteScreen(pgID_("blurred_screen"));
}


void BlurredScreen::onUpdate()
{
    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_Q))
    {
        pgEndPogolyn();
    }

    m_blurred_scr->updateScreenTexture(false);

    m_tetra_prim.local() = m_tetra_prim.local().rotateX_r32(2.0f).rotateY_r32(1.5f);
    m_cat_node.local() = m_cat_node.local().rotateY_r32(0.5f);

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_B))
    {
        m_blur_prim.setVisible(!m_blur_prim.isVisible());
    }
}
