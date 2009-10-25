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
#include "collision_target.h"


static void drawSphere(const pgCdt::Sph sph, pgCol poly_col, pgCol line_col)
{
    pgDbgMgr::drawSphere(sph.getPos(), sph.getRadius(), poly_col, line_col, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
}


static void drawBox(const pgCdt::Box box, pgCol poly_col, pgCol line_col)
{
    pgDbgMgr::drawBox(box.getWorld(), box.getHalfSize() * 2.0f, poly_col, line_col, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
}


static void drawTriangle(const pgCdt::Tri tri, pgCol poly_col, pgCol line_col)
{
    pgDbgMgr::drawLine(tri.getPos1(), tri.getPos2(), line_col, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
    pgDbgMgr::drawLine(tri.getPos2(), tri.getPos3(), line_col, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
    pgDbgMgr::drawLine(tri.getPos3(), tri.getPos1(), line_col, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
    pgDbgMgr::drawPolygon(tri.getPos1(), tri.getPos2(), tri.getPos3(), poly_col, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
}


static void drawRay(const pgCdt::Ray ray, pgCol poly_col, pgCol line_col)
{
    pgDbgMgr::drawLine(ray.getFrom(), ray.getTo(), line_col, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
    pgDbgMgr::drawSphere(ray.getFrom(), 4.0f, poly_col, line_col, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
}


static void drawAABB(const pgCdt::AABB aabb, pgCol line_col)
{
    pgMat world = pgMat::UNIT;
    world.trans = (aabb.getMin() + aabb.getMax()) / 2.0f;

    pgDbgMgr::drawBox(world, aabb.getMax() - aabb.getMin(), pgCol::ZERO, line_col, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
}


bool CollisionTarget::checkTargetHit(const Target1& tgt1, const pgCdt::Ray& hit_ray)
{
    switch (tgt1.type)
    {
    case TARGET1_SPHERE:
        return pgCdt::intersect(NULL, hit_ray, tgt1.sph);

    case TARGET1_BOX:
        return pgCdt::intersect(NULL, hit_ray, tgt1.box);

    case TARGET1_RAY:
        {
            pgMat ray_world;
            ray_world.x_axis = (tgt1.ray.getFrom() - tgt1.ray.getTo()).normalize();
            ray_world.y_axis = ((pgMath::abs(ray_world.x_axis.dot(pgVec::Y_UNIT)) < 0.7) ? pgVec::Y_UNIT : pgVec::Z_UNIT).cross(ray_world.x_axis);
            ray_world.z_axis = ray_world.x_axis.cross(ray_world.y_axis);
            ray_world.trans = (tgt1.ray.getFrom() + tgt1.ray.getTo()) / 2.0f;

            pgCdt::Box ray_box;
            ray_box.setWorld(ray_world);
            ray_box.setSize(tgt1.ray.getFrom().dist(tgt1.ray.getTo()), 20.0f, 20.0f);

            return pgCdt::intersect(NULL, hit_ray, ray_box);
        }

    default:
        return false;
    }
}


bool CollisionTarget::checkTargetHit(const Target2& tgt2, const pgCdt::Ray& hit_ray)
{
    switch (tgt2.type)
    {
    case TARGET2_SPHERE:
        return pgCdt::intersect(NULL, hit_ray, tgt2.sph);

    case TARGET2_BOX:
        return pgCdt::intersect(NULL, hit_ray, tgt2.box);

    case TARGET2_TRIANGLE:
        return pgCdt::intersect(NULL, hit_ray, tgt2.tri);

    default:
        return false;
    }
}


void CollisionTarget::updateTarget(Target1* tgt1, const pgMat& world, const pgVec& size)
{
    tgt1->sph.setPos(world.trans);
    tgt1->sph.setRadius(size.y / 2.0f);

    tgt1->box.setWorld(world);
    tgt1->box.setSize(size.x, size.y, size.z);

    pgVec vec_y = world.y_axis * (size.y / 1.5f);

    tgt1->ray.setPos(world.trans - vec_y, world.trans + vec_y);
}


void CollisionTarget::updateTarget(Target2* tgt2, const pgMat& world, const pgVec& size)
{
    tgt2->sph.setPos(world.trans);
    tgt2->sph.setRadius(size.x / 2.0f);

    tgt2->box.setWorld(world);
    tgt2->box.setSize(size.x, size.y, size.z);

    pgVec vec_x = world.x_axis * (size.x / 1.5f);
    pgVec vec_y = world.y_axis * (size.y / 1.5f);

    tgt2->tri.setPos(world.trans + vec_y, world.trans + vec_x - vec_y, world.trans - vec_x - vec_y);
}


void CollisionTarget::drawTarget(const Target1& tgt1, pgCol poly_col, pgCol line_col, pgCol aabb_col)
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


void CollisionTarget::drawTarget(const Target2& tgt2, pgCol poly_col, pgCol line_col, pgCol aabb_col)
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


bool CollisionTarget::collide(pgCdt::CdtInfo* cdt_info, const Target1& tgt1, const Target2& tgt2)
{
    if (tgt1.type == TARGET1_SPHERE)
    {
        switch (tgt2.type)
        {
        case TARGET2_SPHERE:
            return pgCdt::collide(cdt_info, tgt1.sph, tgt2.sph);

        case TARGET2_BOX:
            return pgCdt::collide(cdt_info, tgt1.sph, tgt2.box);

        case TARGET2_TRIANGLE:
            return pgCdt::collide(cdt_info, tgt1.sph, tgt2.tri);

        default:
            return false;
        }
    }
    else if (tgt1.type == TARGET1_BOX)
    {
        switch (tgt2.type)
        {
        case TARGET2_SPHERE:
            return pgCdt::collide(cdt_info, tgt1.box, tgt2.sph);

        case TARGET2_BOX:
            return pgCdt::collide(cdt_info, tgt1.box, tgt2.box);

        case TARGET2_TRIANGLE:
            return pgCdt::collide(cdt_info, tgt1.box, tgt2.tri);

        default:
            return false;
        }
    }

    return false;
}


bool CollisionTarget::intersect(pgVec* pos, const Target1& tgt1, const Target2& tgt2)
{
    if (tgt1.type == TARGET1_RAY)
    {
        switch (tgt2.type)
        {
        case TARGET2_SPHERE:
            return pgCdt::intersect(pos, tgt1.ray, tgt2.sph);

        case TARGET2_BOX:
            return pgCdt::intersect(pos, tgt1.ray, tgt2.box);

        case TARGET2_TRIANGLE:
            return pgCdt::intersect(pos, tgt1.ray, tgt2.tri);

        default:
            return false;
        }
    }

    return false;
}


void CollisionTarget::drawPos(const pgVec& pos, pgCol col)
{
    pgScr* scr = pgDrawMgr::getScreen(pgDrawMgr::DEFAULT_3D_SCREEN_ID);

    r32 rel_z = pos.toLocalOf(scr->view()).z;
    pgVec vec_x = scr->view().x_axis * (8.0f * -rel_z / scr->getFocusDist());
    pgVec vec_y = scr->view().y_axis * (8.0f * -rel_z / scr->getFocusDist());

    pgDbgMgr::drawLine(pos + vec_x + vec_y, pos - vec_x - vec_y, col, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
    pgDbgMgr::drawLine(pos - vec_x + vec_y, pos + vec_x - vec_y, col, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
}


void CollisionTarget::drawAxis(pgCol col)
{
    const r32 AXIS_LENGTH = 10000.0f;

    pgDbgMgr::drawLine(pgVec::X_UNIT * AXIS_LENGTH, pgVec::X_UNIT * -AXIS_LENGTH, col, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
    pgDbgMgr::drawLine(pgVec::Y_UNIT * AXIS_LENGTH, pgVec::Y_UNIT * -AXIS_LENGTH, col, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
    pgDbgMgr::drawLine(pgVec::Z_UNIT * AXIS_LENGTH, pgVec::Z_UNIT * -AXIS_LENGTH, col, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
}
