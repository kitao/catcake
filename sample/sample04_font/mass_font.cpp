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


class MassFont : public ckTask
{
public:
    MassFont();

private:
    static const u16 BASE_FONT_SIZE = 16;
    static const u16 GRID_SIZE = 12;

    void initMassFont();
    virtual void onUpdate();

    ckFont m_mass_font;
    ckPrim m_mass_prim;
};


void newMassFont()
{
    ckNewTask(MassFont);
}


MassFont::MassFont() : ckTask(ORDER_ZERO)
{
    initMassFont();
}


void MassFont::initMassFont()
{
    const char str[2] =
    {
        static_cast<char>(ckMath::rand('A', 'Z')), '\0'
    };

    ckDrawMgr::setFont(ckID_("stonsans.ttf"), 0);
    ckDrawMgr::setFontSize(BASE_FONT_SIZE);

    ckFont master_font;
    master_font.init(BASE_FONT_SIZE, BASE_FONT_SIZE);

    u16 base_font_width = master_font.drawString(0, 0, str);
    u16 base_font_height = master_font.getHeight();

    ckTex* tex = ckDrawMgr::getTexture(master_font.getTextureID());
    const u8* image = static_cast<const u8*>(tex->getImage());

    m_mass_font.init(base_font_width * GRID_SIZE, base_font_height * GRID_SIZE);

    for (s32 i = 0; i < base_font_height; i++)
    {
        for (s32 j = 0; j < base_font_width; j++)
        {
            if (*(image + i * BASE_FONT_SIZE + j) > 128)
            {
                m_mass_font.drawString(j * GRID_SIZE, i * GRID_SIZE, str);
            }
        }
    }

    m_mass_prim.init(ckPrim::MODE_TRIANGLE_FAN, 4, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_mass_prim.setTextureID(m_mass_font.getTextureID());
    m_mass_prim.setPreset_defaultBlendAdd();

    r32 mass_prim_width = static_cast<r32>(base_font_width * GRID_SIZE);
    r32 mass_prim_height = static_cast<r32>(base_font_height * GRID_SIZE);

    m_mass_prim.setDataRect(0, ckVec::ZERO, mass_prim_width, mass_prim_height, //
        ckCol(ckMath::rand(64, 255), ckMath::rand(64, 255), ckMath::rand(64, 255)), 0.0f, 0.0f, 1.0f, 1.0f);

    m_mass_prim.local() = m_mass_prim.local().rotateY_s32(ckMath::rand(0, 360));
    m_mass_prim.local().trans.set( //
        ckMath::rand(-200.0f, 200.0f, 20.0f), ckMath::rand(-150.0f, 150.0f, 20.0f), ckMath::rand(-2000.0f, 1000.0f, 100.0f));

    m_mass_prim.setColor(ckCol(255, 255, 255, 0));
}


void MassFont::onUpdate()
{
    m_mass_prim.local() = m_mass_prim.local().rotateY_s32(1);
    m_mass_prim.local().trans.z += 2.0f;

    ckCol col = m_mass_prim.getColor();

    if (col.a < 160)
    {
        col.a += 4;
        m_mass_prim.setColor(col);
    }

    if (m_mass_prim.local().trans.z > 1000.0f)
    {
        initMassFont();
    }
}
