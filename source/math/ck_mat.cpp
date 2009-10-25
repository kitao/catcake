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


const pgMat pgMat::ZERO(pgVec(0.0f, 0.0f, 0.0f), pgVec(0.0f, 0.0f, 0.0f), pgVec(0.0f, 0.0f, 0.0f), pgVec(0.0f, 0.0f, 0.0f));
const pgMat pgMat::UNIT(pgVec(1.0f, 0.0f, 0.0f), pgVec(0.0f, 1.0f, 0.0f), pgVec(0.0f, 0.0f, 1.0f), pgVec(0.0f, 0.0f, 0.0f));


pgMat::pgMat() {}


pgMat::pgMat(const pgVec& x_axis_, const pgVec& y_axis_, const pgVec& z_axis_, const pgVec& trans_)
{
    x_axis = x_axis_;
    y_axis = y_axis_;
    z_axis = z_axis_;
    trans = trans_;
}


void pgMat::set(const pgVec& x_axis_, const pgVec& y_axis_, const pgVec& z_axis_, const pgVec& trans_)
{
    x_axis = x_axis_;
    y_axis = y_axis_;
    z_axis = z_axis_;
    trans = trans_;
}


bool pgMat::isUnit() const
{
    return ( //
        x_axis.x == 1.0f && x_axis.y == 0.0f && x_axis.z == 0.0f && //
        y_axis.x == 0.0f && y_axis.y == 1.0f && y_axis.z == 0.0f && //
        z_axis.x == 0.0f && z_axis.y == 0.0f && z_axis.z == 1.0f && //
        trans.x == 0.0f && trans.y == 0.0f && trans.z == 0.0f);
}


pgMat pgMat::rotateX_r32(r32 deg) const
{
    r32 sin = pgMath::sin_r32(deg);
    r32 cos = pgMath::cos_r32(deg);

    return pgMat(pgVec::X_UNIT, pgVec(0.0f, cos, sin), pgVec(0.0f, -sin, cos), pgVec::ZERO).toGlobalFrom(*this);
}


pgMat pgMat::rotateY_r32(r32 deg) const
{
    r32 sin = pgMath::sin_r32(deg);
    r32 cos = pgMath::cos_r32(deg);

    return pgMat(pgVec(cos, 0.0f, -sin), pgVec::Y_UNIT, pgVec(sin, 0.0f, cos), pgVec::ZERO).toGlobalFrom(*this);
}


pgMat pgMat::rotateZ_r32(r32 deg) const
{
    r32 sin = pgMath::sin_r32(deg);
    r32 cos = pgMath::cos_r32(deg);

    return pgMat(pgVec(cos, sin, 0.0f), pgVec(-sin, cos, 0.0f), pgVec::Z_UNIT, pgVec::ZERO).toGlobalFrom(*this);
}


pgMat pgMat::rotateX_s32(s32 deg) const
{
    r32 sin = pgMath::sin_s32(deg);
    r32 cos = pgMath::cos_s32(deg);

    return pgMat(pgVec::X_UNIT, pgVec(0.0f, cos, sin), pgVec(0.0f, -sin, cos), pgVec::ZERO).toGlobalFrom(*this);
}


pgMat pgMat::rotateY_s32(s32 deg) const
{
    r32 sin = pgMath::sin_s32(deg);
    r32 cos = pgMath::cos_s32(deg);

    return pgMat(pgVec(cos, 0.0f, -sin), pgVec::Y_UNIT, pgVec(sin, 0.0f, cos), pgVec::ZERO).toGlobalFrom(*this);
}


pgMat pgMat::rotateZ_s32(s32 deg) const
{
    r32 sin = pgMath::sin_s32(deg);
    r32 cos = pgMath::cos_s32(deg);

    return pgMat(pgVec(cos, sin, 0.0f), pgVec(-sin, cos, 0.0f), pgVec::Z_UNIT, pgVec::ZERO).toGlobalFrom(*this);
}


pgMat pgMat::scale(r32 x_scale, r32 y_scale, r32 z_scale) const
{
    return pgMat(x_axis * x_scale, y_axis * y_scale, z_axis * z_scale, trans);
}


pgMat pgMat::translate(r32 x, r32 y, r32 z) const
{
    return pgMat(x_axis, y_axis, z_axis, x_axis * x + y_axis * y + z_axis * z + trans);
}


pgMat pgMat::slerp(const pgMat& to, r32 ratio) const
{
    return pgQuat::fromMat(*this).slerp(pgQuat::fromMat(to), ratio).toMat(trans.interp(to.trans, ratio));
}


