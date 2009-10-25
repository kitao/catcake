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


#include "pogolyn_main.h"


class Golem : public pgTask
{
public:
    Golem();

private:
    virtual void onUpdate();

    pgMdl m_golem_mdl;
    pgSprt m_copyright_sprt;

    r32 m_cur_mouse_x;
    r32 m_cur_mouse_y;
    r32 m_last_mouse_x;
    r32 m_last_mouse_y;

    r32 m_rot_x;
    r32 m_rot_y;
};


void newGolem()
{
    pgNewTask(Golem);
}


Golem::Golem() : pgTask(ORDER_ZERO)
{
    m_golem_mdl.init(pgID_("golem.3ds"), pgDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_golem_mdl.setLightSetID(pgDrawMgr::DEFAULT_LIGHT_SET_ID);
    m_golem_mdl.getRootDraw()->local() = pgMat::UNIT.rotateX_s32(-90);

    m_copyright_sprt.init(1, pgDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_copyright_sprt.setTextureID(pgID_("copyright_304x34.png"));
    m_copyright_sprt.setPreset_defaultBlendHalf();

    m_copyright_sprt.dataPos(0).set(0.0f, -150.0f);
    m_copyright_sprt.dataCol(0).set(128, 255, 128, 160);
    m_copyright_sprt.setDataSize(0, 304, 34);

    m_rot_y = 0.0f;
    m_rot_x = -20.0f;

    m_cur_mouse_x = m_last_mouse_x = 0.0f;
    m_cur_mouse_y = m_last_mouse_y = 0.0f;

    pgLts* lts = pgDrawMgr::getLightSet(pgDrawMgr::DEFAULT_LIGHT_SET_ID);

    lts->setAmbientColor(pgCol(128, 128, 128));
    lts->setParaLightActive(0, true);
    lts->setParaLightDir(0, pgVec(1.0f, -1.0f, -1.0f));
    lts->setParaLightColor(0, pgCol::FULL);
}


void Golem::onUpdate()
{
    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_F))
    {
        pgSysMgr::toggleFullScreen(640, 480);
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_Q))
    {
        pgEndPogolyn();
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_T))
    {
        m_golem_mdl.setTextureID((m_golem_mdl.getTextureID() == pgID::ZERO) ? pgID_("golem.png") : pgID::ZERO);
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_L))
    {
        m_golem_mdl.setLightSetID((m_golem_mdl.getLightSetID() == pgID::ZERO) ? pgDrawMgr::DEFAULT_LIGHT_SET_ID : pgID::ZERO);
    }

    m_last_mouse_x = m_cur_mouse_x;
    m_last_mouse_y = m_cur_mouse_y;

    pgScr* scr = pgDrawMgr::getScreen(pgDrawMgr::DEFAULT_3D_SCREEN_ID);

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_LBUTTON))
    {
        m_cur_mouse_x = m_last_mouse_x = scr->framebufferXToScreenX(pgKeyMgr::getMouseX());
        m_cur_mouse_y = m_last_mouse_y = scr->framebufferYToScreenY(pgKeyMgr::getMouseY());
    }

    if (pgKeyMgr::isOn(pgKeyMgr::KEY_LBUTTON))
    {
        m_cur_mouse_x = scr->framebufferXToScreenX(pgKeyMgr::getMouseX());
        m_cur_mouse_y = scr->framebufferYToScreenY(pgKeyMgr::getMouseY());
    }

    m_rot_y += 0.1f - (m_cur_mouse_x - m_last_mouse_x) * 0.5f;
    m_rot_x += (m_cur_mouse_y - m_last_mouse_y) * 0.3f;
    m_rot_x = pgMath::clamp(m_rot_x, -80.0f, 80.0f);

    scr->view() = pgMat::UNIT.rotateY_r32(m_rot_y).rotateX_r32(m_rot_x).translate(0.0f, 150.0f, 1000.0f + m_rot_x * m_rot_x * 0.1f);

    for (s32 i = -10; i <= 10; i++)
    {
        r32 offset = i * 50.0f;

        pgDbgMgr::drawLine(pgVec(offset, 0.0f, 500.0f), pgVec(offset, 0.0f, -500.0f), pgCol(0, 255, 0, 128), pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        pgDbgMgr::drawLine(pgVec(500.0f, 0.0f, offset), pgVec(-500.0f, 0.0f, offset), pgCol(0, 255, 0, 128), pgDrawMgr::DEFAULT_3D_SCREEN_ID);
    }
}
