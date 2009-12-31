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
#include "collision_target.h"


class CollisionChecker : public ckTask
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

    ckScr* m_scr;
    ckMat m_view_rot;
    r32 m_view_dist;

    r32 m_mouse_x;
    r32 m_mouse_y;
    MouseState m_mouse_state;
    u8 m_drag_no;

    ckMat m_tgt_world[2];
    ckVec m_tgt_size[2];
    CollisionTarget::Target1 m_tgt1;
    CollisionTarget::Target2 m_tgt2;

    bool m_is_push_back;
};


void newCollisionChecker()
{
    ckNewTask(CollisionChecker);
}


CollisionChecker::CollisionChecker() : ckTask(ORDER_ZERO)
{
    m_scr = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);

    m_tgt1.type = CollisionTarget::TARGET1_SPHERE;
    m_tgt2.type = CollisionTarget::TARGET2_BOX;

    m_is_push_back = false;

    resetPos();
}


void CollisionChecker::resetPos()
{
    m_view_rot = ckMat::UNIT;
    m_view_dist = m_scr->getFocusDist();

    m_mouse_state = STATE_NORMAL;

    m_tgt_world[0] = ckMat::UNIT.translate(-180.0f, 0.0f, 0.0f);
    m_tgt_world[1] = ckMat::UNIT.translate(180.0f, 0.0f, 0.0f);

    m_tgt_size[0].set(200.0f, 200.0f, 200.0f);
    m_tgt_size[1].set(200.0f, 200.0f, 200.0f);
}


