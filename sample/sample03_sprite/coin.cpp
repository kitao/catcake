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
