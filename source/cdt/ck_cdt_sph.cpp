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


#include "ck_cdt_all.h"


ckCdt::Sph::Sph()
{
    m_pos = ckVec::ZERO;
    m_radius = 0.0f;

    updateAABB();
}


const ckCdt::AABB& ckCdt::Sph::getAABB() const
{
    return m_aabb;
}


const ckVec& ckCdt::Sph::getPos() const
{
    return m_pos;
}


void ckCdt::Sph::setPos(const ckVec& pos)
{
    m_pos = pos;

    updateAABB();
}


r32 ckCdt::Sph::getRadius() const
{
    return m_radius;
}


void ckCdt::Sph::setRadius(r32 radius)
{
    if (radius < 0.0f)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_radius = radius;

    updateAABB();
}


void ckCdt::Sph::updateAABB()
{
    m_aabb.setBound( //
        ckVec(m_pos.x - m_radius, m_pos.y - m_radius, m_pos.z - m_radius), //
        ckVec(m_pos.x + m_radius, m_pos.y + m_radius, m_pos.z + m_radius));
}


bool ckCdt::collide(CdtInfo* cdt_info, const Sph& sph1, const Sph& sph2)
{
    if (!checkTouch(sph1.m_aabb, sph2.m_aabb))
    {
        return false;
    }

    ckVec diff = sph1.m_pos - sph2.m_pos;
    r32 sum_rad = sph1.m_radius + sph2.m_radius;
    r32 sq_dist = diff.sqLength();

    if (sq_dist > (sum_rad - ckMath::EPSILON) * (sum_rad - ckMath::EPSILON))
    {
        return false;
    }

    r32 dist = ckMath::sqrt(sq_dist);

    if (cdt_info)
    {
        cdt_info->back_dir = (dist > ckMath::EPSILON) ? diff / dist : ckVec::X_UNIT;
        cdt_info->back_dist = sum_rad - dist;
        cdt_info->pos = sph1.m_pos - cdt_info->back_dir * (sph1.m_radius - cdt_info->back_dist / 2.0f);
    }

    return true;
}


bool ckCdt::collide(CdtInfo* cdt_info, const Sph& sph, const Box& box)
{
    if (!checkTouch(sph.m_aabb, box.m_aabb))
    {
        return false;
    }

    ckVec diff = sph.m_pos - box.m_world.trans;
    ckVec closest = box.m_world.trans;

#define CALC_CLOSEST_POINT(compo, axis) \
    do \
    { \
        r32 dist = diff.dot(axis); \
    \
        if (dist > box.m_half_size.compo) \
        { \
            dist = box.m_half_size.compo; \
        } \
        else if (dist < -box.m_half_size.compo) \
        { \
            dist = -box.m_half_size.compo; \
        } \
    \
        closest += (axis) * dist; \
    } \
    while (0)

    CALC_CLOSEST_POINT(x, box.m_world.x_axis);
    CALC_CLOSEST_POINT(y, box.m_world.y_axis);
    CALC_CLOSEST_POINT(z, box.m_world.z_axis);

#undef CALC_CLOSEST_POINT

    diff = sph.m_pos - closest;

    r32 sq_dist = diff.sqLength();

    if (sq_dist > (sph.m_radius - ckMath::EPSILON) * (sph.m_radius - ckMath::EPSILON))
    {
        return false;
    }

    if (cdt_info)
    {
        r32 dist = ckMath::sqrt(sq_dist);

        cdt_info->back_dir = (dist > ckMath::EPSILON) ? diff / dist : ckVec::X_UNIT;
        cdt_info->back_dist = sph.m_radius - dist;
        cdt_info->pos = closest - cdt_info->back_dir * (cdt_info->back_dist / 2.0f);
    }

    return true;
}


bool ckCdt::collide(CdtInfo* cdt_info, const Sph& sph, const Tri& tri)
{
    if (!checkTouch(sph.m_aabb, tri.m_aabb))
    {
        return false;
    }

    ckVec closest;

    do
    {
        const ckVec& p = sph.m_pos;
        const ckVec& a = tri.m_pos1;
        const ckVec& b = tri.m_pos2;
        const ckVec& c = tri.m_pos3;

        ckVec ap = p - a;
        ckVec ab = b - a;
        ckVec ac = c - a;

        r32 ap_ab = ap.dot(ab);
        r32 ab_ac = ap.dot(ac);

        if (ap_ab <= 0.0f && ab_ac <= 0.0f)
        {
            closest = a;
            break;
        }

        ckVec bp = p - b;
        ckVec bc = c - b;

        r32 bp_ba = -bp.dot(ab);
        r32 bp_bc = bp.dot(bc);

        if (bp_ba <= 0.0f && bp_bc <= 0.0f)
        {
            closest = b;
            break;
        }

        ckVec cp = p - c;

        r32 cp_ca = -cp.dot(ac);
        r32 cp_cb = -cp.dot(bc);

        if (cp_ca <= 0.0f && cp_cb <= 0.0f)
        {
            closest = c;
            break;
        }

        ckVec n = ab.cross(ac);

        r32 vc = n.dot(ap.cross(bp));

        if (vc <= 0.0f && ap_ab >= 0.0f && bp_ba >= 0.0f)
        {
            closest = a + ab * (ap_ab / (ap_ab + bp_ba));
            break;
        }

        r32 va = n.dot(bp.cross(cp));

        if (va <= 0.0f && bp_bc >= 0.0f && cp_cb >= 0.0f)
        {
            closest = b + bc * (bp_bc / (bp_bc + cp_cb));
            break;
        }

        r32 vb = n.dot(cp.cross(ap));

        if (vb <= 0.0f && ab_ac >= 0.0f && cp_ca >= 0.0f)
        {
            closest = a + ac * (ab_ac / (ab_ac + cp_ca));
            break;
        }

        r32 s = va + vb + vc;
        r32 u = va / s;
        r32 v = vb / s;
        r32 w = 1.0f - u - v;

        closest = a * u + b * v + c * w;
    }
    while (0);

    ckVec diff = sph.m_pos - closest;
    r32 sq_dist = diff.sqLength();

    if (sq_dist > (sph.m_radius - ckMath::EPSILON) * (sph.m_radius - ckMath::EPSILON))
    {
        return false;
    }

    if (cdt_info)
    {
        r32 dist = ckMath::sqrt(sq_dist);

        cdt_info->back_dir = (dist > ckMath::EPSILON) ? diff / dist : ckVec::X_UNIT;
        cdt_info->back_dist = sph.m_radius - dist;
        cdt_info->pos = closest - cdt_info->back_dir * (cdt_info->back_dist / 2.0f);
    }

    return true;
}