void CollisionChecker::onUpdate()
{
    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_Q))
    {
        ckEndCatcake();
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_A))
    {
        m_tgt1.type = static_cast<CollisionTarget::Target1Type>((m_tgt1.type + 1) % CollisionTarget::TARGET1_NUM);
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_B))
    {
        m_tgt2.type = static_cast<CollisionTarget::Target2Type>((m_tgt2.type + 1) % CollisionTarget::TARGET2_NUM);
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_P))
    {
        m_is_push_back = !m_is_push_back;
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_R))
    {
        resetPos();
    }

    if (ckSysMgr::isFramebufferSizeChanged())
    {
        m_scr->setAreaInFramebuffer(0, 0, ckSysMgr::getFramebufferWidth(), ckSysMgr::getFramebufferHeight());
        m_scr->setViewSize(ckSysMgr::getFramebufferWidth(), ckSysMgr::getFramebufferHeight());
    }

    r32 last_mouse_x = m_mouse_x;
    r32 last_mouse_y = m_mouse_y;

    m_mouse_x = m_scr->framebufferXToScreenX(ckKeyMgr::getMouseX());
    m_mouse_y = m_scr->framebufferYToScreenY(ckKeyMgr::getMouseY());

    /*
        check hit
    */
    ckVec hit_to(m_mouse_x, m_mouse_y, -m_scr->getFocusDist());
    hit_to *= 10.0f;
    hit_to = hit_to.toGlobalFrom(m_scr->view());

    ckCdt::Ray hit_ray;
    hit_ray.setPos(m_scr->view().trans, hit_to);

    u8 hit_no = CollisionTarget::checkTargetHit(m_tgt1, hit_ray) ? 1 : (CollisionTarget::checkTargetHit(m_tgt2, hit_ray) ? 2 : 0);

    /*
        drag object/camera
    */
    bool is_l_pressed = ckKeyMgr::isPressed(ckKeyMgr::KEY_LBUTTON);
    bool is_m_pressed = ckKeyMgr::isPressed(ckKeyMgr::KEY_MBUTTON);
    bool is_r_pressed = ckKeyMgr::isPressed(ckKeyMgr::KEY_RBUTTON);

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

        ckMat* tgt_world = (m_drag_no > 0) ? &m_tgt_world[m_drag_no - 1] : NULL;
        ckVec* tgt_size = (m_drag_no > 0) ? &m_tgt_size[m_drag_no - 1] : NULL;

        if (m_mouse_state == STATE_DRAG_L)
        {
            if (ckKeyMgr::isOn(ckKeyMgr::KEY_LBUTTON))
            {
                if (m_drag_no > 0)
                {
                    r32 rel_z = tgt_world->trans.toLocalOf(m_scr->view()).z;

                    tgt_world->trans += (m_view_rot.x_axis * drag_x + m_view_rot.y_axis * drag_y) * -rel_z / m_scr->getFocusDist();
                }
                else
                {
                    ckVec vec = (m_view_rot.x_axis * drag_x + m_view_rot.y_axis * drag_y) * m_view_dist / m_scr->getFocusDist();

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
            if (ckKeyMgr::isOn(ckKeyMgr::KEY_MBUTTON))
            {
                if (m_drag_no > 0)
                {
                    tgt_size->x = ckMath::clamp(tgt_size->x + drag_x, 10.0f, 1000.0f);
                    tgt_size->y = ckMath::clamp(tgt_size->y + drag_y, 10.0f, 1000.0f);
                    tgt_size->z = ckMath::clamp(tgt_size->z + drag_y / 2.0f, 10.0f, 1000.0f);
                }
                else
                {
                    m_view_dist = ckMath::max(m_view_dist + drag_y * -5.0f, 500.0f);
                }
            }
            else
            {
                m_mouse_state = STATE_NORMAL;
            }
        }
        else if (m_mouse_state == STATE_DRAG_R)
        {
            if (ckKeyMgr::isOn(ckKeyMgr::KEY_RBUTTON))
            {
                if (m_drag_no > 0)
                {
                    ckVec trans = tgt_world->trans;
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

    m_view_dist = ckMath::max(m_view_dist - ckKeyMgr::getMouseWheel() * 100.0f, 500.0f);

    m_scr->view() = m_view_rot;
    m_scr->view().trans = m_view_rot.z_axis * m_view_dist;

    CollisionTarget::updateTarget(&m_tgt1, m_tgt_world[0], m_tgt_size[0]);
    CollisionTarget::updateTarget(&m_tgt2, m_tgt_world[1], m_tgt_size[1]);

    /*
        check collision
    */
    if (m_tgt1.type == CollisionTarget::TARGET1_RAY)
    {
        ckVec pos;

        if (CollisionTarget::intersect(&pos, m_tgt1, m_tgt2))
        {
            CollisionTarget::drawPos(pos, ckCol(0, 255, 255));

            if (m_is_push_back)
            {
                m_tgt_world[0].trans += pos - m_tgt1.ray.getTo();
                CollisionTarget::updateTarget(&m_tgt1, m_tgt_world[0], m_tgt_size[0]);
            }
        }
    }
    else
    {
        ckCdt::CdtInfo cdt_info;

        if (CollisionTarget::collide(&cdt_info, m_tgt1, m_tgt2))
        {
            CollisionTarget::drawPos(cdt_info.pos, ckCol(0, 255, 255));
            ckDbgMgr::drawLine(cdt_info.pos, cdt_info.pos + cdt_info.back_dir * cdt_info.back_dist, ckCol(255, 0, 255), ckDrawMgr::DEFAULT_3D_SCREEN_ID);

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
    ckCol tgt1_col = ((m_mouse_state == STATE_NORMAL && hit_no == 1) || (m_mouse_state != STATE_NORMAL && m_drag_no == 1)) ? ckCol(192, 64, 0, 128) : ckCol(0, 255, 128, 64);
    ckCol tgt2_col = ((m_mouse_state == STATE_NORMAL && hit_no == 2) || (m_mouse_state != STATE_NORMAL && m_drag_no == 2)) ? ckCol(255, 64, 0, 128) : ckCol(128, 128, 255, 64);
    ckCol line_col = m_is_push_back ? ckCol(255, 192, 64, 255) : ckCol(255, 255, 255, 128);
    ckCol aabb_col(0, 255, 0, 48);

    CollisionTarget::drawTarget(m_tgt1, tgt1_col, line_col, aabb_col);
    CollisionTarget::drawTarget(m_tgt2, tgt2_col, line_col, aabb_col);
    CollisionTarget::drawAxis(ckCol(255, 128, 128, 64));
}
