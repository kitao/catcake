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
