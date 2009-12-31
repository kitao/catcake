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


class Cat : public ckTask
{
public:
    Cat();

private:
    static const u32 CAT_NUM = 32;

    virtual void onUpdate();

    ckNode m_cat_root;
    ckSprt m_cat_sprt[CAT_NUM];
};


void newCat()
{
    ckNewTask(Cat);
}


Cat::Cat() : ckTask(ORDER_ZERO)
{
    m_cat_root.init(ckDrawMgr::DEFAULT_3D_SCREEN_ID);

    for (u32 i = 0; i < CAT_NUM; i++)
    {
        ckSprt* sprt = &m_cat_sprt[i];

        sprt->init(1, &m_cat_root);
        sprt->setTextureID(ckID_("stephanie_275x196.png"));
        sprt->setBlendMode(ckDraw::BLEND_HALF, true);

        sprt->setDataSize(0, 275.0f * 0.5f, 196.0f * 0.5f);
        sprt->dataAng(0) = ckMath::rand(-30, 30);

        sprt->local().trans.set( //
            ckMath::rand(-200.0f, 200.0f, 20.0f), //
            ckMath::rand(-200.0f, 200.0f, 20.0f), //
            ckMath::rand(-200.0f, 200.0f, 20.0f));
    }
}


void Cat::onUpdate()
{
    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_F))
    {
        ckSysMgr::toggleFullScreen(640, 480);
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_Q))
    {
        ckEndCatcake();
    }

    ckDbgMgr::drawBox(ckMat::UNIT, ckVec(400.0f, 400.0f, 400.0f), ckCol::ZERO, ckCol(128, 128, 128), ckDrawMgr::DEFAULT_3D_SCREEN_ID);

    m_cat_root.local() = m_cat_root.local().rotateY_r32(0.2f);

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_A))
    {
        if (m_cat_root.getColor().a == 255)
        {
            m_cat_root.setColor(ckCol(255, 255, 255, 96));
        }
        else
        {
            m_cat_root.setColor(ckCol::FULL);
        }
    }
}
