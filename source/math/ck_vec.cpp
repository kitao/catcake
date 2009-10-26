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


#include "ck_math_all.h"

#include "ck_mem_all.h"


const ckVec ckVec::ZERO(0.0f, 0.0f, 0.0f);
const ckVec ckVec::X_UNIT(1.0f, 0.0f, 0.0f);
const ckVec ckVec::Y_UNIT(0.0f, 1.0f, 0.0f);
const ckVec ckVec::Z_UNIT(0.0f, 0.0f, 1.0f);


ckVec::ckVec() {}


ckVec::ckVec(r32 x_, r32 y_, r32 z_)
{
    x = x_;
    y = y_;
    z = z_;
}


void ckVec::set(r32 x_, r32 y_, r32 z_)
{
    x = x_;
    y = y_;
    z = z_;
}


ckVec ckVec::operator+(const ckVec& vec) const
{
    return ckVec(x + vec.x, y + vec.y, z + vec.z);
}


void ckVec::operator+=(const ckVec& vec)
{
    x += vec.x;
    y += vec.y;
    z += vec.z;
}


ckVec ckVec::operator-() const
{
    return ckVec(-x, -y, -z);
}


ckVec ckVec::operator-(const ckVec& vec) const
{
    return ckVec(x - vec.x, y - vec.y, z - vec.z);
}


void ckVec::operator-=(const ckVec& vec)
{
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
}


ckVec ckVec::operator*(r32 s) const
{
    return ckVec(x * s, y * s, z * s);
}


ckVec operator*(r32 s, const ckVec& vec)
{
    return ckVec(vec.x * s, vec.y * s, vec.z * s);
}


void ckVec::operator*=(r32 s)
{
    x *= s;
    y *= s;
    z *= s;
}


ckVec ckVec::operator/(r32 s) const
{
    r32 inv_s = 1.0f / s;

    return ckVec(x * inv_s, y * inv_s, z * inv_s);
}


void ckVec::operator/=(r32 s)
{
    r32 inv_s = 1.0f / s;

    x *= inv_s;
    y *= inv_s;
    z *= inv_s;
}


r32 ckVec::length() const
{
    return ckMath::sqrt(x * x + y * y + z * z);
}


r32 ckVec::sqLength() const
{
    return x * x + y * y + z * z;
}


r32 ckVec::dist(const ckVec& vec) const
{
    return (*this - vec).length();
}


r32 ckVec::sqDist(const ckVec& vec) const
{
    return (*this - vec).sqLength();
}


r32 ckVec::dot(const ckVec& vec) const
{
    return x * vec.x + y * vec.y + z * vec.z;
}


