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
