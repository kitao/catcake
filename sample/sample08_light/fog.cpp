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

        fog_sprt->setBlendMode(ckDraw::BLEND_HALF, true);

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
                m_fog_sprt[i].setBlendMode(ckDraw::BLEND_ADD, true);
            }
        }
        else
        {
            for (u32 i = 0; i < FOG_NUM; i++)
            {
                m_fog_sprt[i].setBlendMode(ckDraw::BLEND_HALF, true);
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
