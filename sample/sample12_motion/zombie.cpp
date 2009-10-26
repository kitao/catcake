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


class Zombie : public ckTask
{
public:
    Zombie(u16 zombie_no, r32 x, r32 y, r32 z);

private:
    enum ZombieState
    {
        STATE_WALK, //
        STATE_FALL_DOWN, //
        STATE_GET_UP
    };

    virtual void onUpdate();

    u16 m_zombie_no;
    ZombieState m_state;
    u16 m_state_cntr;
    r32 m_aim_z;

    ckMdl m_zombie_mdl;
    ckMot m_zombie_mot;
    ckMdl m_icon_mdl;
};


void newZombie(u16 zombie_no, r32 x, r32 y, r32 z)
{
    ckNewTask(Zombie)(zombie_no, x, y, z);
}


Zombie::Zombie(u16 zombie_no, r32 x, r32 y, r32 z) : ckTask(ORDER_ZERO)
{
    m_zombie_no = zombie_no;
    m_state = STATE_WALK;
    m_state_cntr = 0;
    m_aim_z = z;

    m_zombie_mdl.init(ckID_("zombie.pxm"), ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_zombie_mdl.setLightSetID(ckDrawMgr::DEFAULT_LIGHT_SET_ID);
    m_zombie_mdl.getRootDraw()->local() = ckMat::UNIT;
    m_zombie_mdl.getRootDraw()->local().trans.set(x, y, z);

    m_zombie_mot.init(&m_zombie_mdl, ckID_("zombie.mot"));
    m_zombie_mot.play(&m_zombie_mdl, 1, ckMot::PLAY_LOOP, 1.0f, 0);

    m_icon_mdl.init(ckID_("catcake_icon.pxm"), ckDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_icon_mdl.setLightSetID(ckDrawMgr::DEFAULT_LIGHT_SET_ID);
    m_icon_mdl.getRootDraw()->local().trans.set(-270.0f + m_zombie_no * 80.0f, -195.0f);
    m_icon_mdl.getRootDraw()->local() = m_icon_mdl.getRootDraw()->local().rotateY_s32(m_zombie_no * 15);
}


void Zombie::onUpdate()
{
    if (m_state == STATE_WALK && ckKeyMgr::isPressed(ckKeyMgr::KEY_LBUTTON))
    {
        ckScr* scr = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        r32 mouse_x = scr->framebufferXToScreenX(ckKeyMgr::getMouseX());
        r32 mouse_y = scr->framebufferYToScreenY(ckKeyMgr::getMouseY());

        ckVec hit_to(mouse_x, mouse_y, -scr->getFocusDist());
        hit_to *= 10.0f;
        hit_to = hit_to.toGlobalFrom(scr->view());

        ckCdt::Ray hit_ray;
        hit_ray.setPos(scr->view().trans, hit_to);

        ckCdt::Box box;
        box.setSize(64.0f, 128.0f, 64.0f);
        box.setWorld(m_zombie_mdl.getRootDraw()->local());

        ckVec hit_pos;

        if (ckCdt::intersect(&hit_pos, hit_ray, box))
        {
            m_zombie_mot.play(&m_zombie_mdl, 2, ckMot::PLAY_KEEP, 1.0f, 5);

            m_state = STATE_FALL_DOWN;
            m_state_cntr = 100;

            extern void newBlood(const ckVec& pos);

            for (s32 i = 0; i < 5; i++)
            {
                newBlood(hit_pos);
            }
        }
    }

    if (m_state_cntr > 0)
    {
        m_state_cntr--;

        if (m_state_cntr == 0)
        {
            switch (m_state)
            {
            case STATE_FALL_DOWN:
                m_state = STATE_GET_UP;
                m_state_cntr = 100;
                m_zombie_mot.play(&m_zombie_mdl, 0, ckMot::PLAY_LOOP, 0.1f, 10);
                break;

            case STATE_GET_UP:
                m_state = STATE_WALK;
                m_zombie_mot.play(&m_zombie_mdl, 1, ckMot::PLAY_LOOP, 1.0f, 30);
                break;

            default:
                break;
            }
        }
    }

    m_icon_mdl.getRootDraw()->local() = m_icon_mdl.getRootDraw()->local().rotateY_s32(1);

    if (m_state == STATE_WALK)
    {
        if (m_zombie_mdl.getRootDraw()->local().trans.z > m_aim_z)
        {
            m_zombie_mdl.getRootDraw()->local().trans.z -= 0.3f;
        }

        m_icon_mdl.getRootDraw()->local().trans.y = ckMath::interp(m_icon_mdl.getRootDraw()->local().trans.y, -195.0f, 0.15f);
    }
    else
    {
        m_zombie_mdl.getRootDraw()->local().trans.z += 0.3f;

        m_icon_mdl.getRootDraw()->local().trans.y = ckMath::interp(m_icon_mdl.getRootDraw()->local().trans.y, -300.0f, 0.15f);
    }

    m_zombie_mot.update(&m_zombie_mdl);
}
