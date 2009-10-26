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
