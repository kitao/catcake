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


const r32 BOX_ENLARGEMENT_RATE = 1.001f;


ckCdt::Box::Box()
{
    m_world = ckMat::UNIT;
    m_half_size = ckVec::ZERO;

    updateAABB();
}


const ckCdt::AABB& ckCdt::Box::getAABB() const
{
    return m_aabb;
}


const ckMat& ckCdt::Box::getWorld() const
{
    return m_world;
}


void ckCdt::Box::setWorld(const ckMat& world)
{
    m_world = world;

    updateAABB();
}


r32 ckCdt::Box::getWidth() const
{
    return m_half_size.x * 2.0f;
}


r32 ckCdt::Box::getHeight() const
{
    return m_half_size.y * 2.0f;
}


r32 ckCdt::Box::getDepth() const
{
    return m_half_size.z * 2.0f;
}


const ckVec& ckCdt::Box::getHalfSize() const
{
    return m_half_size;
}


void ckCdt::Box::setSize(r32 width, r32 height, r32 depth)
{
    if (width < 0.0f || height < 0.0f || depth < 0.0f)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_half_size.set(width / 2.0f, height / 2.0f, depth / 2.0f);

    updateAABB();
}


void ckCdt::Box::updateAABB()
{
    ckVec half_diag( //
        ckMath::abs(m_world.x_axis.x) * m_half_size.x + ckMath::abs(m_world.y_axis.x) * m_half_size.y + ckMath::abs(m_world.z_axis.x) * m_half_size.z, //
        ckMath::abs(m_world.x_axis.y) * m_half_size.x + ckMath::abs(m_world.y_axis.y) * m_half_size.y + ckMath::abs(m_world.z_axis.y) * m_half_size.z, //
        ckMath::abs(m_world.x_axis.z) * m_half_size.x + ckMath::abs(m_world.y_axis.z) * m_half_size.y + ckMath::abs(m_world.z_axis.z) * m_half_size.z);

    m_aabb.setBound(m_world.trans - half_diag, m_world.trans + half_diag);
}


