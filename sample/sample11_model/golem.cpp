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


#include "catcake_main.h"


class Golem : public ckTask
{
public:
    Golem();

private:
    virtual void onUpdate();

    ckMdl m_golem_mdl;
    ckSprt m_copyright_sprt;

    r32 m_cur_mouse_x;
    r32 m_cur_mouse_y;
    r32 m_last_mouse_x;
    r32 m_last_mouse_y;

    r32 m_rot_x;
    r32 m_rot_y;
};


void newGolem()
{
    ckNewTask(Golem);
}


Golem::Golem() : ckTask(ORDER_ZERO)
{
    m_golem_mdl.init(ckID_("golem.3ds"), ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_golem_mdl.setLightSetID(ckDrawMgr::DEFAULT_LIGHT_SET_ID);
    m_golem_mdl.getRootDraw()->local() = ckMat::UNIT.rotateX_s32(-90);

    m_copyright_sprt.init(1, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_copyright_sprt.setTextureID(ckID_("copyright_304x34.png"));
    m_copyright_sprt.setBlendMode(ckDraw::BLEND_HALF, true);

    m_copyright_sprt.dataPos(0).set(0.0f, -150.0f);
    m_copyright_sprt.dataCol(0).set(128, 255, 128, 160);
    m_copyright_sprt.setDataSize(0, 304, 34);

    m_rot_y = 0.0f;
    m_rot_x = -20.0f;

    m_cur_mouse_x = m_last_mouse_x = 0.0f;
    m_cur_mouse_y = m_last_mouse_y = 0.0f;

    ckLts* lts = ckDrawMgr::getLightSet(ckDrawMgr::DEFAULT_LIGHT_SET_ID);

    lts->setAmbientColor(ckCol(128, 128, 128));
    lts->setParaLightActive(0, true);
    lts->setParaLightDir(0, ckVec(1.0f, -1.0f, -1.0f));
    lts->setParaLightColor(0, ckCol::FULL);
}


void Golem::onUpdate()
{
    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_F))
    {
        ckSysMgr::toggleFullScreen(640, 480);
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_Q))
    {
        ckEndCatcake();
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_T))
    {
        m_golem_mdl.setTextureID((m_golem_mdl.getTextureID() == ckID::ZERO) ? ckID_("golem.png") : ckID::ZERO);
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_L))
    {
        m_golem_mdl.setLightSetID((m_golem_mdl.getLightSetID() == ckID::ZERO) ? ckDrawMgr::DEFAULT_LIGHT_SET_ID : ckID::ZERO);
    }

    m_last_mouse_x = m_cur_mouse_x;
    m_last_mouse_y = m_cur_mouse_y;

    ckScr* scr = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_LBUTTON))
    {
        m_cur_mouse_x = m_last_mouse_x = scr->framebufferXToScreenX(ckKeyMgr::getMouseX());
        m_cur_mouse_y = m_last_mouse_y = scr->framebufferYToScreenY(ckKeyMgr::getMouseY());
    }

    if (ckKeyMgr::isOn(ckKeyMgr::KEY_LBUTTON))
    {
        m_cur_mouse_x = scr->framebufferXToScreenX(ckKeyMgr::getMouseX());
        m_cur_mouse_y = scr->framebufferYToScreenY(ckKeyMgr::getMouseY());
    }

    m_rot_y += 0.1f - (m_cur_mouse_x - m_last_mouse_x) * 0.5f;
    m_rot_x += (m_cur_mouse_y - m_last_mouse_y) * 0.3f;
    m_rot_x = ckMath::clamp(m_rot_x, -80.0f, 80.0f);

    scr->view() = ckMat::UNIT.rotateY_r32(m_rot_y).rotateX_r32(m_rot_x).translate(0.0f, 150.0f, 1000.0f + m_rot_x * m_rot_x * 0.1f);

    for (s32 i = -10; i <= 10; i++)
    {
        r32 offset = i * 50.0f;

        ckDbgMgr::drawLine(ckVec(offset, 0.0f, 500.0f), ckVec(offset, 0.0f, -500.0f), ckCol(0, 255, 0, 128), ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        ckDbgMgr::drawLine(ckVec(500.0f, 0.0f, offset), ckVec(-500.0f, 0.0f, offset), ckCol(0, 255, 0, 128), ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    }
}