ckVec ckVec::cross(const ckVec& vec) const
{
    return ckVec(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
}


ckVec ckVec::rotateX_r32(r32 deg) const
{
    r32 sin = ckMath::sin_r32(deg);
    r32 cos = ckMath::cos_r32(deg);

    return ckVec(x, y * cos - z * sin, z * cos + y * sin);
}


ckVec ckVec::rotateY_r32(r32 deg) const
{
    r32 sin = ckMath::sin_r32(deg);
    r32 cos = ckMath::cos_r32(deg);

    return ckVec(x * cos + z * sin, y, z * cos - x * sin);
}


ckVec ckVec::rotateZ_r32(r32 deg) const
{
    r32 sin = ckMath::sin_r32(deg);
    r32 cos = ckMath::cos_r32(deg);

    return ckVec(x * cos - y * sin, y * cos + x * sin, z);
}


ckVec ckVec::rotateX_s32(s32 deg) const
{
    r32 sin = ckMath::sin_s32(deg);
    r32 cos = ckMath::cos_s32(deg);

    return ckVec(x, y * cos - z * sin, z * cos + y * sin);
}


ckVec ckVec::rotateY_s32(s32 deg) const
{
    r32 sin = ckMath::sin_s32(deg);
    r32 cos = ckMath::cos_s32(deg);

    return ckVec(x * cos + z * sin, y, z * cos - x * sin);
}


ckVec ckVec::rotateZ_s32(s32 deg) const
{
    r32 sin = ckMath::sin_s32(deg);
    r32 cos = ckMath::cos_s32(deg);

    return ckVec(x * cos - y * sin, y * cos + x * sin, z);
}


ckVec ckVec::normalize() const
{
    return *this / length();
}


ckVec ckVec::interp(const ckVec& to, r32 ratio) const
{
    if (ratio < ckMath::EPSILON)
    {
        return *this;
    }
    else if (ratio > 1.0f - ckMath::EPSILON)
    {
        return to;
    }
    else
    {
        return *this * (1.0f - ratio) + to * ratio;
    }
}


ckVec ckVec::slerp(const ckVec& to, r32 ratio) const
{
    /*
        the length of each vector must be 1.0
    */

    if (ratio < ckMath::EPSILON)
    {
        return *this;
    }
    else if (ratio > 1.0f - ckMath::EPSILON)
    {
        return to;
    }
    else
    {
        r32 a = ckMath::acos(dot(to));
        r32 b = a * ratio;

        if (ckMath::abs(a) > 180.0f - ckMath::EPSILON)
        {
            return *this;
        }
        else
        {
            return ckMath::cos_r32(b) * *this + (to - ckMath::cos_r32(a) * *this) * (ckMath::sin_r32(b) / ckMath::sin_r32(a));
        }
    }
}


ckVec ckVec::toLocalOf(const ckMat& mat) const
{
    ckVec vec = *this - mat.trans;

    return ckVec( //
        vec.dot(mat.x_axis) / mat.x_axis.sqLength(), //
        vec.dot(mat.y_axis) / mat.y_axis.sqLength(), //
        vec.dot(mat.z_axis) / mat.z_axis.sqLength());
}


ckVec ckVec::toGlobalFrom(const ckMat& mat) const
{
    return mat.x_axis * x + mat.y_axis * y + mat.z_axis * z + mat.trans;
}


ckVec ckVec::toLocalOf_noTrans(const ckMat& mat) const
{
    return ckVec( //
        dot(mat.x_axis) / mat.x_axis.sqLength(), //
        dot(mat.y_axis) / mat.y_axis.sqLength(), //
        dot(mat.z_axis) / mat.z_axis.sqLength());
}


ckVec ckVec::toGlobalFrom_noTrans(const ckMat& mat) const
{
    return mat.x_axis * x + mat.y_axis * y + mat.z_axis * z;
}


void ckVec::toR32x4(r32* r32x4, r32 w) const
{
    r32x4[0] = x;
    r32x4[1] = y;
    r32x4[2] = z;
    r32x4[3] = w;
}


ckVec ckVec::fromR32x4(const r32* r32x4)
{
    return ckVec(r32x4[0], r32x4[1], r32x4[2]);
}


void ckVec::mulR32x4(r32* res_r32x4, const r32* lhs_r32x16, const r32* rhs_r32x4)
{
    r32 v[4];

    v[0] = lhs_r32x16[0] * rhs_r32x4[0] + lhs_r32x16[4] * rhs_r32x4[1] + lhs_r32x16[8] * rhs_r32x4[2] + lhs_r32x16[12] * rhs_r32x4[3];
    v[1] = lhs_r32x16[1] * rhs_r32x4[0] + lhs_r32x16[5] * rhs_r32x4[1] + lhs_r32x16[9] * rhs_r32x4[2] + lhs_r32x16[13] * rhs_r32x4[3];
    v[2] = lhs_r32x16[2] * rhs_r32x4[0] + lhs_r32x16[6] * rhs_r32x4[1] + lhs_r32x16[10] * rhs_r32x4[2] + lhs_r32x16[14] * rhs_r32x4[3];
    v[3] = lhs_r32x16[3] * rhs_r32x4[0] + lhs_r32x16[7] * rhs_r32x4[1] + lhs_r32x16[11] * rhs_r32x4[2] + lhs_r32x16[15] * rhs_r32x4[3];

    ckMemMgr::memcpy(res_r32x4, v, sizeof(v));
}