bool ckCdt::collide(CdtInfo* cdt_info, const Box& box1, const Box& box2)
{
    if (!checkTouch(box1.m_aabb, box2.m_aabb))
    {
        return false;
    }

    ckMat local_b = box2.m_world.toLocalOf(box1.m_world);

    ckMat abs_local_b( //
        ckVec(ckMath::abs(local_b.x_axis.x), ckMath::abs(local_b.x_axis.y), ckMath::abs(local_b.x_axis.z)), //
        ckVec(ckMath::abs(local_b.y_axis.x), ckMath::abs(local_b.y_axis.y), ckMath::abs(local_b.y_axis.z)), //
        ckVec(ckMath::abs(local_b.z_axis.x), ckMath::abs(local_b.z_axis.y), ckMath::abs(local_b.z_axis.z)), //
        ckVec::ZERO);

    const ckVec& size_a = box1.m_half_size;
    const ckVec& size_b = box2.m_half_size;

    ckVec t = (box2.m_world.trans - box1.m_world.trans).toLocalOf_noTrans(box1.m_world);

    ckVec back_dir;
    r32 back_dist = 1000000.0f;

    // L = AX
    // L = AY
    // L = AZ
#define CHECK_BACK_DIST(ra, rb, tl, axis) \
    do \
    { \
        r32 cur_back_dist = (ra) + (rb) - ckMath::abs(tl); \
    \
        if (cur_back_dist < ckMath::EPSILON) \
        { \
            return false; \
        } \
        else if (cur_back_dist < back_dist) \
        { \
            back_dir = (tl < 0.0f) ? (axis) : -(axis); \
            back_dist = cur_back_dist; \
        } \
    } \
    while (false)

#define CHECK_SEPARATING_AXIS(compo, axis) \
    do \
    { \
        r32 ra = size_a.compo; \
        r32 rb = abs_local_b.x_axis.compo * size_b.x + abs_local_b.y_axis.compo * size_b.y + abs_local_b.z_axis.compo * size_b.z; \
        r32 tl = t.compo; \
    \
        CHECK_BACK_DIST(ra, rb, tl, axis); \
    } \
    while (false)

    CHECK_SEPARATING_AXIS(x, box1.m_world.x_axis);
    CHECK_SEPARATING_AXIS(y, box1.m_world.y_axis);
    CHECK_SEPARATING_AXIS(z, box1.m_world.z_axis);

#undef CHECK_SEPARATING_AXIS

    // L=BX
    // L=BY
    // L=BZ
#define CHECK_SEPARATING_AXIS(compo1, compo2, axis) \
    do \
    { \
        r32 ra = abs_local_b.compo2.dot(size_a); \
        r32 rb = size_b.compo1; \
        r32 tl = t.dot(local_b.compo2); \
    \
        CHECK_BACK_DIST(ra, rb, tl, axis); \
    } \
    while (false)

    CHECK_SEPARATING_AXIS(x, x_axis, box2.m_world.x_axis);
    CHECK_SEPARATING_AXIS(y, y_axis, box2.m_world.y_axis);
    CHECK_SEPARATING_AXIS(z, z_axis, box2.m_world.z_axis);

#undef CHECK_BACK_DIST
#undef CHECK_SEPARATING_AXIS

    ckVec sa_back_dir[9];
    r32 sa_back_dist[9];
    u8 sa_num = 0;

#define CHECK_SEPARATING_AXIS(lx, ly, lz) \
    do \
    { \
        r32 tl = t.x * (lx) + t.y * (ly) + t.z * (lz); \
        r32 cur_back_dist = ra + rb - ckMath::abs(tl); \
    \
        if (cur_back_dist < ckMath::EPSILON) \
        { \
            return false; \
        } \
    \
        sa_back_dir[sa_num] = (tl < 0.0f) ? ckVec(lx, ly, lz) : ckVec(-(lx), -(ly), -(lz)); \
        sa_back_dist[sa_num] = cur_back_dist; \
        sa_num++; \
    } \
    while (false)

#define IS_VALID_AXIS(a, b) ((a) > ckMath::EPSILON || (b) > ckMath::EPSILON)

    // L = AX * BX
    if (IS_VALID_AXIS(abs_local_b.x_axis.z, abs_local_b.x_axis.y))
    {
        r32 ra = size_a.y * abs_local_b.x_axis.z + size_a.z * abs_local_b.x_axis.y;
        r32 rb = size_b.y * abs_local_b.z_axis.x + size_b.z * abs_local_b.y_axis.x;

        CHECK_SEPARATING_AXIS(0.0f, -local_b.x_axis.z, local_b.x_axis.y);
    }

    // L = AX * BY
    if (IS_VALID_AXIS(abs_local_b.y_axis.z, abs_local_b.y_axis.y))
    {
        r32 ra = size_a.y * abs_local_b.y_axis.z + size_a.z * abs_local_b.y_axis.y;
        r32 rb = size_b.x * abs_local_b.z_axis.x + size_b.z * abs_local_b.x_axis.x;

        CHECK_SEPARATING_AXIS(0.0f, -local_b.y_axis.z, local_b.y_axis.y);
    }

    // L = AX * BZ
    if (IS_VALID_AXIS(abs_local_b.z_axis.z, abs_local_b.z_axis.y))
    {
        r32 ra = size_a.y * abs_local_b.z_axis.z + size_a.z * abs_local_b.z_axis.y;
        r32 rb = size_b.x * abs_local_b.y_axis.x + size_b.y * abs_local_b.x_axis.x;

        CHECK_SEPARATING_AXIS(0.0f, -local_b.z_axis.z, local_b.z_axis.y);
    }

    // L = AY * BX
    if (IS_VALID_AXIS(abs_local_b.x_axis.z, abs_local_b.x_axis.x))
    {
        r32 ra = size_a.x * abs_local_b.x_axis.z + size_a.z * abs_local_b.x_axis.x;
        r32 rb = size_b.y * abs_local_b.z_axis.y + size_b.z * abs_local_b.y_axis.y;

        CHECK_SEPARATING_AXIS(local_b.x_axis.z, 0.0f, -local_b.x_axis.x);
    }

    // L = AY * BY
    if (IS_VALID_AXIS(abs_local_b.y_axis.z, abs_local_b.y_axis.x))
    {
        r32 ra = size_a.x * abs_local_b.y_axis.z + size_a.z * abs_local_b.y_axis.x;
        r32 rb = size_b.x * abs_local_b.z_axis.y + size_b.z * abs_local_b.x_axis.y;

        CHECK_SEPARATING_AXIS(local_b.y_axis.z, 0.0f, -local_b.y_axis.x);
    }

    // L = AY * BZ
    if (IS_VALID_AXIS(abs_local_b.z_axis.z, abs_local_b.z_axis.x))
    {
        r32 ra = size_a.x * abs_local_b.z_axis.z + size_a.z * abs_local_b.z_axis.x;
        r32 rb = size_b.x * abs_local_b.y_axis.y + size_b.y * abs_local_b.x_axis.y;

        CHECK_SEPARATING_AXIS(local_b.z_axis.z, 0.0f, -local_b.z_axis.x);
    }

    // L = AZ * BX
    if (IS_VALID_AXIS(abs_local_b.x_axis.y, abs_local_b.x_axis.x))
    {
        r32 ra = size_a.x * abs_local_b.x_axis.y + size_a.y * abs_local_b.x_axis.x;
        r32 rb = size_b.y * abs_local_b.z_axis.z + size_b.z * abs_local_b.y_axis.z;

        CHECK_SEPARATING_AXIS(-local_b.x_axis.y, local_b.x_axis.x, 0.0f);
    }

    // L = AZ * BY
    if (IS_VALID_AXIS(abs_local_b.y_axis.y, abs_local_b.y_axis.x))
    {
        r32 ra = size_a.x * abs_local_b.y_axis.y + size_a.y * abs_local_b.y_axis.x;
        r32 rb = size_b.x * abs_local_b.z_axis.z + size_b.z * abs_local_b.x_axis.z;

        CHECK_SEPARATING_AXIS(-local_b.y_axis.y, local_b.y_axis.x, 0.0f);
    }

    // L = AZ * BZ
    if (IS_VALID_AXIS(abs_local_b.z_axis.y, abs_local_b.z_axis.x))
    {
        r32 ra = size_a.x * abs_local_b.z_axis.y + size_a.y * abs_local_b.z_axis.x;
        r32 rb = size_b.x * abs_local_b.y_axis.z + size_b.y * abs_local_b.x_axis.z;

        CHECK_SEPARATING_AXIS(-local_b.z_axis.y, local_b.z_axis.x, 0.0f);
    }

#undef CHECK_SEPARATING_AXIS
#undef IS_VALID_AXIS

    if (!cdt_info)
    {
        return true;
    }

    /*
        calculate cdt_info.back_dir and cdt_info.back_dist
    */
    bool is_changed = false;

    for (s32 i = 0; i < sa_num; i++)
    {
        if (sa_back_dist[i] > ckMath::EPSILON)
        {
            r32 length = sa_back_dir[i].length();

            if (length > ckMath::EPSILON)
            {
                sa_back_dist[i] /= length;

                if (sa_back_dist[i] < back_dist)
                {
                    is_changed = true;

                    back_dir = sa_back_dir[i] / length;
                    back_dist = sa_back_dist[i];
                }
            }
        }
    }

    if (is_changed)
    {
        back_dir = back_dir.toGlobalFrom_noTrans(box1.m_world);
    }

    /*
        calculate cdt_info.pos
    */
    ckVec size_b2 = size_b * BOX_ENLARGEMENT_RATE;

    ckVec sx = local_b.x_axis * size_b2.x;
    ckVec sy = local_b.y_axis * size_b2.y;
    ckVec sz = local_b.z_axis * size_b2.z;

    ckVec inter_pos[8];
    u8 inter_num = 0;

    for (s32 i = 0; i < 8; i++)
    {
        ckVec cur_pos = local_b.trans + ((i % 2 == 0) ? sx : -sx) + ((i % 4 < 2) ? sy : -sy) + ((i < 4) ? sz : -sz);

        if (cur_pos.x <= size_a.x && cur_pos.x >= -size_a.x && //
            cur_pos.y <= size_a.y && cur_pos.y >= -size_a.y && //
            cur_pos.z <= size_a.z && cur_pos.z >= -size_a.z)
        {
            cur_pos = cur_pos.toGlobalFrom(box1.m_world);
            inter_pos[inter_num] = cur_pos;
            inter_num++;
        }
    }

    if (inter_num > 0)
    {
        cdt_info->pos = inter_pos[0];

        if (inter_num > 1)
        {
            for (s32 i = 1; i < inter_num; i++)
            {
                cdt_info->pos += inter_pos[i];
            }

            cdt_info->pos /= inter_num;
        }

        cdt_info->back_dir = back_dir;
        cdt_info->back_dist = back_dist;

        return true;
    }

    ////////

    ckVec vert[8];
    ckMat local_a = box1.m_world.toLocalOf(box2.m_world);

    sx = local_a.x_axis * size_a.x;
    sy = local_a.y_axis * size_a.y;
    sz = local_a.z_axis * size_a.z;

    inter_num = 0;

    for (s32 i = 0; i < 8; i++)
    {
        ckVec cur_pos = vert[i] = local_a.trans + ((i % 2 == 0) ? sx : -sx) + ((i % 4 < 2) ? sy : -sy) + ((i < 4) ? sz : -sz);

        if (cur_pos.x <= size_b2.x && cur_pos.x >= -size_b2.x && //
            cur_pos.y <= size_b2.y && cur_pos.y >= -size_b2.y && //
            cur_pos.z <= size_b2.z && cur_pos.z >= -size_b2.z)
        {
            cur_pos = cur_pos.toGlobalFrom(box2.m_world);
            inter_pos[inter_num] = cur_pos;
            inter_num++;
        }
    }

    if (inter_num > 0)
    {
        cdt_info->pos = inter_pos[0];

        if (inter_num > 1)
        {
            for (s32 i = 1; i < inter_num; i++)
            {
                cdt_info->pos += inter_pos[i];
            }

            cdt_info->pos /= inter_num;
        }

        cdt_info->back_dir = back_dir;
        cdt_info->back_dist = back_dist;

        return true;
    }

    ////////

    r32 min_dist, max_dist;
    ckVec size_wa = size_a * 2.0f;

    inter_num = 0;

#define CALC_INTERSECT(ray_pos, ray_dir, compo) \
    do \
    { \
        if (intersectLocalBox(&min_dist, &max_dist, (ray_pos), (ray_dir), size_b2) && min_dist <= size_wa.compo) \
        { \
            inter_pos[inter_num] = ray_pos + ray_dir * ((min_dist + ckMath::min(max_dist, size_wa.compo)) / 2.0f); \
            inter_num++; \
        } \
    } \
    while (false)

    CALC_INTERSECT(vert[1], local_a.x_axis, x);
    CALC_INTERSECT(vert[3], local_a.x_axis, x);
    CALC_INTERSECT(vert[5], local_a.x_axis, x);
    CALC_INTERSECT(vert[7], local_a.x_axis, x);

    if (inter_num == 0)
    {
        CALC_INTERSECT(vert[2], local_a.y_axis, y);
        CALC_INTERSECT(vert[3], local_a.y_axis, y);
        CALC_INTERSECT(vert[6], local_a.y_axis, y);
        CALC_INTERSECT(vert[7], local_a.y_axis, y);
    }

    if (inter_num == 0)
    {
        CALC_INTERSECT(vert[4], local_a.z_axis, z);
        CALC_INTERSECT(vert[5], local_a.z_axis, z);
        CALC_INTERSECT(vert[6], local_a.z_axis, z);
        CALC_INTERSECT(vert[7], local_a.z_axis, z);
    }

#undef CALC_INTERSECT

    if (inter_num > 0)
    {
        cdt_info->pos = inter_pos[0];

        if (inter_num > 1)
        {
            for (s32 i = 1; i < inter_num; i++)
            {
                cdt_info->pos += inter_pos[i];
            }

            cdt_info->pos /= inter_num;
        }

        cdt_info->pos = cdt_info->pos.toGlobalFrom(box2.m_world) + back_dir * (back_dist / 2.0f);
        cdt_info->back_dir = back_dir;
        cdt_info->back_dist = back_dist;

        return true;
    }

    cdt_info->pos = box1.m_world.trans;
    cdt_info->back_dir = ckVec::X_UNIT;
    cdt_info->back_dist = 0.0f;

    return true; // unexpected case
}


