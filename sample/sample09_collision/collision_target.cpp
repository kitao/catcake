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
#include "collision_target.h"


static void drawSphere(const ckCdt::Sph sph, ckCol poly_col, ckCol line_col)
{
    ckDbgMgr::drawSphere(sph.getPos(), sph.getRadius(), poly_col, line_col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
}


static void drawBox(const ckCdt::Box box, ckCol poly_col, ckCol line_col)
{
    ckDbgMgr::drawBox(box.getWorld(), box.getHalfSize() * 2.0f, poly_col, line_col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
}


static void drawTriangle(const ckCdt::Tri tri, ckCol poly_col, ckCol line_col)
{
    ckDbgMgr::drawLine(tri.getPos1(), tri.getPos2(), line_col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    ckDbgMgr::drawLine(tri.getPos2(), tri.getPos3(), line_col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    ckDbgMgr::drawLine(tri.getPos3(), tri.getPos1(), line_col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    ckDbgMgr::drawPolygon(tri.getPos1(), tri.getPos2(), tri.getPos3(), poly_col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
}


static void drawRay(const ckCdt::Ray ray, ckCol poly_col, ckCol line_col)
{
    ckDbgMgr::drawLine(ray.getFrom(), ray.getTo(), line_col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    ckDbgMgr::drawSphere(ray.getFrom(), 4.0f, poly_col, line_col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
}


static void drawAABB(const ckCdt::AABB aabb, ckCol line_col)
{
    ckMat world = ckMat::UNIT;
    world.trans = (aabb.getMin() + aabb.getMax()) / 2.0f;

    ckDbgMgr::drawBox(world, aabb.getMax() - aabb.getMin(), ckCol::ZERO, line_col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
}


bool CollisionTarget::checkTargetHit(const Target1& tgt1, const ckCdt::Ray& hit_ray)
{
    switch (tgt1.type)
    {
    case TARGET1_SPHERE:
        return ckCdt::intersect(NULL, hit_ray, tgt1.sph);

    case TARGET1_BOX:
        return ckCdt::intersect(NULL, hit_ray, tgt1.box);

    case TARGET1_RAY:
        {
            ckMat ray_world;
            ray_world.x_axis = (tgt1.ray.getFrom() - tgt1.ray.getTo()).normalize();
            ray_world.y_axis = ((ckMath::abs(ray_world.x_axis.dot(ckVec::Y_UNIT)) < 0.7) ? ckVec::Y_UNIT : ckVec::Z_UNIT).cross(ray_world.x_axis);
            ray_world.z_axis = ray_world.x_axis.cross(ray_world.y_axis);
            ray_world.trans = (tgt1.ray.getFrom() + tgt1.ray.getTo()) / 2.0f;

            ckCdt::Box ray_box;
            ray_box.setWorld(ray_world);
            ray_box.setSize(tgt1.ray.getFrom().dist(tgt1.ray.getTo()), 20.0f, 20.0f);

            return ckCdt::intersect(NULL, hit_ray, ray_box);
        }

    default:
        return false;
    }
}


bool CollisionTarget::checkTargetHit(const Target2& tgt2, const ckCdt::Ray& hit_ray)
{
    switch (tgt2.type)
    {
    case TARGET2_SPHERE:
        return ckCdt::intersect(NULL, hit_ray, tgt2.sph);

    case TARGET2_BOX:
        return ckCdt::intersect(NULL, hit_ray, tgt2.box);

    case TARGET2_TRIANGLE:
        return ckCdt::intersect(NULL, hit_ray, tgt2.tri);

    default:
        return false;
    }
}


void CollisionTarget::updateTarget(Target1* tgt1, const ckMat& world, const ckVec& size)
{
    tgt1->sph.setPos(world.trans);
    tgt1->sph.setRadius(size.y / 2.0f);

    tgt1->box.setWorld(world);
    tgt1->box.setSize(size.x, size.y, size.z);

    ckVec vec_y = world.y_axis * (size.y / 1.5f);

    tgt1->ray.setPos(world.trans - vec_y, world.trans + vec_y);
}


void CollisionTarget::updateTarget(Target2* tgt2, const ckMat& world, const ckVec& size)
{
    tgt2->sph.setPos(world.trans);
    tgt2->sph.setRadius(size.x / 2.0f);

    tgt2->box.setWorld(world);
    tgt2->box.setSize(size.x, size.y, size.z);

    ckVec vec_x = world.x_axis * (size.x / 1.5f);
    ckVec vec_y = world.y_axis * (size.y / 1.5f);

    tgt2->tri.setPos(world.trans + vec_y, world.trans + vec_x - vec_y, world.trans - vec_x - vec_y);
}


void CollisionTarget::drawTarget(const Target1& tgt1, ckCol poly_col, ckCol line_col, ckCol aabb_col)
{
    switch (tgt1.type)
    {
    case TARGET1_SPHERE:
        drawSphere(tgt1.sph, poly_col, line_col);
        drawAABB(tgt1.sph.getAABB(), aabb_col);
        break;

    case TARGET1_BOX:
        drawBox(tgt1.box, poly_col, line_col);
        drawAABB(tgt1.box.getAABB(), aabb_col);
        break;

    case TARGET1_RAY:
        drawRay(tgt1.ray, poly_col, line_col);
        drawAABB(tgt1.ray.getAABB(), aabb_col);
        break;

    default:
        break;
    }
}


void CollisionTarget::drawTarget(const Target2& tgt2, ckCol poly_col, ckCol line_col, ckCol aabb_col)
{
    switch (tgt2.type)
    {
    case TARGET2_SPHERE:
        drawSphere(tgt2.sph, poly_col, line_col);
        drawAABB(tgt2.sph.getAABB(), aabb_col);
        break;

    case TARGET2_BOX:
        drawBox(tgt2.box, poly_col, line_col);
        drawAABB(tgt2.box.getAABB(), aabb_col);
        break;

    case TARGET2_TRIANGLE:
        drawTriangle(tgt2.tri, poly_col, line_col);
        drawAABB(tgt2.tri.getAABB(), aabb_col);
        break;

    default:
        break;
    }
}


bool CollisionTarget::collide(ckCdt::CdtInfo* cdt_info, const Target1& tgt1, const Target2& tgt2)
{
    if (tgt1.type == TARGET1_SPHERE)
    {
        switch (tgt2.type)
        {
        case TARGET2_SPHERE:
            return ckCdt::collide(cdt_info, tgt1.sph, tgt2.sph);

        case TARGET2_BOX:
            return ckCdt::collide(cdt_info, tgt1.sph, tgt2.box);

        case TARGET2_TRIANGLE:
            return ckCdt::collide(cdt_info, tgt1.sph, tgt2.tri);

        default:
            return false;
        }
    }
    else if (tgt1.type == TARGET1_BOX)
    {
        switch (tgt2.type)
        {
        case TARGET2_SPHERE:
            return ckCdt::collide(cdt_info, tgt1.box, tgt2.sph);

        case TARGET2_BOX:
            return ckCdt::collide(cdt_info, tgt1.box, tgt2.box);

        case TARGET2_TRIANGLE:
            return ckCdt::collide(cdt_info, tgt1.box, tgt2.tri);

        default:
            return false;
        }
    }

    return false;
}


bool CollisionTarget::intersect(ckVec* pos, const Target1& tgt1, const Target2& tgt2)
{
    if (tgt1.type == TARGET1_RAY)
    {
        switch (tgt2.type)
        {
        case TARGET2_SPHERE:
            return ckCdt::intersect(pos, tgt1.ray, tgt2.sph);

        case TARGET2_BOX:
            return ckCdt::intersect(pos, tgt1.ray, tgt2.box);

        case TARGET2_TRIANGLE:
            return ckCdt::intersect(pos, tgt1.ray, tgt2.tri);

        default:
            return false;
        }
    }

    return false;
}


void CollisionTarget::drawPos(const ckVec& pos, ckCol col)
{
    ckScr* scr = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);

    r32 rel_z = pos.toLocalOf(scr->view()).z;
    ckVec vec_x = scr->view().x_axis * (8.0f * -rel_z / scr->getFocusDist());
    ckVec vec_y = scr->view().y_axis * (8.0f * -rel_z / scr->getFocusDist());

    ckDbgMgr::drawLine(pos + vec_x + vec_y, pos - vec_x - vec_y, col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    ckDbgMgr::drawLine(pos - vec_x + vec_y, pos + vec_x - vec_y, col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
}


void CollisionTarget::drawAxis(ckCol col)
{
    const r32 AXIS_LENGTH = 10000.0f;

    ckDbgMgr::drawLine(ckVec::X_UNIT * AXIS_LENGTH, ckVec::X_UNIT * -AXIS_LENGTH, col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    ckDbgMgr::drawLine(ckVec::Y_UNIT * AXIS_LENGTH, ckVec::Y_UNIT * -AXIS_LENGTH, col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    ckDbgMgr::drawLine(ckVec::Z_UNIT * AXIS_LENGTH, ckVec::Z_UNIT * -AXIS_LENGTH, col, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
}
