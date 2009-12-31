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


class Tetrahedron : public ckTask
{
public:
    Tetrahedron();

private:
    virtual void onUpdate();
    virtual void onMessage(ckID msg_id, ckMsg<4>& msg);

    ckPrim m_tetra_prim;
    ckRend_Light m_tetra_rend;
    ckVec m_speed;
};


void newTetrahedron()
{
    ckNewTask(Tetrahedron);
}


Tetrahedron::Tetrahedron() : ckTask(ORDER_ZERO)
{
    m_tetra_prim.init(ckPrim::MODE_TRIANGLES, 12, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_tetra_rend.init(&m_tetra_prim, ckDrawMgr::DEFAULT_LIGHT_SET_ID);

    ckVec pos1(0.0f, 1.225f, 0.0f);
    ckVec pos2(0.0f, -0.408f, 1.154f);
    ckVec pos3(1.0f, -0.408f, -0.577f);
    ckVec pos4(-1.0f, -0.408f, -0.577f);
    r32 size = 50.0f;

    pos1 *= size;
    pos2 *= size;
    pos3 *= size;
    pos4 *= size;

    m_tetra_prim.dataPos(0) = pos1;
    m_tetra_prim.dataPos(1) = pos2;
    m_tetra_prim.dataPos(2) = pos3;
    m_tetra_prim.dataPos(3) = pos1;
    m_tetra_prim.dataPos(4) = pos3;
    m_tetra_prim.dataPos(5) = pos4;
    m_tetra_prim.dataPos(6) = pos1;
    m_tetra_prim.dataPos(7) = pos4;
    m_tetra_prim.dataPos(8) = pos2;
    m_tetra_prim.dataPos(9) = pos4;
    m_tetra_prim.dataPos(10) = pos3;
    m_tetra_prim.dataPos(11) = pos2;

    m_tetra_prim.dataCol(9) = m_tetra_prim.dataCol(10) = m_tetra_prim.dataCol(11) = ckCol::ZERO;

    m_tetra_rend.calcNormalAsTriangles(false);

    m_speed = ckVec::ZERO;
}


void Tetrahedron::onUpdate()
{
    ckVec pos = m_tetra_prim.local().trans;

    m_speed.y -= 0.1f;

    pos += m_speed;

    if (pos.y < 0.0f)
    {
        pos.y = 0.0f;

        m_speed.y = 4.0f;

        m_speed.x += ckMath::rand(-1.0f, 1.0f, 0.1f);
        m_speed.z += ckMath::rand(-1.0f, 1.0f, 0.1f);

        m_speed.x = ckMath::clamp(m_speed.x, -3.0f, 3.0f);
        m_speed.z = ckMath::clamp(m_speed.z, -3.0f, 3.0f);
    }

    if (pos.x > 180.0f)
    {
        pos.x = 180.0f;
        m_speed.x = -ckMath::abs(m_speed.x);
    }

    if (pos.x < -180.0f)
    {
        pos.x = -180.0f;
        m_speed.x = ckMath::abs(m_speed.x);
    }

    if (m_tetra_prim.local().trans.z > 180.0f)
    {
        pos.z = 180.0f;
        m_speed.z = -ckMath::abs(m_speed.z);
    }

    if (m_tetra_prim.local().trans.z < -180.0f)
    {
        pos.z = -180.0f;
        m_speed.z = ckMath::abs(m_speed.z);
    }

    m_tetra_prim.local() = m_tetra_prim.local().rotateY_s32(1).rotateX_s32(2);
    m_tetra_prim.local().trans = pos;
}


void Tetrahedron::onMessage(ckID msg_id, ckMsg<4>& msg)
{
    if (msg_id == ckID_("CHANGE LIGHT SET"))
    {
        m_tetra_rend.setLightSetID(msg.getParam<ckID>(0));
    }
}