pgMat pgMat::slerp_noTrans(const pgMat& to, r32 ratio) const
{
    return pgQuat::fromMat(*this).slerp(pgQuat::fromMat(to), ratio).toMat(pgVec::ZERO);
}


pgMat pgMat::orthonormal() const
{
    pgVec new_z_axis = z_axis.normalize();
    pgVec new_x_axis = y_axis.cross(z_axis).normalize();
    pgVec new_y_axis = new_z_axis.cross(new_x_axis);

    return pgMat(new_x_axis, new_y_axis, new_z_axis, trans);
}


pgMat pgMat::toLocalOf(const pgMat& mat) const
{
    r32 rsq_xa = 1.0f / mat.x_axis.sqLength();
    r32 rsq_ya = 1.0f / mat.y_axis.sqLength();
    r32 rsq_za = 1.0f / mat.z_axis.sqLength();
    pgVec vec = trans - mat.trans;

    return pgMat( //
        pgVec(x_axis.dot(mat.x_axis) * rsq_xa, x_axis.dot(mat.y_axis) * rsq_ya, x_axis.dot(mat.z_axis) * rsq_za), //
        pgVec(y_axis.dot(mat.x_axis) * rsq_xa, y_axis.dot(mat.y_axis) * rsq_ya, y_axis.dot(mat.z_axis) * rsq_za), //
        pgVec(z_axis.dot(mat.x_axis) * rsq_xa, z_axis.dot(mat.y_axis) * rsq_ya, z_axis.dot(mat.z_axis) * rsq_za), //
        pgVec(vec.dot(mat.x_axis) * rsq_xa, vec.dot(mat.y_axis) * rsq_ya, vec.dot(mat.z_axis) * rsq_za));
}


pgMat pgMat::toGlobalFrom(const pgMat& mat) const
{
    return pgMat( //
        x_axis.toGlobalFrom_noTrans(mat), //
        y_axis.toGlobalFrom_noTrans(mat), //
        z_axis.toGlobalFrom_noTrans(mat), //
        trans.toGlobalFrom(mat));
}


pgMat pgMat::toLocalOf_noTrans(const pgMat& mat) const
{
    r32 rsq_xa = 1.0f / mat.x_axis.sqLength();
    r32 rsq_ya = 1.0f / mat.y_axis.sqLength();
    r32 rsq_za = 1.0f / mat.z_axis.sqLength();

    return pgMat( //
        pgVec(x_axis.dot(mat.x_axis) * rsq_xa, x_axis.dot(mat.y_axis) * rsq_ya, x_axis.dot(mat.z_axis) * rsq_za), //
        pgVec(y_axis.dot(mat.x_axis) * rsq_xa, y_axis.dot(mat.y_axis) * rsq_ya, y_axis.dot(mat.z_axis) * rsq_za), //
        pgVec(z_axis.dot(mat.x_axis) * rsq_xa, z_axis.dot(mat.y_axis) * rsq_ya, z_axis.dot(mat.z_axis) * rsq_za), //
        pgVec::ZERO);
}


pgMat pgMat::toGlobalFrom_noTrans(const pgMat& mat) const
{
    return pgMat( //
        x_axis.toGlobalFrom_noTrans(mat), //
        y_axis.toGlobalFrom_noTrans(mat), //
        z_axis.toGlobalFrom_noTrans(mat), //
        pgVec::ZERO);
}


pgMat pgMat::lookAt(const pgVec& from, const pgVec& to, const pgVec& up)
{
    pgVec new_z_axis = (from - to).normalize();
    pgVec new_x_axis = up.cross(new_z_axis).normalize();
    pgVec new_y_axis = new_z_axis.cross(new_x_axis);

    return pgMat(new_x_axis, new_y_axis, new_z_axis, from);
}


void pgMat::toR32x16(r32* r32x16) const
{
    r32x16[0] = x_axis.x;
    r32x16[1] = x_axis.y;
    r32x16[2] = x_axis.z;
    r32x16[3] = 0.0f;

    r32x16[4] = y_axis.x;
    r32x16[5] = y_axis.y;
    r32x16[6] = y_axis.z;
    r32x16[7] = 0.0f;

    r32x16[8] = z_axis.x;
    r32x16[9] = z_axis.y;
    r32x16[10] = z_axis.z;
    r32x16[11] = 0.0f;

    r32x16[12] = trans.x;
    r32x16[13] = trans.y;
    r32x16[14] = trans.z;
    r32x16[15] = 1.0f;
}


