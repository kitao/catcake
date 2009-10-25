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


class Blood : public pgTask
{
public:
    Blood(const pgVec& pos);

private:
    virtual void onUpdate();

    pgVec m_speed;
    pgMdl m_blood_mdl;
};


void newBlood(const pgVec& pos)
{
    pgNewTask(Blood)(pos);
}


Blood::Blood(const pgVec& pos) : pgTask(ORDER_ZERO)
{
    m_speed.set(pgMath::rand(-2.0f, 2.0f, 0.1f), pgMath::rand(0.0f, 2.0f, 0.1f), pgMath::rand(-1.0f, -2.0f, 0.1f));

    m_blood_mdl.init(pgID_("blood.pxm"), pgDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_blood_mdl.setLightSetID(pgDrawMgr::DEFAULT_LIGHT_SET_ID);
    m_blood_mdl.getRootDraw()->local().trans = pos;
}


void Blood::onUpdate()
{
    m_blood_mdl.getRootDraw()->local() = m_blood_mdl.getRootDraw()->local().rotateX_s32(3);
    m_blood_mdl.getRootDraw()->local().trans += m_speed;

    m_speed.y -= 0.1f;

    if (m_blood_mdl.getRootDraw()->local().trans.y < 0.0f)
    {
        pgDeleteTask(this);
    }
}
