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
