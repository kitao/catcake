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
