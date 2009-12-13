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
