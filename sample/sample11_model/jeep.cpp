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


#include "catcake_main.h"


class Jeep : public ckTask
{
public:
    Jeep();

private:
    virtual void onUpdate();
    void resetPivot();

    ckMdl m_jeep_mdl;
    ckMat m_world;
    ckVec m_pivot;
};


void newJeep()
{
    ckNewTask(Jeep);
}


Jeep::Jeep() : ckTask(ORDER_ZERO)
{
    m_jeep_mdl.init(ckID_("jeep.3ds"), ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_jeep_mdl.setLightSetID(ckDrawMgr::DEFAULT_LIGHT_SET_ID);

    m_world = ckMat::UNIT.translate(ckMath::rand(-100.0f, 100.0f, 10.0f), 0.0f, ckMath::rand(-100.0f, 100.0f, 10.0f));

    resetPivot();
}


void Jeep::onUpdate()
{
    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_T))
    {
        m_jeep_mdl.setTextureID((m_jeep_mdl.getTextureID() == ckID::ZERO) ? ckID_("jeep.png") : ckID::ZERO);
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_L))
    {
        m_jeep_mdl.setLightSetID((m_jeep_mdl.getLightSetID() == ckID::ZERO) ? ckDrawMgr::DEFAULT_LIGHT_SET_ID : ckID::ZERO);
    }

    r32 sq_dist = m_world.trans.sqDist(m_pivot);

    if (sq_dist < 200.0f * 200.0f || sq_dist > 1000.0f * 1000.0f)
    {
        resetPivot();
    }

    ckVec local_pivot = m_pivot.toLocalOf(m_world);

    if (local_pivot.x > 10.0f)
    {
        m_world = m_world.rotateY_r32(-1.5f);
    }
    else if (local_pivot.x < -10.0f)
    {
        m_world = m_world.rotateY_r32(1.5f);
    }

    m_world.trans -= m_world.z_axis * 4.0f;

    if (m_world.trans.sqLength() < 250.0f * 250.0f)
    {
        m_world.trans = m_world.trans.normalize() * 250.0f;
    }

    m_jeep_mdl.getRootDraw()->local() = m_world.rotateY_s32(180).rotateX_s32(-90);

    ckMat mat = ckMat::UNIT;
    mat.trans = m_pivot;
    ckDbgMgr::drawAxis(mat, 50.0f, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
}


void Jeep::resetPivot()
{
    m_pivot = ckVec::X_UNIT.rotateY_s32(ckMath::rand(-180, 180)) * 400.0f;
}
