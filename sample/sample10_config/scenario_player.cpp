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


class ScenarioPlayer : public ckTask
{
public:
    ScenarioPlayer();

private:
    virtual void onUpdate();

    ckScr* m_scr;
    r32 m_rot_y;

    ckConf* m_conf;
    ckEnt* m_ent;
    u16 m_wait_cntr;

    ckPrim m_bg_prim;
};


void newScenarioPlayer()
{
    ckNewTask(ScenarioPlayer);
}


ScenarioPlayer::ScenarioPlayer() : ckTask(ORDER_MINUS_1)
{
    m_scr = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_rot_y = 0.0f;

    m_conf = ckConfMgr::getConfig(ckID_("scenario.ckl"));
    m_ent = m_conf->getEntryFromFirstN("command");
    m_wait_cntr = 0;

    if (!m_conf->isValid())
    {
        ckDbgMgr::trace("*** config file error in %d ***\n", m_conf->getErrorLineNo());
    }

    m_bg_prim.init(ckPrim::MODE_TRIANGLE_FAN, 14, ckDrawMgr::DEFAULT_3D_SCREEN_ID);

    m_bg_prim.dataPos(0) = ckVec::ZERO;
    m_bg_prim.dataCol(0).set(0, 96, 64);

    for (s32 i = 1; i < 14; i++)
    {
        s32 deg = i * 30;

        m_bg_prim.dataPos(i).set(ckMath::cos_s32(deg) * 300.0f, 0.0f, ckMath::sin_s32(deg) * 300.0f);
        m_bg_prim.dataCol(i).set(0, 0, 0);
    }
}


void ScenarioPlayer::onUpdate()
{
    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_F))
    {
        ckSysMgr::toggleFullScreen(640, 480);
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_Q))
    {
        ckEndCatcake();
    }

    m_rot_y += 0.3f;
    m_scr->view() = ckMat::UNIT.rotateY_r32(m_rot_y).rotateX_s32(-15).translate(0.0f, 70.0f, 700.0f);

    if (m_wait_cntr > 0)
    {
        m_wait_cntr--;
    }
    else
    {
        while (m_ent)
        {
            u8 cmd = m_ent->getValue_s32(0);
            u16 val_num = m_ent->getValueNum();

            if (cmd == 1 && val_num == 6) // new_coral(r32 x, r32 z, s32 type, s32 branch_num, s32 survival_time)
            {
                extern void newCoral(r32 x, r32 z, u8 type, u8 branch_num, u16 survival_time);
                newCoral(m_ent->getValue_r32(1), m_ent->getValue_r32(2), m_ent->getValue_s32(3), m_ent->getValue_s32(4), m_ent->getValue_s32(5));

                m_ent = m_conf->getEntryAfterN("command", m_ent);
            }
            else if (cmd == 2 && val_num == 2) // wait(s32 frame_num)
            {
                m_wait_cntr = m_ent->getValue_s32(1);

                m_ent = m_conf->getEntryAfterN("command", m_ent);
                break;
            }
            else if (cmd == 3 && val_num == 1) // repeat
            {
                m_ent = m_conf->getEntryFromFirstN("command");
            }
        }
    }
}
