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


#include "pg_math_all.h"

#include "pg_mem_all.h"


const pgVec pgVec::ZERO(0.0f, 0.0f, 0.0f);
const pgVec pgVec::X_UNIT(1.0f, 0.0f, 0.0f);
const pgVec pgVec::Y_UNIT(0.0f, 1.0f, 0.0f);
const pgVec pgVec::Z_UNIT(0.0f, 0.0f, 1.0f);


pgVec::pgVec() {}


pgVec::pgVec(r32 x_, r32 y_, r32 z_)
{
    x = x_;
    y = y_;
    z = z_;
}


void pgVec::set(r32 x_, r32 y_, r32 z_)
{
    x = x_;
    y = y_;
    z = z_;
}


pgVec pgVec::operator+(const pgVec& vec) const
{
    return pgVec(x + vec.x, y + vec.y, z + vec.z);
}


void pgVec::operator+=(const pgVec& vec)
{
    x += vec.x;
    y += vec.y;
    z += vec.z;
}


pgVec pgVec::operator-() const
{
    return pgVec(-x, -y, -z);
}


pgVec pgVec::operator-(const pgVec& vec) const
{
    return pgVec(x - vec.x, y - vec.y, z - vec.z);
}


void pgVec::operator-=(const pgVec& vec)
{
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
}


pgVec pgVec::operator*(r32 s) const
{
    return pgVec(x * s, y * s, z * s);
}


pgVec operator*(r32 s, const pgVec& vec)
{
    return pgVec(vec.x * s, vec.y * s, vec.z * s);
}


void pgVec::operator*=(r32 s)
{
    x *= s;
    y *= s;
    z *= s;
}


pgVec pgVec::operator/(r32 s) const
{
    r32 inv_s = 1.0f / s;

    return pgVec(x * inv_s, y * inv_s, z * inv_s);
}


void pgVec::operator/=(r32 s)
{
    r32 inv_s = 1.0f / s;

    x *= inv_s;
    y *= inv_s;
    z *= inv_s;
}


r32 pgVec::length() const
{
    return pgMath::sqrt(x * x + y * y + z * z);
}


r32 pgVec::sqLength() const
{
    return x * x + y * y + z * z;
}


r32 pgVec::dist(const pgVec& vec) const
{
    return (*this - vec).length();
}


r32 pgVec::sqDist(const pgVec& vec) const
{
    return (*this - vec).sqLength();
}


r32 pgVec::dot(const pgVec& vec) const
{
    return x * vec.x + y * vec.y + z * vec.z;
}