pgMat pgMat::fromR32x16(const r32* r32x16)
{
    return pgMat( //
        pgVec(r32x16[0], r32x16[1], r32x16[2]), //
        pgVec(r32x16[4], r32x16[5], r32x16[6]), //
        pgVec(r32x16[8], r32x16[9], r32x16[10]), //
        pgVec(r32x16[12], r32x16[13], r32x16[14]));
}


void pgMat::mulR32x16(r32* res_r32x16, const r32* lhs_r32x16, const r32* rhs_r32x16)
{
    r32 m[16];

    m[0] = lhs_r32x16[0] * rhs_r32x16[0] + lhs_r32x16[4] * rhs_r32x16[1] + lhs_r32x16[8] * rhs_r32x16[2] + lhs_r32x16[12] * rhs_r32x16[3];
    m[1] = lhs_r32x16[1] * rhs_r32x16[0] + lhs_r32x16[5] * rhs_r32x16[1] + lhs_r32x16[9] * rhs_r32x16[2] + lhs_r32x16[13] * rhs_r32x16[3];
    m[2] = lhs_r32x16[2] * rhs_r32x16[0] + lhs_r32x16[6] * rhs_r32x16[1] + lhs_r32x16[10] * rhs_r32x16[2] + lhs_r32x16[14] * rhs_r32x16[3];
    m[3] = lhs_r32x16[3] * rhs_r32x16[0] + lhs_r32x16[7] * rhs_r32x16[1] + lhs_r32x16[11] * rhs_r32x16[2] + lhs_r32x16[15] * rhs_r32x16[3];

    m[4] = lhs_r32x16[0] * rhs_r32x16[4] + lhs_r32x16[4] * rhs_r32x16[5] + lhs_r32x16[8] * rhs_r32x16[6] + lhs_r32x16[12] * rhs_r32x16[7];
    m[5] = lhs_r32x16[1] * rhs_r32x16[4] + lhs_r32x16[5] * rhs_r32x16[5] + lhs_r32x16[9] * rhs_r32x16[6] + lhs_r32x16[13] * rhs_r32x16[7];
    m[6] = lhs_r32x16[2] * rhs_r32x16[4] + lhs_r32x16[6] * rhs_r32x16[5] + lhs_r32x16[10] * rhs_r32x16[6] + lhs_r32x16[14] * rhs_r32x16[7];
    m[7] = lhs_r32x16[3] * rhs_r32x16[4] + lhs_r32x16[7] * rhs_r32x16[5] + lhs_r32x16[11] * rhs_r32x16[6] + lhs_r32x16[15] * rhs_r32x16[7];

    m[8] = lhs_r32x16[0] * rhs_r32x16[8] + lhs_r32x16[4] * rhs_r32x16[9] + lhs_r32x16[8] * rhs_r32x16[10] + lhs_r32x16[12] * rhs_r32x16[11];
    m[9] = lhs_r32x16[1] * rhs_r32x16[8] + lhs_r32x16[5] * rhs_r32x16[9] + lhs_r32x16[9] * rhs_r32x16[10] + lhs_r32x16[13] * rhs_r32x16[11];
    m[10] = lhs_r32x16[2] * rhs_r32x16[8] + lhs_r32x16[6] * rhs_r32x16[9] + lhs_r32x16[10] * rhs_r32x16[10] + lhs_r32x16[14] * rhs_r32x16[11];
    m[11] = lhs_r32x16[3] * rhs_r32x16[8] + lhs_r32x16[7] * rhs_r32x16[9] + lhs_r32x16[11] * rhs_r32x16[10] + lhs_r32x16[15] * rhs_r32x16[11];

    m[12] = lhs_r32x16[0] * rhs_r32x16[12] + lhs_r32x16[4] * rhs_r32x16[13] + lhs_r32x16[8] * rhs_r32x16[14] + lhs_r32x16[12] * rhs_r32x16[15];
    m[13] = lhs_r32x16[1] * rhs_r32x16[12] + lhs_r32x16[5] * rhs_r32x16[13] + lhs_r32x16[9] * rhs_r32x16[14] + lhs_r32x16[13] * rhs_r32x16[15];
    m[14] = lhs_r32x16[2] * rhs_r32x16[12] + lhs_r32x16[6] * rhs_r32x16[13] + lhs_r32x16[10] * rhs_r32x16[14] + lhs_r32x16[14] * rhs_r32x16[15];
    m[15] = lhs_r32x16[3] * rhs_r32x16[12] + lhs_r32x16[7] * rhs_r32x16[13] + lhs_r32x16[11] * rhs_r32x16[14] + lhs_r32x16[15] * rhs_r32x16[15];

    pgMemMgr::memcpy(res_r32x16, m, sizeof(m));
}
