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


class Coin : public ckTask
{
public:
    Coin();

private:
    virtual void onUpdate();

    ckSprt m_coin_sprt;
};


void newCoin()
{
    ckNewTask(Coin);
}


Coin::Coin() : ckTask(ORDER_ZERO)
{
    m_coin_sprt.init(6, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_coin_sprt.setTextureID(ckID_("coin_400x200.png"));
    m_coin_sprt.setBlendMode(ckDraw::BLEND_HALF, true);

    m_coin_sprt.dataPos(0).set(-200.0f, 150.0f);
    m_coin_sprt.dataPos(1).set(-200.0f, 0.0f);
    m_coin_sprt.dataPos(2).set(-200.0f, -150.0f);
    m_coin_sprt.dataPos(3).set(200.0f, 150.0f);
    m_coin_sprt.dataPos(4).set(200.0f, 0.0f);
    m_coin_sprt.dataPos(5).set(200.0f, -150.0f);

    for (s32 i = 0; i < 6; i++)
    {
        m_coin_sprt.setDataSize(i, 96.0f, 96.0f);

        if (i % 2 == 0)
        {
            m_coin_sprt.setDataUV(i, 0.0f, 0.0f, 0.5f, 1.0f);
        }
        else
        {
            m_coin_sprt.setDataUV(i, 0.5f, 0.0f, 1.0f, 1.0f);
        }
    }
}


void Coin::onUpdate()
{
    ckScr* scr = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_2D_SCREEN_ID);
    r32 mouse_x = scr->framebufferXToScreenX(ckKeyMgr::getMouseX());
    r32 mouse_y = scr->framebufferYToScreenY(ckKeyMgr::getMouseY());

    for (s32 i = 0; i < 6; i++)
    {
        m_coin_sprt.dataAng(i) += (i % 2 == 0) ? -1 : 1;

        r32 diff_x = m_coin_sprt.dataPos(i).x - mouse_x;
        r32 diff_y = m_coin_sprt.dataPos(i).y - mouse_y;

        r32 aim_size = (diff_x * diff_x + diff_y * diff_y <= 48.0f * 48.0f) ? 192.0f : 96.0f;

        m_coin_sprt.dataW(i) = m_coin_sprt.dataH(i) = ckMath::interp(m_coin_sprt.dataW(i), aim_size, 0.1f);
    }
}
