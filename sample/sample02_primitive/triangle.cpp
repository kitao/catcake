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
