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


const r32 NODE_WIDTH = 8.0f;
const r32 NODE_HEIGHT = 35.0f;
const r32 GROW_SPEED = 0.08f;
const r32 OFFSET_SPEED = 0.004f;


class Coral : public pgTask
{
public:
    Coral(const pgMat& world, u8 type, u8 branch_num, u16 survival_time);
    Coral(Coral* parent, const pgMat& world, u8 type, u8 branch_num);

private:
    void init(Coral* parent, const pgMat& world, u8 type, u8 branch_num, u16 survival_time);
    virtual void onUpdate();

    pgScr* m_scr;
    Coral* m_parent;
    u8 m_type;
    u8 m_branch_num;

    bool m_is_live;
    u16 m_survival_time;
    r32 m_grow_rate;
    r32 m_v_offset;
    r32 m_fall_speed;

    pgPrim m_coral_prim;
};


void newCoral(r32 x, r32 z, u8 type, u8 branch_num, u16 survival_time)
{
    pgNewTask(Coral)(pgMat::UNIT.translate(x, 0.0f, z), type, branch_num, survival_time);
}


Coral::Coral(const pgMat& world, u8 type, u8 branch_num, u16 survival_time) : pgTask(ORDER_ZERO)
{
    init(NULL, world, type, branch_num, survival_time);
}


Coral::Coral(Coral* parent, const pgMat& world, u8 type, u8 branch_num) : pgTask(parent)
{
    init(parent, world, type, branch_num, 0);
}


void Coral::init(Coral* parent, const pgMat& world, u8 type, u8 branch_num, u16 survival_time)
{
    m_scr = pgDrawMgr::getScreen(pgDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_parent = parent;
    m_type = type;
    m_branch_num = branch_num;

    m_is_live = true;
    m_survival_time = survival_time;
    m_grow_rate = GROW_SPEED;
    m_v_offset = 0.0f;
    m_fall_speed = 0.0f;

    m_coral_prim.init(pgPrim::MODE_TRIANGLE_STRIP, 4, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_coral_prim.setTextureID(pgID_("coral_64x128.png"));
    m_coral_prim.local() = world;

    r32 u1 = 0.25f * m_type;
    r32 u2 = u1 + 0.25f;

    m_coral_prim.setDataUV(0, u1, 0.0f);
    m_coral_prim.setDataUV(1, u2, 0.0f);
    m_coral_prim.setDataUV(2, u1, 0.0f);
    m_coral_prim.setDataUV(3, u2, 0.0f);
}


void Coral::onUpdate()
{
    if (m_survival_time > 0)
    {
        m_survival_time--;
    }

    if (m_is_live && ((m_parent && !m_parent->m_is_live) || (!m_parent && m_survival_time == 0)))
    {
        m_is_live = false;
        m_fall_speed = pgMath::rand(2.0f, 3.0f, 0.1f);

        m_coral_prim.setPreset_defaultBlendHalf();
    }

    if (m_is_live)
    {
        m_v_offset += OFFSET_SPEED;

        if (m_v_offset > 0.5f)
        {
            m_v_offset -= 0.5f;
        }

        m_coral_prim.dataV(0) = m_coral_prim.dataV(1) = m_v_offset + 0.5f - m_grow_rate / 2.0f;
        m_coral_prim.dataV(2) = m_coral_prim.dataV(3) = m_v_offset + 0.5f;
    }
    else
    {
        if (m_coral_prim.local().trans.y > 0.0f)
        {
            m_coral_prim.local().trans.y -= m_fall_speed;
        }

        if (m_coral_prim.dataCol(0).a >= 2)
        {
            for (s32 i = 0; i < 4; i++)
            {
                m_coral_prim.dataCol(i).a -= 2;
            }

            if (!m_parent && m_coral_prim.dataCol(0).a < 2)
            {
                pgDeleteTask(this);
                return;
            }
        }
    }

    if (m_is_live && m_grow_rate < 1.0f)
    {
        m_grow_rate = pgMath::min(m_grow_rate + GROW_SPEED, 1.0f);

        if (m_branch_num > 0 && m_grow_rate >= 1.0f)
        {
            pgMat mat = pgMat::UNIT;
            mat.trans.set(0.0f, NODE_HEIGHT * m_grow_rate - 2.0f, 0.0f);

            pgMat mat1 = mat.rotateY_s32(pgMath::rand(10, 170)).rotateZ_s32(pgMath::rand(10, 30)).rotateX_s32(pgMath::rand(10, 30)).toGlobalFrom(m_coral_prim.local());
            pgMat mat2 = mat.rotateY_s32(pgMath::rand(10, -170)).rotateZ_s32(pgMath::rand(10, 30)).rotateX_s32(pgMath::rand(10, 30)).toGlobalFrom(m_coral_prim.local());

            if (mat1.y_axis.dot(pgVec::Y_UNIT) > 0.0f)
            {
                pgNewTask(Coral)(this, mat1, m_type, m_branch_num - 1);
            }

            if (mat2.y_axis.dot(pgVec::Y_UNIT) > 0.0f)
            {
                pgNewTask(Coral)(this, mat2, m_type, m_branch_num - 1);
            }
        }
    }

    pgVec pos1 = m_coral_prim.local().trans;
    pgVec pos2 = pos1 + m_coral_prim.local().y_axis;

    pgVec vec_x = m_scr->calcVisibleVector(pos1, pos2).toLocalOf_noTrans(m_coral_prim.local()) * (NODE_WIDTH / 2.0f);
    pgVec vec_y(0.0f, NODE_HEIGHT * m_grow_rate, 0.0f);

    m_coral_prim.dataPos(0) = -vec_x + vec_y;
    m_coral_prim.dataPos(1) = vec_x + vec_y;
    m_coral_prim.dataPos(2) = -vec_x;
    m_coral_prim.dataPos(3) = vec_x;
}
