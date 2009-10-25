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


class FontDraw : public pgTask
{
public:
    FontDraw();

private:
    virtual void onUpdate();

    u64 m_start_time;
    pgScr* m_scr3d;
    pgMat m_view1;
    pgMat m_view2;
    bool m_is_view2;

    pgFont m_text_font1;
    pgSprt m_text_sprt1;

    pgFont m_text_font2;
    pgSprt m_text_sprt2;

    pgFont m_time_font;
    pgSprt m_time_sprt;
};


void newFontDraw()
{
    pgNewTask(FontDraw);
}


FontDraw::FontDraw() : pgTask(ORDER_ZERO)
{
    m_start_time = pgSysMgr::getUsecTime();
    m_scr3d = pgDrawMgr::getScreen(pgDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_view1 = m_scr3d->view();
    m_view2 = m_view1.rotateY_s32(90).rotateX_s32(-30).translate(800.0f, 10.0f, 1500.0f);
    m_is_view2 = false;

    /*
        set up m_text_font1 and m_text_sprt1
    */
    const char* text1 = "Pogolyn Font-drawing Sample";

    pgDrawMgr::setFont(pgID_("stonsans.ttf"), 0);
    pgDrawMgr::setFontSize(48);

    m_text_font1.init(pgDrawMgr::calcFontDrawWidth(text1), pgDrawMgr::getFontSize());
    m_text_font1.drawString(0, 0, text1);

    m_text_sprt1.init(2, pgDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_text_sprt1.setTextureID(m_text_font1.getTextureID());
    m_text_sprt1.setPreset_defaultBlendHalf();

    m_text_sprt1.dataPos(0).set(1.0f, 99.0f, 0.0f);
    m_text_sprt1.setDataSize(0, m_text_font1.getWidth(), m_text_font1.getHeight());

    m_text_sprt1.dataPos(1).set(0.0f, 100.0f, 0.0f);
    m_text_sprt1.dataCol(1).set(255, 64, 128);
    m_text_sprt1.setDataSize(1, m_text_font1.getWidth(), m_text_font1.getHeight());

    /*
        set up m_text_font2 and m_text_sprt2
    */
    wchar_t text2[64];
    pgSysMgr::swprintf(text2, sizeof(text2) / sizeof(wchar_t), L"You can use %ls and %ls.", L"ASCII", L"UNICODE");

#ifdef WIN32
    pgDrawMgr::setFont(pgID_("msgothic.ttc"), 0);
#endif
    pgDrawMgr::setFontSize(36);

    m_text_font2.init(pgDrawMgr::calcFontDrawWidth(text2), pgDrawMgr::getFontSize());
    m_text_font2.drawString(0, 0, text2);

    m_text_sprt2.init(2, pgDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_text_sprt2.setTextureID(m_text_font2.getTextureID());
    m_text_sprt2.setPreset_defaultBlendHalf();

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

    m_time_sprt.init(2, pgDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_time_sprt.setTextureID(m_time_font.getTextureID());
    m_time_sprt.setPreset_defaultBlendHalf();

    m_time_sprt.setDataSize(0, 256, 100);

    m_time_sprt.dataPos(1).set(0.0f, -45.0f);
    m_time_sprt.dataCol(1).set(128, 128, 128, 128);
    m_time_sprt.setDataSize(1, 256, 80);
    m_time_sprt.setDataUV(1, 0.0f, 1.0f, 1.0f, 0.0f);

    m_time_sprt.local().trans.set(20.0f, -80.0f);
}


void FontDraw::onUpdate()
{
    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_F))
    {
        pgSysMgr::toggleFullScreen(640, 480);
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_Q))
    {
        pgEndPogolyn();
    }

    r32 time = static_cast<r32>(pgSysMgr::getUsecTime() - m_start_time) / 1000000.0f;
    time -= (static_cast<u32>(time) / 100) * 100;

    pgDrawMgr::setFont(pgID_("stonsans.ttf"), 0);
    pgDrawMgr::setFontSize(100);

    m_time_font.clearImage();
    m_time_font.drawString(0, 0, "%2.2f", time);

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_V))
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
