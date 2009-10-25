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


class Cat : public pgTask
{
public:
    Cat();

private:
    static const u32 CAT_NUM = 32;

    virtual void onUpdate();

    pgNode m_cat_root;
    pgSprt m_cat_sprt[CAT_NUM];
};


void newCat()
{
    pgNewTask(Cat);
}


Cat::Cat() : pgTask(ORDER_ZERO)
{
    m_cat_root.init(pgDrawMgr::DEFAULT_3D_SCREEN_ID);

    for (u32 i = 0; i < CAT_NUM; i++)
    {
        pgSprt* sprt = &m_cat_sprt[i];

        sprt->init(1, &m_cat_root);
        sprt->setTextureID(pgID_("stephanie_275x196.png"));
        sprt->setPreset_defaultBlendHalf();

        sprt->setDataSize(0, 275.0f * 0.5f, 196.0f * 0.5f);
        sprt->dataAng(0) = pgMath::rand(-30, 30);

        sprt->local().trans.set( //
            pgMath::rand(-200.0f, 200.0f, 20.0f), //
            pgMath::rand(-200.0f, 200.0f, 20.0f), //
            pgMath::rand(-200.0f, 200.0f, 20.0f));
    }
}


void Cat::onUpdate()
{
    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_F))
    {
        pgSysMgr::toggleFullScreen(640, 480);
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_Q))
    {
        pgEndPogolyn();
    }

    pgDbgMgr::drawBox(pgMat::UNIT, pgVec(400.0f, 400.0f, 400.0f), pgCol::ZERO, pgCol(128, 128, 128), pgDrawMgr::DEFAULT_3D_SCREEN_ID);

    m_cat_root.local() = m_cat_root.local().rotateY_r32(0.2f);

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_A))
    {
        if (m_cat_root.getColor().a == 255)
        {
            m_cat_root.setColor(pgCol(255, 255, 255, 96));
        }
        else
        {
            m_cat_root.setColor(pgCol::FULL);
        }
    }
}