pgVec pgVec::cross(const pgVec& vec) const
{
    return pgVec(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
}


pgVec pgVec::rotateX_r32(r32 deg) const
{
    r32 sin = pgMath::sin_r32(deg);
    r32 cos = pgMath::cos_r32(deg);

    return pgVec(x, y * cos - z * sin, z * cos + y * sin);
}


pgVec pgVec::rotateY_r32(r32 deg) const
{
    r32 sin = pgMath::sin_r32(deg);
    r32 cos = pgMath::cos_r32(deg);

    return pgVec(x * cos + z * sin, y, z * cos - x * sin);
}


pgVec pgVec::rotateZ_r32(r32 deg) const
{
    r32 sin = pgMath::sin_r32(deg);
    r32 cos = pgMath::cos_r32(deg);

    return pgVec(x * cos - y * sin, y * cos + x * sin, z);
}


pgVec pgVec::rotateX_s32(s32 deg) const
{
    r32 sin = pgMath::sin_s32(deg);
    r32 cos = pgMath::cos_s32(deg);

    return pgVec(x, y * cos - z * sin, z * cos + y * sin);
}


pgVec pgVec::rotateY_s32(s32 deg) const
{
    r32 sin = pgMath::sin_s32(deg);
    r32 cos = pgMath::cos_s32(deg);

    return pgVec(x * cos + z * sin, y, z * cos - x * sin);
}


pgVec pgVec::rotateZ_s32(s32 deg) const
{
    r32 sin = pgMath::sin_s32(deg);
    r32 cos = pgMath::cos_s32(deg);

    return pgVec(x * cos - y * sin, y * cos + x * sin, z);
}


pgVec pgVec::normalize() const
{
    return *this / length();
}


pgVec pgVec::interp(const pgVec& to, r32 ratio) const
{
    if (ratio < pgMath::EPSILON)
    {
        return *this;
    }
    else if (ratio > 1.0f - pgMath::EPSILON)
    {
        return to;
    }
    else
    {
        return *this * (1.0f - ratio) + to * ratio;
    }
}


pgVec pgVec::slerp(const pgVec& to, r32 ratio) const
{
    /*
        the length of each vector must be 1.0
    */

    if (ratio < pgMath::EPSILON)
    {
        return *this;
    }
    else if (ratio > 1.0f - pgMath::EPSILON)
    {
        return to;
    }
    else
    {
        r32 a = pgMath::acos(dot(to));
        r32 b = a * ratio;

        if (pgMath::abs(a) > 180.0f - pgMath::EPSILON)
        {
            return *this;
        }
        else
        {
            return pgMath::cos_r32(b) * *this + (to - pgMath::cos_r32(a) * *this) * (pgMath::sin_r32(b) / pgMath::sin_r32(a));
        }
    }
}


pgVec pgVec::toLocalOf(const pgMat& mat) const
{
    pgVec vec = *this - mat.trans;

    return pgVec( //
        vec.dot(mat.x_axis) / mat.x_axis.sqLength(), //
        vec.dot(mat.y_axis) / mat.y_axis.sqLength(), //
        vec.dot(mat.z_axis) / mat.z_axis.sqLength());
}


pgVec pgVec::toGlobalFrom(const pgMat& mat) const
{
    return mat.x_axis * x + mat.y_axis * y + mat.z_axis * z + mat.trans;
}


pgVec pgVec::toLocalOf_noTrans(const pgMat& mat) const
{
    return pgVec( //
        dot(mat.x_axis) / mat.x_axis.sqLength(), //
        dot(mat.y_axis) / mat.y_axis.sqLength(), //
        dot(mat.z_axis) / mat.z_axis.sqLength());
}


pgVec pgVec::toGlobalFrom_noTrans(const pgMat& mat) const
{
    return mat.x_axis * x + mat.y_axis * y + mat.z_axis * z;
}


void pgVec::toR32x4(r32* r32x4, r32 w) const
{
    r32x4[0] = x;
    r32x4[1] = y;
    r32x4[2] = z;
    r32x4[3] = w;
}


pgVec pgVec::fromR32x4(const r32* r32x4)
{
    return pgVec(r32x4[0], r32x4[1], r32x4[2]);
}


void pgVec::mulR32x4(r32* res_r32x4, const r32* lhs_r32x16, const r32* rhs_r32x4)
{
    r32 v[4];

    v[0] = lhs_r32x16[0] * rhs_r32x4[0] + lhs_r32x16[4] * rhs_r32x4[1] + lhs_r32x16[8] * rhs_r32x4[2] + lhs_r32x16[12] * rhs_r32x4[3];
    v[1] = lhs_r32x16[1] * rhs_r32x4[0] + lhs_r32x16[5] * rhs_r32x4[1] + lhs_r32x16[9] * rhs_r32x4[2] + lhs_r32x16[13] * rhs_r32x4[3];
    v[2] = lhs_r32x16[2] * rhs_r32x4[0] + lhs_r32x16[6] * rhs_r32x4[1] + lhs_r32x16[10] * rhs_r32x4[2] + lhs_r32x16[14] * rhs_r32x4[3];
    v[3] = lhs_r32x16[3] * rhs_r32x4[0] + lhs_r32x16[7] * rhs_r32x4[1] + lhs_r32x16[11] * rhs_r32x4[2] + lhs_r32x16[15] * rhs_r32x4[3];

    pgMemMgr::memcpy(res_r32x4, v, sizeof(v));
}
