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


class DebugDraw : public ckTask
{
public:
    DebugDraw();

private:
    virtual void onUpdate();

    ckMat m_box_world;
    ckVec m_box_size;

    ckMat m_sphere_world;
    r32 m_sphere_radius;
};


void newDebugDraw()
{
    ckNewTask(DebugDraw);
}


DebugDraw::DebugDraw() : ckTask(ORDER_ZERO)
{
    m_box_world = ckMat::UNIT;
    m_box_world.trans.set(-130.0f, 100.0f, 0.0f);
    m_box_size.set(150.0f, 100.0f, 50.0f);

    m_sphere_world = ckMat::UNIT;
    m_sphere_world.trans.set(130.0f, 100.0f, 0.0);
    m_sphere_radius = 80.0f;
}


void DebugDraw::onUpdate()
{
    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_F))
    {
        ckSysMgr::toggleFullScreen(640, 480);
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_Q))
    {
        ckEndCatcake();
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_V))
    {
        ckDbgMgr::dumpVector(m_box_size, "m_box_size");
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_M))
    {
        ckDbgMgr::dumpMatrix(m_sphere_world, "m_sphere_world");
    }

    m_box_world = m_box_world.rotateY_s32(2).rotateX_r32(1);
    m_sphere_world = m_sphere_world.rotateX_s32(2).rotateY_s32(1);

    ckDbgMgr::drawBox(m_box_world, m_box_size, ckCol(0, 128, 255, 128), ckCol(255, 255, 255, 128), ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    ckDbgMgr::drawSphere(m_sphere_world.trans, m_sphere_radius, ckCol(255, 0, 128, 128), ckCol(255, 255, 255, 128), ckDrawMgr::DEFAULT_3D_SCREEN_ID);

    ckDbgMgr::drawAxis(m_box_world, 100.0f, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    ckDbgMgr::drawAxis(m_sphere_world, 100.0f, ckDrawMgr::DEFAULT_3D_SCREEN_ID);

    ckDbgMgr::drawLine(ckVec(-250.0f, -50.0f), ckVec(-150.0f, -150.0f), ckCol(255, 255, 0), ckDrawMgr::DEFAULT_2D_SCREEN_ID);
    ckDbgMgr::drawPolygon(ckVec(0.0f, -50.0f), ckVec(-50.0f, -150.0f), ckVec(50.0f, -150.0f), ckCol(255, 0, 255), ckDrawMgr::DEFAULT_2D_SCREEN_ID);
    ckDbgMgr::drawPolygon(ckVec(150.0f, -50.0f), ckVec(150.0f, -150.0f), ckVec(250.0f, -50.0f), ckVec(250.0f, -150.0f), ckCol(0, 192, 0), ckDrawMgr::DEFAULT_2D_SCREEN_ID);

    u32 frame_count = ckTaskMgr::getFrameCount();

    if (frame_count % 10 == 0)
    {
        ckDbgMgr::trace("%d\n", frame_count);
    }
}
