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


class Waterdrop : public ckTask
{
public:
    Waterdrop(r32 x, r32 y);

private:
    void resetParam(r32 x, r32 y);
    virtual void onUpdate();

    ckPrim m_water_prim;
    ckRend_Shader m_water_rend;

    r32 m_speed;
    r32 m_aim_rate;
};


void newWaterdrop(r32 x, r32 y)
{
    ckNewTask(Waterdrop)(x, y);
}


Waterdrop::Waterdrop(r32 x, r32 y) : ckTask(ORDER_ZERO)
{
    m_aim_rate = 0.6f;

    m_water_prim.init(ckPrim::MODE_TRIANGLE_FAN, 4, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_water_prim.setTextureID(ckID_("bg_512x512.png"));
    m_water_prim.setBlendMode(ckDraw::BLEND_HALF, true);

    m_water_rend.init(&m_water_prim, ckID_("refract.shd"));
    m_water_rend.set2ndTextureID(ckID_("waterdrop_64x64.png"));

    m_water_rend.uniform(0) = m_aim_rate;

    m_water_rend.attrib(0, 0) = 0.0f;
    m_water_rend.attrib(0, 1) = 0.0f;

    m_water_rend.attrib(1, 0) = 0.0f;
    m_water_rend.attrib(1, 1) = 1.0f;

    m_water_rend.attrib(2, 0) = 1.0f;
    m_water_rend.attrib(2, 1) = 1.0f;

    m_water_rend.attrib(3, 0) = 1.0f;
    m_water_rend.attrib(3, 1) = 0.0f;

    resetParam(x, y);
}


void Waterdrop::resetParam(r32 x, r32 y)
{
    r32 scale = ckMath::rand(0.5f, 1.0f, 0.1f);

    m_water_prim.setDataRect(0, ckVec::ZERO, 48.0f * scale, 64.0f * scale, ckCol::FULL, 0.0f, 0.0f, 1.0f, 1.0f);
    m_water_prim.local().trans.set(x, y, scale);

    m_speed = ckMath::rand(2.0f, 4.0f, 0.1f) * scale;
}


void Waterdrop::onUpdate()
{
    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_R))
    {
        m_aim_rate = -m_aim_rate;
    }

    if (m_water_rend.uniform(0) < m_aim_rate)
    {
        m_water_rend.uniform(0) += 0.01f;

        if (m_water_rend.uniform(0) > m_aim_rate)
        {
            m_water_rend.uniform(0) = m_aim_rate;
        }
    }
    else if (m_water_rend.uniform(0) > m_aim_rate)
    {
        m_water_rend.uniform(0) -= 0.01f;

        if (m_water_rend.uniform(0) < m_aim_rate)
        {
            m_water_rend.uniform(0) = m_aim_rate;
        }
    }

    m_water_prim.local().trans.y -= m_speed;

    if (m_water_prim.local().trans.y < -320)
    {
        resetParam(ckMath::rand(-320.0f, 320.0f, 1.0f), ckMath::rand(300.0f, 340.0f, 1.0f));
    }
}
