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


ckCdt::Ray::Ray()
{
    m_from = m_to = ckVec::ZERO;

    updateAABB();
}


const ckCdt::AABB& ckCdt::Ray::getAABB() const
{
    return m_aabb;
}


const ckVec& ckCdt::Ray::getFrom() const
{
    return m_from;
}


const ckVec& ckCdt::Ray::getTo() const
{
    return m_to;
}


void ckCdt::Ray::setPos(const ckVec& from, const ckVec& to)
{
    m_from = from;
    m_to = to;

    updateAABB();
}


void ckCdt::Ray::updateAABB()
{
    ckVec min, max;

    if (m_from.x < m_to.x)
    {
        min.x = m_from.x;
        max.x = m_to.x;
    }
    else
    {
        min.x = m_to.x;
        max.x = m_from.x;
    }

    if (m_from.y < m_to.y)
    {
        min.y = m_from.y;
        max.y = m_to.y;
    }
    else
    {
        min.y = m_to.y;
        max.y = m_from.y;
    }

    if (m_from.z < m_to.z)
    {
        min.z = m_from.z;
        max.z = m_to.z;
    }
    else
    {
        min.z = m_to.z;
        max.z = m_from.z;
    }

    m_aabb.setBound(min, max);
}


bool ckCdt::intersect(ckVec* pos, const Ray& ray, const Sph& sph)
{
    if (!checkTouch(ray.m_aabb, sph.m_aabb))
    {
        return false;
    }

    ckVec m = sph.getPos() - ray.m_from;
    r32 c = m.sqLength() - sph.getRadius() * sph.getRadius();

    if (c <= 0.0f)
    {
        if (pos)
        {
            *pos = ray.m_from;
        }

        return true;
    }

    ckVec d = ray.m_to - ray.m_from;
    r32 b = m.dot(d);

    if (b < 0.0f)
    {
        return false;
    }

    r32 sq_d = d.sqLength();

    c /= sq_d;
    b /= sq_d;

    r32 discr = b * b - c;

    if (discr < 0.0f)
    {
        return false;
    }

    r32 dist = b - ckMath::sqrt(discr);

    if (dist > 1.0f)
    {
        return false;
    }

    if (pos)
    {
        *pos = ray.m_from + d * dist;
    }

    return true;
}


bool ckCdt::intersect(ckVec* pos, const Ray& ray, const Box& box)
{
    if (!checkTouch(ray.m_aabb, box.m_aabb))
    {
        return false;
    }

    ckVec rel_pos = ray.m_from.toLocalOf(box.m_world);
    ckVec dir = ray.m_to - ray.m_from;
    ckVec rel_dir = dir.toLocalOf_noTrans(box.m_world);

    r32 min_dist, max_dist;

    if (intersectLocalBox(&min_dist, &max_dist, rel_pos, rel_dir, box.m_half_size) && min_dist <= 1.0f)
    {
        if (pos)
        {
            *pos = ray.m_from + dir * min_dist;
        }

        return true;
    }
    else
    {
        return false;
    }
}


bool ckCdt::intersect(ckVec* pos, const Ray& ray, const Tri& tri)
{
    if (!checkTouch(ray.m_aabb, tri.m_aabb))
    {
        return false;
    }

    ckVec ray_dir = ray.m_to - ray.m_from;

    r32 dist;

    if (intersectTri(&dist, ray.m_from, ray_dir, tri) && dist <= 1.0f)
    {
        if (pos)
        {
            *pos = ray.m_from + ray_dir * dist;
        }

        return true;
    }
    else
    {
        return false;
    }
}


bool ckCdt::intersectLocalBox(r32* min_dist, r32* max_dist, const ckVec& local_ray_pos, const ckVec& local_ray_dir, const ckVec& box_half_size)
{
    *min_dist = -1000000.0f;
    *max_dist = 1000000.0f;

    const r32* p = &local_ray_pos.x;
    const r32* d = &local_ray_dir.x;
    const r32* s = &box_half_size.x;

    for (s32 i = 0; i < 3; i++, p++, d++, s++)
    {
        if (*d < ckMath::EPSILON && *d > -ckMath::EPSILON)
        {
            if (*p < -*s || *p > *s)
            {
                return false;
            }
        }
        else
        {
            r32 ood = 1.0f / *d;
            r32 t1 = (*s - *p) * ood;
            r32 t2 = (-*s - *p) * ood;

            if (t1 > t2)
            {
                r32 t3 = t1;
                t1 = t2;
                t2 = t3;
            }

            if (t1 > *min_dist)
            {
                *min_dist = t1;
            }

            if (t2 < *max_dist)
            {
                *max_dist = t2;
            }

            if (*min_dist > *max_dist)
            {
                return false;
            }
        }
    }

    if (*max_dist < 0.0f)
    {
        return false;
    }

    if (*min_dist < 0.0f)
    {
        *min_dist = 0.0f;
    }

    return true;
}


bool ckCdt::intersectTri(r32* dist, const ckVec& ray_pos, const ckVec& ray_dir, const Tri& tri)
{
    ckVec ab = tri.m_pos2 - tri.m_pos1;
    ckVec ac = tri.m_pos3 - tri.m_pos1;
    ckVec pvec = ray_dir.cross(ac);

    r32 det = ab.dot(pvec);

    if (det < ckMath::EPSILON && det > -ckMath::EPSILON)
    {
        return false;
    }

    ckVec tvec = ray_pos - tri.m_pos1;
    r32 inv_det = 1.0f / det;
    r32 u = tvec.dot(pvec) * inv_det;

    if (u < 0.0f || u > 1.0f)
    {
        return false;
    }

    ckVec qvec = tvec.cross(ab);
    r32 v = ray_dir.dot(qvec) * inv_det;

    if (v < 0.0f || u + v > 1.0f)
    {
        return false;
    }

    *dist = ac.dot(qvec) * inv_det;

    return (*dist < 0.0f) ? false : true;
}