bool ckCdt::collide(CdtInfo* cdt_info, const Box& box, const Sph& sph)
{
    bool res = collide(cdt_info, sph, box);

    if (cdt_info)
    {
        cdt_info->back_dir = -cdt_info->back_dir;
    }

    return res;
}


bool ckCdt::collide(CdtInfo* cdt_info, const Box& box, const Tri& tri)
{
    if (!checkTouch(box.m_aabb, tri.m_aabb))
    {
        return false;
    }

    ckVec back_dir;
    r32 back_dist = 1000000.0f;

    ckVec center = (tri.m_pos1 + tri.m_pos2 + tri.m_pos3) / 3.0f;

    // L = Box.X
    // L = Box.Y
    // L = Box.Z
    ckVec a = tri.m_pos1.toLocalOf(box.m_world);
    ckVec b = tri.m_pos2.toLocalOf(box.m_world);
    ckVec c = tri.m_pos3.toLocalOf(box.m_world);

    r32 min_dist, max_dist;

#define CHECK_SEPARATING_AXIS(compo, axis) \
    do \
    { \
        min_dist = (a.compo < b.compo && a.compo < c.compo) ? a.compo : ((b.compo < c.compo) ? b.compo : c.compo); \
    \
        if (min_dist > box.m_half_size.compo - ckMath::EPSILON) \
        { \
            return false; \
        } \
    \
        r32 cur_back_dist = box.m_half_size.compo - min_dist; \
    \
        if (cur_back_dist < back_dist) \
        { \
            back_dir = -(axis); \
            back_dist = cur_back_dist; \
        } \
    \
        max_dist = (a.compo > b.compo && a.compo > c.compo) ? a.compo : ((b.compo > c.compo) ? b.compo : c.compo); \
    \
        if (max_dist < -box.m_half_size.compo + ckMath::EPSILON) \
        { \
            return false; \
        } \
    \
        cur_back_dist = max_dist + box.m_half_size.compo; \
    \
        if (cur_back_dist < back_dist) \
        { \
            back_dir = (axis); \
            back_dist = cur_back_dist; \
        } \
    } \
    while (false)

    CHECK_SEPARATING_AXIS(x, box.m_world.x_axis);
    CHECK_SEPARATING_AXIS(y, box.m_world.y_axis);
    CHECK_SEPARATING_AXIS(z, box.m_world.z_axis);

#undef CHECK_SEPARATING_AXIS

    ckVec p = center.toLocalOf(box.m_world);

    ckVec ap = p - a;
    ckVec bp = p - b;
    ckVec cp = p - c;

    ckVec sa_back_dir[10];
    r32 sa_back_dist[10];
    u8 sa_num;

    // L = Normal of Triangle
    {
        ckVec n = (a - b).cross(a - c);

        r32 ra = 0.0f;
        r32 rb = ckMath::abs(n.x) * box.m_half_size.x + ckMath::abs(n.y) * box.m_half_size.y + ckMath::abs(n.z) * box.m_half_size.z;
        r32 tl = p.dot(n);

        r32 cur_back_dist = ra + rb - ckMath::abs(tl);

        if (cur_back_dist < ckMath::EPSILON)
        {
            return false;
        }

        sa_back_dir[0] = (tl < 0.0f) ? n : -n;
        sa_back_dist[0] = cur_back_dist;
        sa_num = 1;
    }

#define CHECK_SEPARATING_AXIS(lx, ly, lz, abs_lx, abs_ly, abs_lz) \
    do \
    { \
        if (abs_lx > ckMath::EPSILON || abs_ly > ckMath::EPSILON || abs_lz > ckMath::EPSILON) \
        { \
            r32 apl = a.x * (lx) + a.y * (ly) + a.z * (lz); \
            r32 bpl = b.x * (lx) + b.y * (ly) + b.z * (lz); \
            r32 cpl = c.x * (lx) + c.y * (ly) + c.z * (lz); \
    \
            r32 ra = abs_lx * box.m_half_size.x + abs_ly * box.m_half_size.y + abs_lz * box.m_half_size.z; \
            max_dist = (apl > bpl && apl > cpl) ? apl : ((bpl > cpl) ? bpl : cpl); \
            min_dist = (apl < bpl && apl < cpl) ? apl : ((bpl < cpl) ? bpl : cpl); \
    \
            r32 cur_back_dist1 = ra + max_dist; \
            r32 cur_back_dist2 = ra - min_dist; \
    \
            if (cur_back_dist1 < ckMath::EPSILON || cur_back_dist2 < ckMath::EPSILON) \
            { \
                return false; \
            } \
    \
            r32 tl = p.x * (lx) + p.y * (ly) + p.z * (lz); \
    \
            sa_back_dir[sa_num] = (tl < 0.0f) ? ckVec(lx, ly, lz) : ckVec(-(lx), -(ly), -(lz)); \
            sa_back_dist[sa_num] = ckMath::min(cur_back_dist1, cur_back_dist2); \
            sa_num++; \
        } \
    } \
    while (false)

#define CHECK_SEPARATING_AXIES(edge) \
    do \
    { \
        CHECK_SEPARATING_AXIS(0.0f, -(edge).z, (edge).y, 0.0f, ckMath::abs((edge).z), ckMath::abs((edge).y)); \
        CHECK_SEPARATING_AXIS((edge).z, 0.0f, -(edge).x, ckMath::abs((edge).z), 0.0f, ckMath::abs((edge).x)); \
        CHECK_SEPARATING_AXIS(-(edge).y, (edge).x, 0.0f, ckMath::abs((edge).y), ckMath::abs((edge).x), 0.0f); \
    } \
    while (false)

    // L = Box.X * AB
    // L = Box.Y * AB
    // L = Box.Z * AB
    ckVec ab = b - a;
    CHECK_SEPARATING_AXIES(ab);

    // L = Box.X * BC
    // L = Box.Y * BC
    // L = Box.Z * BC
    ckVec bc = c - b;
    CHECK_SEPARATING_AXIES(bc);

    // L = Box.X * CA
    // L = Box.Y * CA
    // L = Box.Z * CA
    ckVec ca = a - c;
    CHECK_SEPARATING_AXIES(ca);

#undef CHECK_SEPARATING_AXIES
#undef CHECK_SEPARATING_AXIS

    if (!cdt_info)
    {
        return true;
    }

    /*
        calculate cdt_info.back_dir and cdt_info.back_dist
    */
    bool is_changed = false;

    for (s32 i = 0; i < sa_num; i++)
    {
        if (sa_back_dist[i] > ckMath::EPSILON)
        {
            r32 length = sa_back_dir[i].length();

            if (length > ckMath::EPSILON)
            {
                sa_back_dist[i] /= length;

                if (sa_back_dist[i] < back_dist)
                {
                    is_changed = true;

                    back_dir = sa_back_dir[i] / length;
                    back_dist = sa_back_dist[i];
                }
            }
        }
    }

    if (is_changed)
    {
        back_dir = back_dir.toGlobalFrom_noTrans(box.m_world);
    }

    /*
        calculate cdt_info.pos
    */
    ckVec inter_pos[16];
    u8 inter_num = 0;

    ckVec box_half_size2 = box.m_half_size * BOX_ENLARGEMENT_RATE;

#define CHECK_VERT_IN_BOX(local_vert, global_vert) \
    do \
    { \
        if ((local_vert).x <= box_half_size2.x && (local_vert).x >= -box_half_size2.x && /**/ \
            (local_vert).y <= box_half_size2.y && (local_vert).y >= -box_half_size2.y && /**/ \
            (local_vert).z <= box_half_size2.z && (local_vert).z >= -box_half_size2.z) \
        { \
            inter_pos[inter_num] = global_vert; \
            inter_num++; \
        } \
    } \
    while (false)

#define CALC_INTERSECT_POS() \
    do \
    { \
        if (inter_num > 0) \
        { \
            cdt_info->pos = inter_pos[0]; \
    \
            if (inter_num > 1) \
            { \
                for (s32 i = 1; i < inter_num; i++) \
                { \
                    cdt_info->pos += inter_pos[i]; \
                } \
    \
                cdt_info->pos /= inter_num; \
            } \
    \
            cdt_info->back_dir = back_dir; \
            cdt_info->back_dist = back_dist; \
    \
            return true; \
        } \
    } \
    while (false)

    CHECK_VERT_IN_BOX(a, tri.m_pos1);
    CHECK_VERT_IN_BOX(b, tri.m_pos2);
    CHECK_VERT_IN_BOX(c, tri.m_pos3);

    CALC_INTERSECT_POS();

    ////////

    inter_num = 0;

    if (intersectLocalBox(&min_dist, &max_dist, a, ab, box_half_size2) && min_dist <= 1.0f)
    {
        inter_pos[inter_num] = (a + ab * ((min_dist + ((max_dist > 1.0f) ? 1.0f : max_dist)) / 2.0f)).toGlobalFrom(box.m_world);
        inter_num++;
    }

    if (intersectLocalBox(&min_dist, &max_dist, b, bc, box_half_size2) && min_dist <= 1.0f)
    {
        inter_pos[inter_num] = (b + bc * ((min_dist + ((max_dist > 1.0f) ? 1.0f : max_dist)) / 2.0f)).toGlobalFrom(box.m_world);
        inter_num++;
    }

    if (intersectLocalBox(&min_dist, &max_dist, c, ca, box_half_size2) && min_dist <= 1.0f)
    {
        inter_pos[inter_num] = (c + ca * ((min_dist + ((max_dist > 1.0f) ? 1.0f : max_dist)) / 2.0f)).toGlobalFrom(box.m_world);
        inter_num++;
    }

    CALC_INTERSECT_POS();

    ////////

    inter_num = 0;

    r32 dist;
    ckVec ray_pos;

#define INTERSECT_TRIANGLE(ray_dir, max_dist) \
    do \
    { \
        if (intersectTri(&dist, ray_pos, ray_dir, tri) && dist <= (max_dist)) \
        { \
            inter_pos[inter_num] = ray_pos + (ray_dir) * dist; \
            inter_num++; \
        } \
    } \
    while (false)

    ckVec vec_x = box.m_world.x_axis * box_half_size2.x;
    ckVec vec_y = box.m_world.y_axis * box_half_size2.y;
    ckVec vec_z = box.m_world.z_axis * box_half_size2.z;

    r32 size_x = box_half_size2.x * 2.0f;
    r32 size_y = box_half_size2.y * 2.0f;
    r32 size_z = box_half_size2.z * 2.0f;

    ray_pos = box.m_world.trans - vec_x + vec_y + vec_z;
    INTERSECT_TRIANGLE(box.m_world.x_axis, size_x);

    ray_pos = box.m_world.trans - vec_x - vec_y + vec_z;
    INTERSECT_TRIANGLE(box.m_world.x_axis, size_x);

    ray_pos = box.m_world.trans - vec_x + vec_y - vec_z;
    INTERSECT_TRIANGLE(box.m_world.x_axis, size_x);

    ray_pos = box.m_world.trans - vec_x - vec_y - vec_z;
    INTERSECT_TRIANGLE(box.m_world.x_axis, size_x);

    ray_pos = box.m_world.trans + vec_x - vec_y + vec_z;
    INTERSECT_TRIANGLE(box.m_world.y_axis, size_y);

    ray_pos = box.m_world.trans - vec_x - vec_y + vec_z;
    INTERSECT_TRIANGLE(box.m_world.y_axis, size_y);

    ray_pos = box.m_world.trans + vec_x - vec_y - vec_z;
    INTERSECT_TRIANGLE(box.m_world.y_axis, size_y);

    ray_pos = box.m_world.trans - vec_x - vec_y - vec_z;
    INTERSECT_TRIANGLE(box.m_world.y_axis, size_y);

    ray_pos = box.m_world.trans + vec_x + vec_y - vec_z;
    INTERSECT_TRIANGLE(box.m_world.z_axis, size_z);

    ray_pos = box.m_world.trans - vec_x + vec_y - vec_z;
    INTERSECT_TRIANGLE(box.m_world.z_axis, size_z);

    ray_pos = box.m_world.trans + vec_x - vec_y - vec_z;
    INTERSECT_TRIANGLE(box.m_world.z_axis, size_z);

    ray_pos = box.m_world.trans - vec_x - vec_y - vec_z;
    INTERSECT_TRIANGLE(box.m_world.z_axis, size_z);

    CALC_INTERSECT_POS();

    cdt_info->pos = box.m_world.trans;
    cdt_info->back_dir = ckVec::X_UNIT;
    cdt_info->back_dist = 0.0f;

    return true; // unexpected case
}
