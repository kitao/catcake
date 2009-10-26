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


class SubScreen : public ckTask
{
public:
    SubScreen();
    virtual ~SubScreen();

private:
    virtual void onUpdate();

    ckScr* m_sub_scr1;
    ckScr* m_sub_scr2;
};


void newSubScreen()
{
    ckNewTask(SubScreen);
}


SubScreen::SubScreen() : ckTask(ORDER_ZERO)
{
    m_sub_scr1 = ckDrawMgr::newScreen(ckID::genID());
    m_sub_scr1->setClearColor(ckCol(0, 128, 0, 0));
    m_sub_scr1->setGuestScreenID(0, ckID_("blurred_screen"));
    m_sub_scr1->setAreaInFramebuffer(466, 16, 160, 120);
    m_sub_scr1->view() = ckMat::lookAt(ckVec(300.0f, 0.0f, 0.0f), ckVec::ZERO, ckVec::Y_UNIT);

    m_sub_scr2 = ckDrawMgr::newScreen(ckID::genID());
    m_sub_scr2->setClearColor(ckCol(128, 0, 0, 128));
    m_sub_scr2->setGuestScreenID(0, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_sub_scr2->setGuestScreenID(1, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_sub_scr2->setAreaInFramebuffer(16, 344, 160, 120);
    m_sub_scr2->view() = ckMat::lookAt(ckVec(0.0f, 1000.0f, 400.0f), ckVec::ZERO, ckVec::Y_UNIT);
}


SubScreen::~SubScreen()
{
    ckDrawMgr::deleteScreen(m_sub_scr1->getID());
    ckDrawMgr::deleteScreen(m_sub_scr2->getID());
}


void SubScreen::onUpdate()
{
    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_S))
    {
        m_sub_scr1->setActive(!m_sub_scr1->isActive());
        m_sub_scr2->setActive(!m_sub_scr2->isActive());
    }
}
