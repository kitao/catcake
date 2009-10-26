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
        sprt->setPreset_defaultBlendHalf();

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
