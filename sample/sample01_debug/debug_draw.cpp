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


class DebugDraw : public pgTask
{
public:
    DebugDraw();

private:
    virtual void onUpdate();

    pgMat m_box_world;
    pgVec m_box_size;

    pgMat m_sphere_world;
    r32 m_sphere_radius;
};


void newDebugDraw()
{
    pgNewTask(DebugDraw);
}


DebugDraw::DebugDraw() : pgTask(ORDER_ZERO)
{
    m_box_world = pgMat::UNIT;
    m_box_world.trans.set(-130.0f, 100.0f, 0.0f);
    m_box_size.set(150.0f, 100.0f, 50.0f);

    m_sphere_world = pgMat::UNIT;
    m_sphere_world.trans.set(130.0f, 100.0f, 0.0);
    m_sphere_radius = 80.0f;
}


void DebugDraw::onUpdate()
{
    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_F))
    {
        pgSysMgr::toggleFullScreen(640, 480);
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_Q))
    {
        pgEndPogolyn();
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_V))
    {
        pgDbgMgr::dumpVector(m_box_size, "m_box_size");
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_M))
    {
        pgDbgMgr::dumpMatrix(m_sphere_world, "m_sphere_world");
    }

    m_box_world = m_box_world.rotateY_s32(2).rotateX_r32(1);
    m_sphere_world = m_sphere_world.rotateX_s32(2).rotateY_s32(1);

    pgDbgMgr::drawBox(m_box_world, m_box_size, pgCol(0, 128, 255, 128), pgCol(255, 255, 255, 128), pgDrawMgr::DEFAULT_3D_SCREEN_ID);
    pgDbgMgr::drawSphere(m_sphere_world.trans, m_sphere_radius, pgCol(255, 0, 128, 128), pgCol(255, 255, 255, 128), pgDrawMgr::DEFAULT_3D_SCREEN_ID);

    pgDbgMgr::drawAxis(m_box_world, 100.0f, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
    pgDbgMgr::drawAxis(m_sphere_world, 100.0f, pgDrawMgr::DEFAULT_3D_SCREEN_ID);

    pgDbgMgr::drawLine(pgVec(-250.0f, -50.0f), pgVec(-150.0f, -150.0f), pgCol(255, 255, 0), pgDrawMgr::DEFAULT_2D_SCREEN_ID);
    pgDbgMgr::drawPolygon(pgVec(0.0f, -50.0f), pgVec(-50.0f, -150.0f), pgVec(50.0f, -150.0f), pgCol(255, 0, 255), pgDrawMgr::DEFAULT_2D_SCREEN_ID);
    pgDbgMgr::drawPolygon(pgVec(150.0f, -50.0f), pgVec(150.0f, -150.0f), pgVec(250.0f, -50.0f), pgVec(250.0f, -150.0f), pgCol(0, 192, 0), pgDrawMgr::DEFAULT_2D_SCREEN_ID);

    u32 frame_count = pgTaskMgr::getFrameCount();

    if (frame_count % 10 == 0)
    {
        pgDbgMgr::trace("%d\n", frame_count);
    }
}
