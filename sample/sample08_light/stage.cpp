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
    ckLts* m_lts1;
    ckLts* m_lts2;
    ckLts* m_cur_lts;
};


void newStage()
{
    ckNewTask(Stage);
}


Stage::Stage() : ckTask(ORDER_ZERO)
{
    m_rot_y = 0.0f;
    m_rot_x = -30.0f;

    /*
        setup lightset1
    */
    m_lts1 = ckDrawMgr::getLightSet(ckDrawMgr::DEFAULT_LIGHT_SET_ID);

    m_lts1->setAmbientColor(ckCol(64, 32, 64));

    m_lts1->setParaLightActive(0, true);
    m_lts1->setParaLightDir(0, ckVec(1.0f, -1.0f, -1.0f));
    m_lts1->setParaLightColor(0, ckCol(64, 64, 64));

    ckLit* lit1 = m_lts1->newPointLight(ckID::genID());
    lit1->setPos(ckVec(200.0f, 50.0f, 200.0f));
    lit1->setRadius(50.0f, 400.0f);
    lit1->setColor(ckCol(0, 255, 255));

    ckLit* lit2 = m_lts1->newPointLight(ckID::genID());
    lit2->setPos(ckVec(-200.0f, -50.0f, -200.0f));
    lit2->setRadius(50.0f, 400.0f);
    lit2->setColor(ckCol(255, 0, 0));

    /*
        setup lightset2
    */
    m_lts2 = ckDrawMgr::newLightSet(ckID::genID());

    m_lts2->setAmbientColor(ckCol::ZERO);

    m_lts2->setParaLightActive(0, true);
    m_lts2->setParaLightDir(0, ckVec(-1.0f, -1.0f, 0.0f));
    m_lts2->setParaLightColor(0, ckCol(255, 0, 0));

    m_lts2->setParaLightActive(1, true);
    m_lts2->setParaLightDir(1, ckVec(0.0f, -1.0f, -1.0f));
    m_lts2->setParaLightColor(1, ckCol(0, 255, 0));

    m_lts2->setParaLightActive(2, true);
    m_lts2->setParaLightDir(2, ckVec(-1.0f, 0.0f, -1.0f));
    m_lts2->setParaLightColor(2, ckCol(0, 0, 255));

    m_cur_lts = m_lts1;
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

    ckVec pos1(200.0f, 0.0f, 200.0f);
    ckVec pos2(-200.0f, 0.0f, 200.0f);
    ckVec pos3(-200.0f, 0.0f, -200.0f);
    ckVec pos4(200.0f, 0.0f, -200.0f);
    ckCol col(128, 128, 128);

    ckDbgMgr::drawLine(pos1, pos2, col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    ckDbgMgr::drawLine(pos2, pos3, col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    ckDbgMgr::drawLine(pos3, pos4, col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    ckDbgMgr::drawLine(pos4, pos1, col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);

    m_rot_y += 0.1f;

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

    m_rot_x = ckMath::clamp(m_rot_x, -80.0f, -20.0f);

    ckScr* scr = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    scr->view() = ckMat::UNIT.rotateY_r32(m_rot_y).rotateX_r32(m_rot_x).translate(0.0f, 0.0f, 650.0f + m_rot_x);

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_L))
    {
        ckMsg<4> msg;

        m_cur_lts = (m_cur_lts == m_lts1) ? m_lts2 : m_lts1;

        msg.setParam(0, m_cur_lts->getID());

        ckTaskMgr::sendMessage(ckID_("CHANGE LIGHT SET"), msg);
    }
}
