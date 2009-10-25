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
#include "collision_target.h"


class CollisionChecker : public pgTask
{
public:
    CollisionChecker();

private:
    enum MouseState
    {
        STATE_NORMAL, //
        STATE_DRAG_L, //
        STATE_DRAG_M, //
        STATE_DRAG_R
    };

    void resetPos();
    virtual void onUpdate();

    pgScr* m_scr;
    pgMat m_view_rot;
    r32 m_view_dist;

    r32 m_mouse_x;
    r32 m_mouse_y;
    MouseState m_mouse_state;
    u8 m_drag_no;

    pgMat m_tgt_world[2];
    pgVec m_tgt_size[2];
    CollisionTarget::Target1 m_tgt1;
    CollisionTarget::Target2 m_tgt2;

    bool m_is_push_back;
};


void newCollisionChecker()
{
    pgNewTask(CollisionChecker);
}


CollisionChecker::CollisionChecker() : pgTask(ORDER_ZERO)
{
    m_scr = pgDrawMgr::getScreen(pgDrawMgr::DEFAULT_3D_SCREEN_ID);

    m_tgt1.type = CollisionTarget::TARGET1_SPHERE;
    m_tgt2.type = CollisionTarget::TARGET2_BOX;

    m_is_push_back = false;

    resetPos();
}


void CollisionChecker::resetPos()
{
    m_view_rot = pgMat::UNIT;
    m_view_dist = m_scr->getFocusDist();

    m_mouse_state = STATE_NORMAL;

    m_tgt_world[0] = pgMat::UNIT.translate(-180.0f, 0.0f, 0.0f);
    m_tgt_world[1] = pgMat::UNIT.translate(180.0f, 0.0f, 0.0f);

    m_tgt_size[0].set(200.0f, 200.0f, 200.0f);
    m_tgt_size[1].set(200.0f, 200.0f, 200.0f);
}


