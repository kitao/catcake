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


class CatcakeLogo : public ckTask
{
public:
    CatcakeLogo(const char** cmd, u8 cmd_num);
    virtual ~CatcakeLogo();

private:
    void setPos();
    virtual void onUpdate();

    const char** m_cmd;
    u8 m_cmd_num;
    ckScr* m_scr;
    ckSprt m_logo_sprt;
};


void newCatcakeLogo(const char** cmd, u8 cmd_num)
{
    ckNewTask(CatcakeLogo)(cmd, cmd_num);
}


CatcakeLogo::CatcakeLogo(const char** cmd, u8 cmd_num) : ckTask(ORDER_PLUS_7)
{
    m_cmd = cmd;
    m_cmd_num = cmd_num;

    m_scr = ckDrawMgr::newScreen(ckID::genID());
    m_scr->setClearMode(false, true);
    m_scr->setPerspective(false);
    m_scr->moveLast();

    m_logo_sprt.init(1, m_scr->getID());
    m_logo_sprt.setTextureID(ckID_("catcake_logo_71x14.png"));
    m_logo_sprt.setDrawFlag(ckDraw::FLAG_BILINEAR, false);

    m_logo_sprt.setDataSize(0, 71.0f * 3.0f, 14.0f * 3.0f);

    setPos();
}


CatcakeLogo::~CatcakeLogo()
{
    ckDrawMgr::deleteScreen(m_scr->getID());
}


void CatcakeLogo::setPos()
{
    r32 x = (ckSysMgr::getFramebufferWidth() - m_logo_sprt.dataW(0)) / 2.0f - 16.0f;
    r32 y = (ckSysMgr::getFramebufferHeight() - m_logo_sprt.dataH(0)) / -2.0f + 16.0f;

    m_logo_sprt.dataPos(0).set(x, y);
}


void CatcakeLogo::onUpdate()
{
    if (ckSysMgr::isFramebufferSizeChanged())
    {
        m_scr->setAreaInFramebuffer(0, 0, ckSysMgr::getFramebufferWidth(), ckSysMgr::getFramebufferHeight());
        m_scr->setViewSize(ckSysMgr::getFramebufferWidth(), ckSysMgr::getFramebufferHeight());

        setPos();
    }

    const r32 left = ckSysMgr::getFramebufferWidth() / -2.0f + 16.0f;
    const r32 top = ckSysMgr::getFramebufferHeight() / 2.0f - 16.0f;
    const r32 interval = 12.0f;

    for (s32 i = 0; i < m_cmd_num; i++)
    {
        ckDbgMgr::drawString(left, top - interval * i, ckCol::FULL, 1, m_cmd[i]);
    }
}
