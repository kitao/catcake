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


class CopyCat : public ckTask
{
public:
    CopyCat();

private:
    static const u32 COPY_CAT_NUM = 32;

    virtual void onUpdate();

    bool m_is_copy_texture;
    ckScr* m_scr2d;
    ckSprt m_copy_sprt;
};


void newCopyCat()
{
    ckNewTask(CopyCat);
}


CopyCat::CopyCat() : ckTask(ORDER_ZERO)
{
    m_is_copy_texture = true;

    m_scr2d = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_scr2d->attachScreenTexture(ckTex::FORMAT_RGBA);

    m_copy_sprt.init(COPY_CAT_NUM, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_copy_sprt.setTextureID(m_scr2d->getScreenTextureID());
    m_copy_sprt.setBlendMode(ckDraw::BLEND_HALF, true);

    for (u32 i = 0; i < COPY_CAT_NUM; i++)
    {
        m_copy_sprt.dataPos(i).set(ckMath::rand(-320.0f, 320.0f, 10.0f), ckMath::rand(-240.0f, 240.0f, 10.0f), ckMath::rand(-300.0f, 300.0f, 10.0f));
        m_copy_sprt.dataAng(i) = ckMath::rand(-90, 90);
        m_copy_sprt.setDataSize(i, 120.0f, 90.0f);
    }
}


void CopyCat::onUpdate()
{
    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_F))
    {
        ckSysMgr::toggleFullScreen(640, 480);

        m_is_copy_texture = true;
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_C))
    {
        m_is_copy_texture = !m_is_copy_texture;
    }

    if (m_is_copy_texture)
    {
        m_scr2d->updateScreenTexture(false);
    }
}