void CollisionChecker::onUpdate()
{
    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_Q))
    {
        pgEndPogolyn();
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_A))
    {
        m_tgt1.type = static_cast<CollisionTarget::Target1Type>((m_tgt1.type + 1) % CollisionTarget::TARGET1_NUM);
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_B))
    {
        m_tgt2.type = static_cast<CollisionTarget::Target2Type>((m_tgt2.type + 1) % CollisionTarget::TARGET2_NUM);
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_P))
    {
        m_is_push_back = !m_is_push_back;
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_R))
    {
        resetPos();
    }

    if (pgSysMgr::isFramebufferSizeChanged())
    {
        m_scr->setAreaInFramebuffer(0, 0, pgSysMgr::getFramebufferWidth(), pgSysMgr::getFramebufferHeight());
        m_scr->setViewSize(pgSysMgr::getFramebufferWidth(), pgSysMgr::getFramebufferHeight());
    }

    r32 last_mouse_x = m_mouse_x;
    r32 last_mouse_y = m_mouse_y;

    m_mouse_x = m_scr->framebufferXToScreenX(pgKeyMgr::getMouseX());
    m_mouse_y = m_scr->framebufferYToScreenY(pgKeyMgr::getMouseY());

    /*
        check hit
    */
    pgVec hit_to(m_mouse_x, m_mouse_y, -m_scr->getFocusDist());
    hit_to *= 10.0f;
    hit_to = hit_to.toGlobalFrom(m_scr->view());

    pgCdt::Ray hit_ray;
    hit_ray.setPos(m_scr->view().trans, hit_to);

    u8 hit_no = CollisionTarget::checkTargetHit(m_tgt1, hit_ray) ? 1 : (CollisionTarget::checkTargetHit(m_tgt2, hit_ray) ? 2 : 0);

    /*
        drag object/camera
    */
    bool is_l_pressed = pgKeyMgr::isPressed(pgKeyMgr::KEY_LBUTTON);
    bool is_m_pressed = pgKeyMgr::isPressed(pgKeyMgr::KEY_MBUTTON);
    bool is_r_pressed = pgKeyMgr::isPressed(pgKeyMgr::KEY_RBUTTON);

    if (is_l_pressed || is_m_pressed || is_r_pressed)
    {
        last_mouse_x = m_mouse_x;
        last_mouse_y = m_mouse_y;

        m_mouse_state = is_l_pressed ? STATE_DRAG_L : (is_m_pressed ? STATE_DRAG_M : STATE_DRAG_R);
        m_drag_no = hit_no;
    }

    if (m_mouse_state != STATE_NORMAL)
    {
        r32 drag_x = m_mouse_x - last_mouse_x;
        r32 drag_y = m_mouse_y - last_mouse_y;

        pgMat* tgt_world = (m_drag_no > 0) ? &m_tgt_world[m_drag_no - 1] : NULL;
        pgVec* tgt_size = (m_drag_no > 0) ? &m_tgt_size[m_drag_no - 1] : NULL;

        if (m_mouse_state == STATE_DRAG_L)
        {
            if (pgKeyMgr::isOn(pgKeyMgr::KEY_LBUTTON))
            {
                if (m_drag_no > 0)
                {
                    r32 rel_z = tgt_world->trans.toLocalOf(m_scr->view()).z;

                    tgt_world->trans += (m_view_rot.x_axis * drag_x + m_view_rot.y_axis * drag_y) * -rel_z / m_scr->getFocusDist();
                }
                else
                {
                    pgVec vec = (m_view_rot.x_axis * drag_x + m_view_rot.y_axis * drag_y) * m_view_dist / m_scr->getFocusDist();

                    m_tgt_world[0].trans += vec;
                    m_tgt_world[1].trans += vec;
                }
            }
            else
            {
                m_mouse_state = STATE_NORMAL;
            }
        }
        else if (m_mouse_state == STATE_DRAG_M)
        {
            if (pgKeyMgr::isOn(pgKeyMgr::KEY_MBUTTON))
            {
                if (m_drag_no > 0)
                {
                    tgt_size->x = pgMath::clamp(tgt_size->x + drag_x, 10.0f, 1000.0f);
                    tgt_size->y = pgMath::clamp(tgt_size->y + drag_y, 10.0f, 1000.0f);
                    tgt_size->z = pgMath::clamp(tgt_size->z + drag_y / 2.0f, 10.0f, 1000.0f);
                }
                else
                {
                    m_view_dist = pgMath::max(m_view_dist + drag_y * -5.0f, 500.0f);
                }
            }
            else
            {
                m_mouse_state = STATE_NORMAL;
            }
        }
        else if (m_mouse_state == STATE_DRAG_R)
        {
            if (pgKeyMgr::isOn(pgKeyMgr::KEY_RBUTTON))
            {
                if (m_drag_no > 0)
                {
                    pgVec trans = tgt_world->trans;
                    *tgt_world = tgt_world->toLocalOf_noTrans(m_view_rot);
                    *tgt_world = tgt_world->toGlobalFrom_noTrans(m_view_rot.rotateX_r32(-drag_y).rotateY_r32(drag_x));
                    tgt_world->trans = trans;
                }
                else
                {
                    m_view_rot = m_view_rot.rotateX_r32(drag_y).rotateY_r32(-drag_x);
                }
            }
            else
            {
                m_mouse_state = STATE_NORMAL;
            }
        }
    }

    m_view_dist = pgMath::max(m_view_dist - pgKeyMgr::getMouseWheel() * 100.0f, 500.0f);

    m_scr->view() = m_view_rot;
    m_scr->view().trans = m_view_rot.z_axis * m_view_dist;

    CollisionTarget::updateTarget(&m_tgt1, m_tgt_world[0], m_tgt_size[0]);
    CollisionTarget::updateTarget(&m_tgt2, m_tgt_world[1], m_tgt_size[1]);

    /*
        check collision
    */
    if (m_tgt1.type == CollisionTarget::TARGET1_RAY)
    {
        pgVec pos;

        if (CollisionTarget::intersect(&pos, m_tgt1, m_tgt2))
        {
            CollisionTarget::drawPos(pos, pgCol(0, 255, 255));

            if (m_is_push_back)
            {
                m_tgt_world[0].trans += pos - m_tgt1.ray.getTo();
                CollisionTarget::updateTarget(&m_tgt1, m_tgt_world[0], m_tgt_size[0]);
            }
        }
    }
    else
    {
        pgCdt::CdtInfo cdt_info;

        if (CollisionTarget::collide(&cdt_info, m_tgt1, m_tgt2))
        {
            CollisionTarget::drawPos(cdt_info.pos, pgCol(0, 255, 255));
            pgDbgMgr::drawLine(cdt_info.pos, cdt_info.pos + cdt_info.back_dir * cdt_info.back_dist, pgCol(255, 0, 255), pgDrawMgr::DEFAULT_3D_SCREEN_ID);

            if (m_is_push_back)
            {
                m_tgt_world[0].trans += cdt_info.back_dir * cdt_info.back_dist;
                CollisionTarget::updateTarget(&m_tgt1, m_tgt_world[0], m_tgt_size[0]);
            }
        }
    }

    /*
        draw objects
    */
    pgCol tgt1_col = ((m_mouse_state == STATE_NORMAL && hit_no == 1) || (m_mouse_state != STATE_NORMAL && m_drag_no == 1)) ? pgCol(192, 64, 0, 128) : pgCol(0, 255, 128, 64);
    pgCol tgt2_col = ((m_mouse_state == STATE_NORMAL && hit_no == 2) || (m_mouse_state != STATE_NORMAL && m_drag_no == 2)) ? pgCol(255, 64, 0, 128) : pgCol(128, 128, 255, 64);
    pgCol line_col = m_is_push_back ? pgCol(255, 192, 64, 255) : pgCol(255, 255, 255, 128);
    pgCol aabb_col(0, 255, 0, 48);

    CollisionTarget::drawTarget(m_tgt1, tgt1_col, line_col, aabb_col);
    CollisionTarget::drawTarget(m_tgt2, tgt2_col, line_col, aabb_col);
    CollisionTarget::drawAxis(pgCol(255, 128, 128, 64));
}
