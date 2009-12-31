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


class Stage : public ckTask
{
public:
    Stage();

private:
    virtual void onUpdate();

    r32 m_rot_x;
    r32 m_rot_y;

    ckPrim m_road_prim;
    ckPrim m_line_prim;
};


void newStage()
{
    ckNewTask(Stage);
}


Stage::Stage() : ckTask(ORDER_ZERO)
{
    m_rot_y = -120.0f;
    m_rot_x = -10.0f;

    ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID)->setClearColor(ckCol(96, 32, 0));

    ckLts* lts = ckDrawMgr::getLightSet(ckDrawMgr::DEFAULT_LIGHT_SET_ID);

    lts->setAmbientColor(ckCol(160, 128, 128));
    lts->setParaLightActive(0, true);
    lts->setParaLightDir(0, ckVec(1.0f, -1.0f, -1.0f));
    lts->setParaLightColor(0, ckCol(255, 128, 96));

    m_road_prim.init(ckPrim::MODE_TRIANGLE_STRIP, 6, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_road_prim.setBlendMode(ckDraw::BLEND_HALF, true);
    m_road_prim.setSortOffset(-1000.0f);

    m_road_prim.dataPos(0).set(200.0f, 0.0f, -20000.0f);
    m_road_prim.dataPos(1).set(-200.0f, 0.0f, -20000.0f);
    m_road_prim.dataPos(2).set(200.0f, 0.0f, 0.0f);
    m_road_prim.dataPos(3).set(-200.0f, 0.0f, 0.0f);
    m_road_prim.dataPos(4).set(200.0f, 0.0f, 20000.0f);
    m_road_prim.dataPos(5).set(-200.0f, 0.0f, 20000.0f);

    for (s32 i = 0; i < 6; i++)
    {
        m_road_prim.dataCol(i).set(64, 64, 80, (i >= 2 && i <= 3) ? 255 : 0);
    }

    m_line_prim.init(ckPrim::MODE_TRIANGLES, 6 * 100, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_line_prim.setBlendMode(ckDraw::BLEND_HALF, true);

    for (s32 i = 0; i < 100; i++)
    {
        s32 offset = i - 50;

        m_line_prim.dataPos(i * 6 + 0).set(15.0f, 0.0f, -100.0f + offset * 400.0f);
        m_line_prim.dataPos(i * 6 + 1).set(-15.0f, 0.0f, -100.0f + offset * 400.0f);
        m_line_prim.dataPos(i * 6 + 2).set(15.0f, 0.0f, 100.0f + offset * 400.0f);
        m_line_prim.dataPos(i * 6 + 4).set(-15.0f, 0.0f, -100.0f + offset * 400.0f);
        m_line_prim.dataPos(i * 6 + 3).set(-15.0f, 0.0f, 100.0f + offset * 400.0f);
        m_line_prim.dataPos(i * 6 + 5).set(15.0f, 0.0f, 100.0f + offset * 400.0f);

        for (s32 j = 0; j < 6; j++)
        {
            m_line_prim.dataCol(i * 6 + j).set(255, 255, 255, 100 - ckMath::abs(offset) * 2);
        }
    }
}


void Stage::onUpdate()
{
    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_F))
    {
        ckSysMgr::toggleFullScreen(640, 480);
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_Q))
    {
        ckEndCatcake();
    }

    m_line_prim.local().trans.z += 0.3f;

    if (m_line_prim.local().trans.z >= 400.0f)
    {
        m_line_prim.local().trans.z -= 400.0f;
    }

    m_rot_y -= 0.1f;

    if (ckKeyMgr::isOn(ckKeyMgr::KEY_RIGHT))
    {
        m_rot_y += 1.0f;
    }

    if (ckKeyMgr::isOn(ckKeyMgr::KEY_LEFT))
    {
        m_rot_y -= 1.0f;
    }

    if (ckKeyMgr::isOn(ckKeyMgr::KEY_UP))
    {
        m_rot_x -= 1.0f;
    }

    if (ckKeyMgr::isOn(ckKeyMgr::KEY_DOWN))
    {
        m_rot_x += 1.0f;
    }

    m_rot_x = ckMath::clamp(m_rot_x, -80.0f, -5.0f);

    ckScr* scr = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    scr->view() = ckMat::UNIT.rotateY_r32(m_rot_y).rotateX_r32(m_rot_x).translate(0.0f, 30.0f, 650.0f - m_rot_x * 2.0f);
}
