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


class Fog : public ckTask
{
public:
    Fog();

private:
    static const u32 FOG_NUM = 128;

    virtual void onUpdate();
    virtual void onMessage(ckID msg_id, ckMsg<4>& msg);

    ckLts* m_cur_lts;
    ckSprt m_fog_sprt[FOG_NUM];
};


void newFog()
{
    ckNewTask(Fog);
}


Fog::Fog() : ckTask(ORDER_ZERO)
{
    m_cur_lts = ckDrawMgr::getLightSet(ckDrawMgr::DEFAULT_LIGHT_SET_ID);

    for (u32 i = 0; i < FOG_NUM; i++)
    {
        ckSprt* fog_sprt = &m_fog_sprt[i];

        fog_sprt->init(1, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        fog_sprt->setTextureID(ckID_("fog_128x128.png"));

        fog_sprt->setPreset_defaultBlendHalf();

        fog_sprt->dataPos(0).set(ckMath::rand(-180.0f, 180.0f, 1.0f), ckMath::rand(0.0f, 2.0f, 0.1f), ckMath::rand(-180.0f, 180.0f, 1.0f));
        fog_sprt->setDataSize(0, 128.0f, 128.0f);
        fog_sprt->dataAng(0) = ckMath::rand(-180, 180);
    }
}


void Fog::onUpdate()
{
    for (u32 i = 0; i < FOG_NUM; i++)
    {
        ckSprt* fog_sprt = &m_fog_sprt[i];

        m_cur_lts->findNearLight(fog_sprt->dataPos(0));

        fog_sprt->dataCol(0) = m_cur_lts->getAmbientColor();

        for (s32 j = 0; j < m_cur_lts->getNearLightNum(); j++)
        {
            fog_sprt->dataCol(0) += m_cur_lts->getNearLightColor(j);
        }

        fog_sprt->dataCol(0).a = 32;

        fog_sprt->dataAng(0)++;
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_B))
    {
        if (m_fog_sprt->getBlendMode() == ckDraw::BLEND_HALF)
        {
            for (u32 i = 0; i < FOG_NUM; i++)
            {
                m_fog_sprt[i].setPreset_defaultBlendAdd();
            }
        }
        else
        {
            for (u32 i = 0; i < FOG_NUM; i++)
            {
                m_fog_sprt[i].setPreset_defaultBlendHalf();
            }
        }
    }
}


void Fog::onMessage(ckID msg_id, ckMsg<4>& msg)
{
    if (msg_id == ckID_("CHANGE LIGHT SET"))
    {
        m_cur_lts = ckDrawMgr::getLightSet(msg.getParam<ckID>(0));
    }
}
