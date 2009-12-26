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


class HelloCatcake : public ckTask
{
public:
    HelloCatcake();

private:
    virtual void onUpdate();

    ckFont m_text_font;
    ckSprt m_text_sprt;
    ckPrim m_logo_prim;
};


void newHelloCatcake()
{
    ckNewTask(HelloCatcake);
}


HelloCatcake::HelloCatcake() : ckTask(ORDER_ZERO)
{
    const char* text = "Hello, Catcake!";

    ckDrawMgr::setFont(ckID_("stonsans.ttf"), 0);
    ckDrawMgr::setFontSize(40);

    m_text_font.init(ckDrawMgr::calcFontDrawWidth(text), ckDrawMgr::getFontSize());
    m_text_font.drawString(0, 0, text);

    m_text_sprt.init(1, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_text_sprt.setBlendMode(ckDraw::BLEND_HALF, true);
    m_text_sprt.setTextureID(m_text_font.getTextureID());
    m_text_sprt.setDataSize(0, m_text_font.getWidth(), m_text_font.getHeight());
    m_text_sprt.local().trans.set(0.0f, 90.0f, 0.0f);

    m_logo_prim.init(ckPrim::MODE_TRIANGLE_FAN, 4, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_logo_prim.setTextureID(ckID_("catcake_logo_71x14.png"));
    m_logo_prim.setDrawFlag(ckDraw::FLAG_BILINEAR, false);
    m_logo_prim.setDataRect(0, ckVec::ZERO, 71.0f * 4.0f, 14.0f * 4.0f, ckCol::FULL, 0.0f, 0.0f, 1.0f, 1.0f);
    m_logo_prim.local().trans.set(0.0f, -70.0f, 0.0f);
}


void HelloCatcake::onUpdate()
{
    m_logo_prim.local() = m_logo_prim.local().rotateY_s32(3).rotateX_s32(1);
}
