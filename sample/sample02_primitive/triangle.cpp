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


class Triangle : public ckTask
{
public:
    Triangle();

private:
    virtual void onUpdate();

    ckScr* m_scr2d;
    ckPrim m_tri_prim1;
    ckPrim m_tri_prim2;
};


void newTriangle()
{
    ckNewTask(Triangle);
}


Triangle::Triangle() : ckTask(ORDER_ZERO)
{
    m_scr2d = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_2D_SCREEN_ID);

    /*
        set up m_tri_prim1
    */
    m_tri_prim1.init(ckPrim::MODE_TRIANGLES, 3, ckDrawMgr::DEFAULT_3D_SCREEN_ID);

    m_tri_prim1.dataPos(0).set(0.0f, 100.0f, 0.0f);
    m_tri_prim1.dataPos(1).set(-110.0f, -100.0f, 0.0f);
    m_tri_prim1.dataPos(2).set(110.0f, -100.0f, 0.0f);

    m_tri_prim1.dataCol(0).set(255, 0, 0);
    m_tri_prim1.dataCol(1).set(0, 255, 0);
    m_tri_prim1.dataCol(2).set(0, 0, 255);

    m_tri_prim1.local().trans.set(-150.0f, 0.0f, 0.0f);

    /*
        set up m_tri_prim2
    */
    m_tri_prim2.init(ckPrim::MODE_TRIANGLES, 6, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_tri_prim2.setTextureID(ckID_("coin_400x200.png"));
    m_tri_prim2.setDrawFlag(ckDraw::FLAG_BACKFACE_CULLING, true);

    m_tri_prim2.dataPos(0).set(0.0f, 100.0f, 0.0f);
    m_tri_prim2.dataPos(1).set(-110.0f, -100.0f, 0.0f);
    m_tri_prim2.dataPos(2).set(110.0f, -100.0f, 0.0f);

    m_tri_prim2.dataPos(3).set(0.0f, 100.0f, 0.0f);
    m_tri_prim2.dataPos(4).set(110.0f, -100.0f, 0.0f);
    m_tri_prim2.dataPos(5).set(-110.0f, -100.0f, 0.0f);

    m_tri_prim2.setDataUV(0, 0.25f, 0.0f);
    m_tri_prim2.setDataUV(1, 0.035f, 0.75f);
    m_tri_prim2.setDataUV(2, 0.465f, 0.75f);

    m_tri_prim2.setDataUV(3, 0.75f, 0.0f);
    m_tri_prim2.setDataUV(4, 0.535f, 0.75f);
    m_tri_prim2.setDataUV(5, 0.965f, 0.75f);

    m_tri_prim2.local().trans.set(150.0f, 0.0f, 0.0f);
}


void Triangle::onUpdate()
{
    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_F))
    {
        ckSysMgr::toggleFullScreen(640, 480);
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_Q))
    {
        ckEndCatcake();
    }

    ckDbgMgr::drawBox(ckMat::UNIT, ckVec(400.0f, 400.0f, 400.0f), ckCol::ZERO, ckCol(128, 128, 128), ckDrawMgr::DEFAULT_3D_SCREEN_ID);

    m_scr2d->view() = m_scr2d->view().rotateZ_r32(-0.1f);

    m_tri_prim1.local() = m_tri_prim1.local().rotateY_s32(2);
    m_tri_prim2.local() = m_tri_prim2.local().rotateY_s32(2);
}
