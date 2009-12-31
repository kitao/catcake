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


class FontDraw : public ckTask
{
public:
    FontDraw();

private:
    virtual void onUpdate();

    u64 m_start_time;
    ckScr* m_scr3d;
    ckMat m_view1;
    ckMat m_view2;
    bool m_is_view2;

    ckFont m_text_font1;
    ckSprt m_text_sprt1;

    ckFont m_text_font2;
    ckSprt m_text_sprt2;

    ckFont m_time_font;
    ckSprt m_time_sprt;
};


void newFontDraw()
{
    ckNewTask(FontDraw);
}


FontDraw::FontDraw() : ckTask(ORDER_ZERO)
{
    m_start_time = ckSysMgr::getUsecTime();
    m_scr3d = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_view1 = m_scr3d->view();
    m_view2 = m_view1.rotateY_s32(90).rotateX_s32(-30).translate(800.0f, 10.0f, 1500.0f);
    m_is_view2 = false;

    /*
        set up m_text_font1 and m_text_sprt1
    */
    const char* text1 = "Catcake Font-drawing Sample";

    ckDrawMgr::setFont(ckID_("stonsans.ttf"), 0);
    ckDrawMgr::setFontSize(48);

    m_text_font1.init(ckDrawMgr::calcFontDrawWidth(text1), ckDrawMgr::getFontSize());
    m_text_font1.drawString(0, 0, text1);

    m_text_sprt1.init(2, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_text_sprt1.setTextureID(m_text_font1.getTextureID());
    m_text_sprt1.setBlendMode(ckDraw::BLEND_HALF, true);

    m_text_sprt1.dataPos(0).set(1.0f, 99.0f, 0.0f);
    m_text_sprt1.setDataSize(0, m_text_font1.getWidth(), m_text_font1.getHeight());

    m_text_sprt1.dataPos(1).set(0.0f, 100.0f, 0.0f);
    m_text_sprt1.dataCol(1).set(255, 64, 128);
    m_text_sprt1.setDataSize(1, m_text_font1.getWidth(), m_text_font1.getHeight());

    /*
        set up m_text_font2 and m_text_sprt2
    */
    wchar_t text2[64];
    ckSysMgr::swprintf(text2, sizeof(text2) / sizeof(wchar_t), L"You can use %ls and %ls.", L"ASCII", L"UNICODE");

#ifdef WIN32
    ckDrawMgr::setFont(ckID_("msgothic.ttc"), 0);
#endif
    ckDrawMgr::setFontSize(36);

    m_text_font2.init(ckDrawMgr::calcFontDrawWidth(text2), ckDrawMgr::getFontSize());
    m_text_font2.drawString(0, 0, text2);

    m_text_sprt2.init(2, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_text_sprt2.setTextureID(m_text_font2.getTextureID());
    m_text_sprt2.setBlendMode(ckDraw::BLEND_HALF, true);

    m_text_sprt2.dataPos(0).set(1.0f, 29.0f, 0.0f);
    m_text_sprt2.dataCol(0).set(192, 192, 255);
    m_text_sprt2.setDataSize(0, m_text_font2.getWidth(), m_text_font2.getHeight());

    m_text_sprt2.dataPos(1).set(0.0f, 30.0f, 0.0f);
    m_text_sprt2.dataCol(1).set(64, 128, 255);
    m_text_sprt2.setDataSize(1, m_text_font2.getWidth(), m_text_font2.getHeight());

    /*
        set up m_time_font and m_time_sprt
    */
    m_time_font.init(256, 100);

    m_time_sprt.init(2, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_time_sprt.setTextureID(m_time_font.getTextureID());
    m_time_sprt.setBlendMode(ckDraw::BLEND_HALF, true);

    m_time_sprt.setDataSize(0, 256, 100);

    m_time_sprt.dataPos(1).set(0.0f, -45.0f);
    m_time_sprt.dataCol(1).set(128, 128, 128, 128);
    m_time_sprt.setDataSize(1, 256, 80);
    m_time_sprt.setDataUV(1, 0.0f, 1.0f, 1.0f, 0.0f);

    m_time_sprt.local().trans.set(20.0f, -80.0f);
}


void FontDraw::onUpdate()
{
    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_F))
    {
        ckSysMgr::toggleFullScreen(640, 480);
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_Q))
    {
        ckEndCatcake();
    }

    r32 time = static_cast<r32>(ckSysMgr::getUsecTime() - m_start_time) / 1000000.0f;
    time -= (static_cast<u32>(time) / 100) * 100;

    ckDrawMgr::setFont(ckID_("stonsans.ttf"), 0);
    ckDrawMgr::setFontSize(100);

    m_time_font.clearImage();
    m_time_font.drawString(0, 0, "%2.2f", time);

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_V))
    {
        m_is_view2 = !m_is_view2;
    }

    if (m_is_view2)
    {
        m_scr3d->view() = m_scr3d->view().slerp(m_view2, 0.05f);
    }
    else
    {
        m_scr3d->view() = m_scr3d->view().slerp(m_view1, 0.05f);
    }
}
