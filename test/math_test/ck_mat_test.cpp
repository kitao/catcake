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


#include "test.h"


void pgMatTest()
{
    /*
        static const pgMat ZERO
        static const pgMat UNIT
    */
    {
        pgAssert(isEqual(pgMat::ZERO, pgMat(pgVec::ZERO, pgVec::ZERO, pgVec::ZERO, pgVec::ZERO)));
        pgAssert(isEqual(pgMat::UNIT, pgMat(pgVec::X_UNIT, pgVec::Y_UNIT, pgVec::Z_UNIT, pgVec::ZERO)));
    }


    /*
        pgMat()
        pgMat(const pgVec& x_axis_, const pgVec& y_axis_, const pgVec& z_axis_, const pgVec& trans_)
        void set(const pgVec& x_axis_, const pgVec& y_axis_, const pgVec& z_axis_, const pgVec& trans_)
    */
    {
        pgMat mat1;

        mat1.set(pgVec(1.0f, 2.0f, 3.0f), pgVec(4.0f, 5.0f, 6.0f), pgVec(7.0f, 8.0f, 9.0f), pgVec(10.0f, 11.0f, 12.0f));
        pgAssert(isEqual(mat1, pgMat(pgVec(1.0f, 2.0f, 3.0f), pgVec(4.0f, 5.0f, 6.0f), pgVec(7.0f, 8.0f, 9.0f), pgVec(10.0f, 11.0f, 12.0f))));

        pgMat mat2 = mat1;
        pgAssert(isEqual(mat1, mat2));

        pgMat mat3(pgVec(1.0f, 2.0f, 3.0f), pgVec(4.0f, 5.0f, 6.0f), pgVec(7.0f, 8.0f, 9.0f), pgVec(10.0f, 11.0f, 12.0f));
        pgAssert(isEqual(mat3, mat2));
    }

    /*
        bool isUnit() const
    */
    {
        pgAssert(pgMat::UNIT.isUnit());
        pgAssert(!pgMat::ZERO.isUnit());
    }

    /*
        pgMat rotateX_r32(r32 deg) const
        pgMat rotateY_r32(r32 deg) const
        pgMat rotateZ_r32(r32 deg) const
    */
    {
        pgMat mat1 = pgMat(pgVec::X_UNIT, pgVec::Y_UNIT, pgVec::Z_UNIT, pgVec(1.0f, 2.0f, 3.0f));

        pgMat mat2 = mat1.rotateX_r32(90.0f);
        pgAssert(isEqual(mat2, pgMat(pgVec(1.0f, 0.0f, 0.0f), pgVec(0.0f, 0.0f, 1.0f), pgVec(0.0f, -1.0f, 0.0f), pgVec(1.0f, 2.0f, 3.0f))));

        pgMat mat3 = mat2.rotateY_r32(90.0f);
        pgAssert(isEqual(mat3, pgMat(pgVec(0.0f, 1.0f, 0.0f), pgVec(0.0f, 0.0f, 1.0f), pgVec(1.0f, 0.0f, 0.0f), pgVec(1.0f, 2.0f, 3.0f))));

        pgMat mat4 = mat3.rotateZ_r32(90.0f);
        pgAssert(isEqual(mat4, pgMat(pgVec(0.0f, 0.0f, 1.0f), pgVec(0.0f, -1.0f, 0.0f), pgVec(1.0f, 0.0f, 0.0f), pgVec(1.0f, 2.0f, 3.0f))));
    }

    /*
        pgMat rotateX_s32(s32 deg) const
        pgMat rotateY_s32(s32 deg) const
        pgMat rotateZ_s32(s32 deg) const
    */
    {
        pgMat mat1 = pgMat(pgVec::X_UNIT, pgVec::Y_UNIT, pgVec::Z_UNIT, pgVec(1.0f, 2.0f, 3.0f));

        pgMat mat2 = mat1.rotateX_s32(90);
        pgAssert(isEqual(mat2, pgMat(pgVec(1.0f, 0.0f, 0.0f), pgVec(0.0f, 0.0f, 1.0f), pgVec(0.0f, -1.0f, 0.0f), pgVec(1.0f, 2.0f, 3.0f))));

        pgMat mat3 = mat2.rotateY_s32(90);
        pgAssert(isEqual(mat3, pgMat(pgVec(0.0f, 1.0f, 0.0f), pgVec(0.0f, 0.0f, 1.0f), pgVec(1.0f, 0.0f, 0.0f), pgVec(1.0f, 2.0f, 3.0f))));

        pgMat mat4 = mat3.rotateZ_s32(90);
        pgAssert(isEqual(mat4, pgMat(pgVec(0.0f, 0.0f, 1.0f), pgVec(0.0f, -1.0f, 0.0f), pgVec(1.0f, 0.0f, 0.0f), pgVec(1.0f, 2.0f, 3.0f))));
    }

    /*
        pgMat scale(r32 x_Scale, r32 y_Scale, r32 z_Scale = 1.0f) const
        pgMat translate(r32 x, r32 y, r32 z = 0.0f) const
    */
    {
        pgMat mat1(pgMat(pgVec(1.0f, 2.0f, 3.0f), pgVec(4.0f, 5.0f, 6.0f), pgVec(7.0f, 8.0f, 9.0f), pgVec(10.0f, 11.0f, 12.0f)));

        pgMat mat2 = mat1.scale(2.0f, 3.0f, -1.0f);
        pgAssert(isEqual(mat2, pgMat(pgVec(2.0f, 4.0f, 6.0f), pgVec(12.0f, 15.0f, 18.0f), pgVec(-7.0f, -8.0f, -9.0f), pgVec(10.0f, 11.0f, 12.0f))));

        pgMat mat3 = mat2.translate(3.0f, -2.0f, 1.0f);
        pgAssert(isEqual(mat3, pgMat(mat2.x_axis, mat2.y_axis, mat2.z_axis, pgVec(-15.0f, -15.0f, -15.0f))));
    }

    /*
        pgMat slerp(const pgMat& to, r32 ratio) const
        pgMat slerp_noTrans(const pgMat& to, r32 ratio) const
    */
    {
        for (s32 i = -360; i <= 360; i += 10)
        {
            r32 deg = static_cast<r32>(i) / 2.1f;
            r32 ratio = static_cast<r32>((i / 10 + 36) % 11) / 10.0f;

            pgMat mat1 = pgMat::UNIT.rotateX_s32(i).rotateY_s32(i * 2).rotateZ_s32(i * 3).translate(0.1f * i, 0.2f * i, 0.3f * i);
            pgMat mat2 = mat1.rotateX_r32(deg).translate(-0.3f * i, -0.2f * i, -0.1f * i);

            pgMat mat3 = mat1.rotateX_r32(deg * ratio);
            mat3.trans = mat1.trans.interp(mat2.trans, ratio);
            pgAssert(isEqual(mat1.slerp(mat2, ratio), mat3));

            pgMat mat4 = mat3;
            mat4.trans = pgVec::ZERO;
            pgAssert(isEqual(mat1.slerp_noTrans(mat2, ratio), mat4));
        }
    }

    /*
        pgMat orthonormal() const
    */
    {
        pgMat mat1 = pgMat::UNIT.rotateX_s32(30).rotateY_s32(30).rotateZ_s32(30);
        mat1.trans.set(1.0f, 2.0f, 3.0f);

        pgMat mat2 = mat1.scale(10.0f, 11.0f, 12.0f);

        pgMat mat3 = mat2.orthonormal();
        pgAssert(isEqual(mat3, mat1));
        pgAssert(isEqual(mat3.y_axis.cross(mat3.z_axis), mat3.x_axis));
        pgAssert(isEqual(mat3.z_axis.cross(mat3.x_axis), mat3.y_axis));
        pgAssert(isEqual(mat3.x_axis.cross(mat3.y_axis), mat3.z_axis));
    }

    /*
        pgMat toLocalOf(const pgMat& mat) const
        pgMat toGlobalFrom(const pgMat& mat) const
    */
    {
        pgMat mat1 = pgMat(pgVec(0.0f, 0.0f, 0.5f), pgVec(2.0f, 0.0f, 0.0f), pgVec(0.0f, -0.5f, 0.0f), pgVec(1.0f, 2.0f, 3.0f));
        pgMat mat2 = pgMat(pgVec::X_UNIT, pgVec::Y_UNIT, pgVec::Z_UNIT, pgVec(3.0f, 4.0f, 5.0f));

        pgMat mat3 = mat2.toLocalOf(mat1);
        pgAssert(isEqual(mat3, pgMat(pgVec(0.0f, 0.5f, 0.0f), pgVec(0.0f, 0.0f, -2.0f), pgVec(2.0f, 0.0f, 0.0f), pgVec(4.0f, 1.0f, -4.0f))));

        pgMat mat4 = mat3.toGlobalFrom(mat1);
        pgAssert(isEqual(mat4, mat2));
    }

    /*
        pgMat toLocalOf_noTrans(const pgMat& mat) const
        pgMat toGlobalFrom_noTrans(const pgMat& mat) const
    */
    {
        pgMat mat1 = pgMat(pgVec(0.0f, 0.0f, 0.5f), pgVec(2.0f, 0.0f, 0.0f), pgVec(0.0f, -0.5f, 0.0f), pgVec(1.0f, 2.0f, 3.0f));
        pgMat mat2 = pgMat(pgVec::X_UNIT, pgVec::Y_UNIT, pgVec::Z_UNIT, pgVec(3.0f, 4.0f, 5.0f));

        pgMat mat3 = mat2.toLocalOf_noTrans(mat1);
        pgAssert(isEqual(mat3, pgMat(pgVec(0.0f, 0.5f, 0.0f), pgVec(0.0f, 0.0f, -2.0f), pgVec(2.0f, 0.0f, 0.0f), pgVec(0.0f, 0.0f, 0.0f))));

        pgMat mat4 = mat3.toGlobalFrom_noTrans(mat1);
        pgAssert(isEqual(mat4, pgMat::UNIT));
    }

    /*
        static pgMat lookAt(const pgVec& from, const pgVec& to, const pgVec& up)
    */
    {
        pgMat mat = pgMat::lookAt(pgVec(5.0f, 2.0f, -3.0f), pgVec(-5.0f, 2.0f, -3.0f), pgVec(1.0f, 0.0f, 10.0f));
        pgAssert(isEqual(mat, pgMat(pgVec(0.0f, 1.0f, 0.0f), pgVec(0.0f, 0.0f, 1.0f), pgVec(1.0f, 0.0f, 0.0f), pgVec(5.0f, 2.0f, -3.0f))));
    }

    /*
        void toR32x16(r32* r32x16) const
        static pgMat fromR32x16(const r32* r32x16)
    */
    {
        pgMat mat1(pgVec(1.0f, 2.0f, 3.0f), pgVec(4.0f, 5.0f, 6.0f), pgVec(7.0f, 8.0f, 9.0f), pgVec(10.0f, 11.0f, 12.0f));

        r32 m[16];
        mat1.toR32x16(m);
        pgAssert( //
            isEqual(m[0], 1.0f) && isEqual(m[1], 2.0f) && isEqual(m[2], 3.0f) && isEqual(m[3], 0.0f) && //
            isEqual(m[4], 4.0f) && isEqual(m[5], 5.0f) && isEqual(m[6], 6.0f) && isEqual(m[7], 0.0f) && //
            isEqual(m[8], 7.0f) && isEqual(m[9], 8.0f) && isEqual(m[10], 9.0f) && isEqual(m[11], 0.0f) && //
            isEqual(m[12], 10.0f) && isEqual(m[13], 11.0f) && isEqual(m[14], 12.0f) && isEqual(m[15], 1.0f));

        pgMat mat2 = pgMat::fromR32x16(m);
        pgAssert(isEqual(mat2, mat1));
    }

    /*
        static void mulR32x16(r32* res_r32x16, const r32* lhs_r32x16, const r32* rhs_r32x16)
    */
    {
        r32 m1[16] =
        {
            1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f
        };

        r32 m2[16] =
        {
            2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 1.0f
        };

        r32 m3[16];

        pgMat::mulR32x16(m3, m1, m2);
        pgAssert( //
            isEqual(m3[0], 118.0f) && isEqual(m3[1], 132.0f) && isEqual(m3[2], 146.0f) && isEqual(m3[3], 160.0f) && //
            isEqual(m3[4], 230.0f) && isEqual(m3[5], 260.0f) && isEqual(m3[6], 290.0f) && isEqual(m3[7], 320.0f) && //
            isEqual(m3[8], 342.0f) && isEqual(m3[9], 388.0f) && isEqual(m3[10], 434.0f) && isEqual(m3[11], 480.0f) && //
            isEqual(m3[12], 246.0f) && isEqual(m3[13], 292.0f) && isEqual(m3[14], 338.0f) && isEqual(m3[15], 384.0f));

        pgMat::mulR32x16(m1, m1, m1);
        pgAssert( //
            isEqual(m1[0], 90.0f) && isEqual(m1[1], 100.0f) && isEqual(m1[2], 110.0f) && isEqual(m1[3], 120.0f) && //
            isEqual(m1[4], 202.0f) && isEqual(m1[5], 228.0f) && isEqual(m1[6], 254.0f) && isEqual(m1[7], 280.0f) && //
            isEqual(m1[8], 314.0f) && isEqual(m1[9], 356.0f) && isEqual(m1[10], 398.0f) && isEqual(m1[11], 440.0f) && //
            isEqual(m1[12], 426.0f) && isEqual(m1[13], 484.0f) && isEqual(m1[14], 542.0f) && isEqual(m1[15], 600.0f));
    }
}
