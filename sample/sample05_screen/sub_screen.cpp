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
