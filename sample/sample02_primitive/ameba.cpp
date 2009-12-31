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


class Ameba : public ckTask
{
public:
    Ameba(r32 x, r32 y, ckCol color);

private:
    static const u16 CIRCLE_VERT_NUM = 36;
    static const u16 AMEBA_VERT_NUM = CIRCLE_VERT_NUM + 2;
    static const u16 ONE_VERT_DEG = 360 / CIRCLE_VERT_NUM;

    void setAmebaPos(s32 phase);
    virtual void onUpdate();

    ckPrim m_ameba_prim;
    s32 m_phase;
};


void newAmeba(r32 x, r32 y, ckCol color)
{
    ckNewTask(Ameba)(x, y, color);
}


Ameba::Ameba(r32 x, r32 y, ckCol color) : ckTask(ORDER_ZERO)
{
    m_ameba_prim.init(ckPrim::MODE_TRIANGLE_FAN, AMEBA_VERT_NUM, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_ameba_prim.setBlendMode(ckDraw::BLEND_HALF, true);

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
    m_ameba_prim.dataPos(0) = ckVec::ZERO;

    for (s32 i = 1; i < AMEBA_VERT_NUM; i++)
    {
        ckVec dir(ckMath::cos_s32((i - 1) * ONE_VERT_DEG), ckMath::sin_s32((i - 1) * ONE_VERT_DEG));
        r32 rad = ckMath::sin_s32((i - 1) * 60) * ckMath::sin_s32(phase) * 8.0f + 64.0f;

        m_ameba_prim.dataPos(i) = dir * rad;
    }
}


void Ameba::onUpdate()
{
    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_P))
    {
        if (m_ameba_prim.getPrimMode() == ckPrim::MODE_TRIANGLE_FAN)
        {
            m_ameba_prim.setPrimMode(ckPrim::MODE_LINE_STRIP);
        }
        else
        {
            m_ameba_prim.setPrimMode(ckPrim::MODE_TRIANGLE_FAN);
        }
    }

    m_phase += 4;
    setAmebaPos(m_phase);
}
