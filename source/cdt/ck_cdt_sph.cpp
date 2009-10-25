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


#include "pg_cdt_all.h"


pgCdt::Sph::Sph()
{
    m_pos = pgVec::ZERO;
    m_radius = 0.0f;

    updateAABB();
}


const pgCdt::AABB& pgCdt::Sph::getAABB() const
{
    return m_aabb;
}


const pgVec& pgCdt::Sph::getPos() const
{
    return m_pos;
}


void pgCdt::Sph::setPos(const pgVec& pos)
{
    m_pos = pos;

    updateAABB();
}


r32 pgCdt::Sph::getRadius() const
{
    return m_radius;
}


void pgCdt::Sph::setRadius(r32 radius)
{
    if (radius < 0.0f)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_radius = radius;

    updateAABB();
}


void pgCdt::Sph::updateAABB()
{
    m_aabb.setBound( //
        pgVec(m_pos.x - m_radius, m_pos.y - m_radius, m_pos.z - m_radius), //
        pgVec(m_pos.x + m_radius, m_pos.y + m_radius, m_pos.z + m_radius));
}


bool pgCdt::collide(CdtInfo* cdt_info, const Sph& sph1, const Sph& sph2)
{
    if (!checkTouch(sph1.m_aabb, sph2.m_aabb))
    {
        return false;
    }

    pgVec diff = sph1.m_pos - sph2.m_pos;
    r32 sum_rad = sph1.m_radius + sph2.m_radius;
    r32 sq_dist = diff.sqLength();

    if (sq_dist > (sum_rad - pgMath::EPSILON) * (sum_rad - pgMath::EPSILON))
    {
        return false;
    }

    r32 dist = pgMath::sqrt(sq_dist);

    if (cdt_info)
    {
        cdt_info->back_dir = (dist > pgMath::EPSILON) ? diff / dist : pgVec::X_UNIT;
        cdt_info->back_dist = sum_rad - dist;
        cdt_info->pos = sph1.m_pos - cdt_info->back_dir * (sph1.m_radius - cdt_info->back_dist / 2.0f);
    }

    return true;
}


bool pgCdt::collide(CdtInfo* cdt_info, const Sph& sph, const Box& box)
{
    if (!checkTouch(sph.m_aabb, box.m_aabb))
    {
        return false;
    }

    pgVec diff = sph.m_pos - box.m_world.trans;
    pgVec closest = box.m_world.trans;

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

    if (sq_dist > (sph.m_radius - pgMath::EPSILON) * (sph.m_radius - pgMath::EPSILON))
    {
        return false;
    }

    if (cdt_info)
    {
        r32 dist = pgMath::sqrt(sq_dist);

        cdt_info->back_dir = (dist > pgMath::EPSILON) ? diff / dist : pgVec::X_UNIT;
        cdt_info->back_dist = sph.m_radius - dist;
        cdt_info->pos = closest - cdt_info->back_dir * (cdt_info->back_dist / 2.0f);
    }

    return true;
}


bool pgCdt::collide(CdtInfo* cdt_info, const Sph& sph, const Tri& tri)
{
    if (!checkTouch(sph.m_aabb, tri.m_aabb))
    {
        return false;
    }

    pgVec closest;

    do
    {
        const pgVec& p = sph.m_pos;
        const pgVec& a = tri.m_pos1;
        const pgVec& b = tri.m_pos2;
        const pgVec& c = tri.m_pos3;

        pgVec ap = p - a;
        pgVec ab = b - a;
        pgVec ac = c - a;

        r32 ap_ab = ap.dot(ab);
        r32 ab_ac = ap.dot(ac);

        if (ap_ab <= 0.0f && ab_ac <= 0.0f)
        {
            closest = a;
            break;
        }

        pgVec bp = p - b;
        pgVec bc = c - b;

        r32 bp_ba = -bp.dot(ab);
        r32 bp_bc = bp.dot(bc);

        if (bp_ba <= 0.0f && bp_bc <= 0.0f)
        {
            closest = b;
            break;
        }

        pgVec cp = p - c;

        r32 cp_ca = -cp.dot(ac);
        r32 cp_cb = -cp.dot(bc);

        if (cp_ca <= 0.0f && cp_cb <= 0.0f)
        {
            closest = c;
            break;
        }

        pgVec n = ab.cross(ac);

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

    pgVec diff = sph.m_pos - closest;
    r32 sq_dist = diff.sqLength();

    if (sq_dist > (sph.m_radius - pgMath::EPSILON) * (sph.m_radius - pgMath::EPSILON))
    {
        return false;
    }

    if (cdt_info)
    {
        r32 dist = pgMath::sqrt(sq_dist);

        cdt_info->back_dir = (dist > pgMath::EPSILON) ? diff / dist : pgVec::X_UNIT;
        cdt_info->back_dist = sph.m_radius - dist;
        cdt_info->pos = closest - cdt_info->back_dir * (cdt_info->back_dist / 2.0f);
    }

    return true;
}
