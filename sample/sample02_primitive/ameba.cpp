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


class Ameba : public pgTask
{
public:
    Ameba(r32 x, r32 y, pgCol color);

private:
    static const u16 CIRCLE_VERT_NUM = 36;
    static const u16 AMEBA_VERT_NUM = CIRCLE_VERT_NUM + 2;
    static const u16 ONE_VERT_DEG = 360 / CIRCLE_VERT_NUM;

    void setAmebaPos(s32 phase);
    virtual void onUpdate();

    pgPrim m_ameba_prim;
    s32 m_phase;
};


void newAmeba(r32 x, r32 y, pgCol color)
{
    pgNewTask(Ameba)(x, y, color);
}


Ameba::Ameba(r32 x, r32 y, pgCol color) : pgTask(ORDER_ZERO)
{
    m_ameba_prim.init(pgPrim::MODE_TRIANGLE_FAN, AMEBA_VERT_NUM, pgDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_ameba_prim.setPreset_defaultBlendHalf();

    setAmebaPos(m_phase);

    m_ameba_prim.dataCol(0).set(color.r, color.g, color.b, 128);

    for (s32 i = 1; i < AMEBA_VERT_NUM; i++)
    {
        m_ameba_prim.dataCol(i).set(color.r, color.g, color.b, 96);
    }

    m_ameba_prim.local().trans.set(x, y);

    m_phase = 0;
}


void Ameba::setAmebaPos(s32 phase)
{
    m_ameba_prim.dataPos(0) = pgVec::ZERO;

    for (s32 i = 1; i < AMEBA_VERT_NUM; i++)
    {
        pgVec dir(pgMath::cos_s32((i - 1) * ONE_VERT_DEG), pgMath::sin_s32((i - 1) * ONE_VERT_DEG));
        r32 rad = pgMath::sin_s32((i - 1) * 60) * pgMath::sin_s32(phase) * 8.0f + 64.0f;

        m_ameba_prim.dataPos(i) = dir * rad;
    }
}


void Ameba::onUpdate()
{
    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_P))
    {
        if (m_ameba_prim.getPrimMode() == pgPrim::MODE_TRIANGLE_FAN)
        {
            m_ameba_prim.setPrimMode(pgPrim::MODE_LINE_STRIP);
        }
        else
        {
            m_ameba_prim.setPrimMode(pgPrim::MODE_TRIANGLE_FAN);
        }
    }

    m_phase += 4;
    setAmebaPos(m_phase);
}
