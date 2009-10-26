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
    m_copy_sprt.setPreset_defaultBlendHalf();

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
