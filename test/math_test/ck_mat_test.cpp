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


#include "test.h"


void ckMatTest()
{
    /*
        static const ckMat ZERO
        static const ckMat UNIT
    */
    {
        ckAssert(isEqual(ckMat::ZERO, ckMat(ckVec::ZERO, ckVec::ZERO, ckVec::ZERO, ckVec::ZERO)));
        ckAssert(isEqual(ckMat::UNIT, ckMat(ckVec::X_UNIT, ckVec::Y_UNIT, ckVec::Z_UNIT, ckVec::ZERO)));
    }


    /*
        ckMat()
        ckMat(const ckVec& x_axis_, const ckVec& y_axis_, const ckVec& z_axis_, const ckVec& trans_)
        void set(const ckVec& x_axis_, const ckVec& y_axis_, const ckVec& z_axis_, const ckVec& trans_)
    */
    {
        ckMat mat1;

        mat1.set(ckVec(1.0f, 2.0f, 3.0f), ckVec(4.0f, 5.0f, 6.0f), ckVec(7.0f, 8.0f, 9.0f), ckVec(10.0f, 11.0f, 12.0f));
        ckAssert(isEqual(mat1, ckMat(ckVec(1.0f, 2.0f, 3.0f), ckVec(4.0f, 5.0f, 6.0f), ckVec(7.0f, 8.0f, 9.0f), ckVec(10.0f, 11.0f, 12.0f))));

        ckMat mat2 = mat1;
        ckAssert(isEqual(mat1, mat2));

        ckMat mat3(ckVec(1.0f, 2.0f, 3.0f), ckVec(4.0f, 5.0f, 6.0f), ckVec(7.0f, 8.0f, 9.0f), ckVec(10.0f, 11.0f, 12.0f));
        ckAssert(isEqual(mat3, mat2));
    }

    /*
        bool isUnit() const
    */
    {
        ckAssert(ckMat::UNIT.isUnit());
        ckAssert(!ckMat::ZERO.isUnit());
    }

    /*
        ckMat rotateX_r32(r32 deg) const
        ckMat rotateY_r32(r32 deg) const
        ckMat rotateZ_r32(r32 deg) const
    */
    {
        ckMat mat1 = ckMat(ckVec::X_UNIT, ckVec::Y_UNIT, ckVec::Z_UNIT, ckVec(1.0f, 2.0f, 3.0f));

        ckMat mat2 = mat1.rotateX_r32(90.0f);
        ckAssert(isEqual(mat2, ckMat(ckVec(1.0f, 0.0f, 0.0f), ckVec(0.0f, 0.0f, 1.0f), ckVec(0.0f, -1.0f, 0.0f), ckVec(1.0f, 2.0f, 3.0f))));

        ckMat mat3 = mat2.rotateY_r32(90.0f);
        ckAssert(isEqual(mat3, ckMat(ckVec(0.0f, 1.0f, 0.0f), ckVec(0.0f, 0.0f, 1.0f), ckVec(1.0f, 0.0f, 0.0f), ckVec(1.0f, 2.0f, 3.0f))));

        ckMat mat4 = mat3.rotateZ_r32(90.0f);
        ckAssert(isEqual(mat4, ckMat(ckVec(0.0f, 0.0f, 1.0f), ckVec(0.0f, -1.0f, 0.0f), ckVec(1.0f, 0.0f, 0.0f), ckVec(1.0f, 2.0f, 3.0f))));
    }

    /*
        ckMat rotateX_s32(s32 deg) const
        ckMat rotateY_s32(s32 deg) const
        ckMat rotateZ_s32(s32 deg) const
    */
    {
        ckMat mat1 = ckMat(ckVec::X_UNIT, ckVec::Y_UNIT, ckVec::Z_UNIT, ckVec(1.0f, 2.0f, 3.0f));

        ckMat mat2 = mat1.rotateX_s32(90);
        ckAssert(isEqual(mat2, ckMat(ckVec(1.0f, 0.0f, 0.0f), ckVec(0.0f, 0.0f, 1.0f), ckVec(0.0f, -1.0f, 0.0f), ckVec(1.0f, 2.0f, 3.0f))));

        ckMat mat3 = mat2.rotateY_s32(90);
        ckAssert(isEqual(mat3, ckMat(ckVec(0.0f, 1.0f, 0.0f), ckVec(0.0f, 0.0f, 1.0f), ckVec(1.0f, 0.0f, 0.0f), ckVec(1.0f, 2.0f, 3.0f))));

        ckMat mat4 = mat3.rotateZ_s32(90);
        ckAssert(isEqual(mat4, ckMat(ckVec(0.0f, 0.0f, 1.0f), ckVec(0.0f, -1.0f, 0.0f), ckVec(1.0f, 0.0f, 0.0f), ckVec(1.0f, 2.0f, 3.0f))));
    }

    /*
        ckMat scale(r32 x_Scale, r32 y_Scale, r32 z_Scale = 1.0f) const
        ckMat translate(r32 x, r32 y, r32 z = 0.0f) const
    */
    {
        ckMat mat1(ckMat(ckVec(1.0f, 2.0f, 3.0f), ckVec(4.0f, 5.0f, 6.0f), ckVec(7.0f, 8.0f, 9.0f), ckVec(10.0f, 11.0f, 12.0f)));

        ckMat mat2 = mat1.scale(2.0f, 3.0f, -1.0f);
        ckAssert(isEqual(mat2, ckMat(ckVec(2.0f, 4.0f, 6.0f), ckVec(12.0f, 15.0f, 18.0f), ckVec(-7.0f, -8.0f, -9.0f), ckVec(10.0f, 11.0f, 12.0f))));

        ckMat mat3 = mat2.translate(3.0f, -2.0f, 1.0f);
        ckAssert(isEqual(mat3, ckMat(mat2.x_axis, mat2.y_axis, mat2.z_axis, ckVec(-15.0f, -15.0f, -15.0f))));
    }

    /*
        ckMat slerp(const ckMat& to, r32 ratio) const
        ckMat slerp_noTrans(const ckMat& to, r32 ratio) const
    */
    {
        for (s32 i = -360; i <= 360; i += 10)
        {
            r32 deg = static_cast<r32>(i) / 2.1f;
            r32 ratio = static_cast<r32>((i / 10 + 36) % 11) / 10.0f;

            ckMat mat1 = ckMat::UNIT.rotateX_s32(i).rotateY_s32(i * 2).rotateZ_s32(i * 3).translate(0.1f * i, 0.2f * i, 0.3f * i);
            ckMat mat2 = mat1.rotateX_r32(deg).translate(-0.3f * i, -0.2f * i, -0.1f * i);

            ckMat mat3 = mat1.rotateX_r32(deg * ratio);
            mat3.trans = mat1.trans.interp(mat2.trans, ratio);
            ckAssert(isEqual(mat1.slerp(mat2, ratio), mat3));

            ckMat mat4 = mat3;
            mat4.trans = ckVec::ZERO;
            ckAssert(isEqual(mat1.slerp_noTrans(mat2, ratio), mat4));
        }
    }

    /*
        ckMat orthonormal() const
    */
    {
        ckMat mat1 = ckMat::UNIT.rotateX_s32(30).rotateY_s32(30).rotateZ_s32(30);
        mat1.trans.set(1.0f, 2.0f, 3.0f);

        ckMat mat2 = mat1.scale(10.0f, 11.0f, 12.0f);

        ckMat mat3 = mat2.orthonormal();
        ckAssert(isEqual(mat3, mat1));
        ckAssert(isEqual(mat3.y_axis.cross(mat3.z_axis), mat3.x_axis));
        ckAssert(isEqual(mat3.z_axis.cross(mat3.x_axis), mat3.y_axis));
        ckAssert(isEqual(mat3.x_axis.cross(mat3.y_axis), mat3.z_axis));
    }

    /*
        ckMat toLocalOf(const ckMat& mat) const
        ckMat toGlobalFrom(const ckMat& mat) const
    */
    {
        ckMat mat1 = ckMat(ckVec(0.0f, 0.0f, 0.5f), ckVec(2.0f, 0.0f, 0.0f), ckVec(0.0f, -0.5f, 0.0f), ckVec(1.0f, 2.0f, 3.0f));
        ckMat mat2 = ckMat(ckVec::X_UNIT, ckVec::Y_UNIT, ckVec::Z_UNIT, ckVec(3.0f, 4.0f, 5.0f));

        ckMat mat3 = mat2.toLocalOf(mat1);
        ckAssert(isEqual(mat3, ckMat(ckVec(0.0f, 0.5f, 0.0f), ckVec(0.0f, 0.0f, -2.0f), ckVec(2.0f, 0.0f, 0.0f), ckVec(4.0f, 1.0f, -4.0f))));

        ckMat mat4 = mat3.toGlobalFrom(mat1);
        ckAssert(isEqual(mat4, mat2));
    }

    /*
        ckMat toLocalOf_noTrans(const ckMat& mat) const
        ckMat toGlobalFrom_noTrans(const ckMat& mat) const
    */
    {
        ckMat mat1 = ckMat(ckVec(0.0f, 0.0f, 0.5f), ckVec(2.0f, 0.0f, 0.0f), ckVec(0.0f, -0.5f, 0.0f), ckVec(1.0f, 2.0f, 3.0f));
        ckMat mat2 = ckMat(ckVec::X_UNIT, ckVec::Y_UNIT, ckVec::Z_UNIT, ckVec(3.0f, 4.0f, 5.0f));

        ckMat mat3 = mat2.toLocalOf_noTrans(mat1);
        ckAssert(isEqual(mat3, ckMat(ckVec(0.0f, 0.5f, 0.0f), ckVec(0.0f, 0.0f, -2.0f), ckVec(2.0f, 0.0f, 0.0f), ckVec(0.0f, 0.0f, 0.0f))));

        ckMat mat4 = mat3.toGlobalFrom_noTrans(mat1);
        ckAssert(isEqual(mat4, ckMat::UNIT));
    }

    /*
        static ckMat lookAt(const ckVec& from, const ckVec& to, const ckVec& up)
    */
    {
        ckMat mat = ckMat::lookAt(ckVec(5.0f, 2.0f, -3.0f), ckVec(-5.0f, 2.0f, -3.0f), ckVec(1.0f, 0.0f, 10.0f));
        ckAssert(isEqual(mat, ckMat(ckVec(0.0f, 1.0f, 0.0f), ckVec(0.0f, 0.0f, 1.0f), ckVec(1.0f, 0.0f, 0.0f), ckVec(5.0f, 2.0f, -3.0f))));
    }

    /*
        void toR32x16(r32* r32x16) const
        static ckMat fromR32x16(const r32* r32x16)
    */
    {
        ckMat mat1(ckVec(1.0f, 2.0f, 3.0f), ckVec(4.0f, 5.0f, 6.0f), ckVec(7.0f, 8.0f, 9.0f), ckVec(10.0f, 11.0f, 12.0f));

        r32 m[16];
        mat1.toR32x16(m);
        ckAssert( //
            isEqual(m[0], 1.0f) && isEqual(m[1], 2.0f) && isEqual(m[2], 3.0f) && isEqual(m[3], 0.0f) && //
            isEqual(m[4], 4.0f) && isEqual(m[5], 5.0f) && isEqual(m[6], 6.0f) && isEqual(m[7], 0.0f) && //
            isEqual(m[8], 7.0f) && isEqual(m[9], 8.0f) && isEqual(m[10], 9.0f) && isEqual(m[11], 0.0f) && //
            isEqual(m[12], 10.0f) && isEqual(m[13], 11.0f) && isEqual(m[14], 12.0f) && isEqual(m[15], 1.0f));

        ckMat mat2 = ckMat::fromR32x16(m);
        ckAssert(isEqual(mat2, mat1));
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

        ckMat::mulR32x16(m3, m1, m2);
        ckAssert( //
            isEqual(m3[0], 118.0f) && isEqual(m3[1], 132.0f) && isEqual(m3[2], 146.0f) && isEqual(m3[3], 160.0f) && //
            isEqual(m3[4], 230.0f) && isEqual(m3[5], 260.0f) && isEqual(m3[6], 290.0f) && isEqual(m3[7], 320.0f) && //
            isEqual(m3[8], 342.0f) && isEqual(m3[9], 388.0f) && isEqual(m3[10], 434.0f) && isEqual(m3[11], 480.0f) && //
            isEqual(m3[12], 246.0f) && isEqual(m3[13], 292.0f) && isEqual(m3[14], 338.0f) && isEqual(m3[15], 384.0f));

        ckMat::mulR32x16(m1, m1, m1);
        ckAssert( //
            isEqual(m1[0], 90.0f) && isEqual(m1[1], 100.0f) && isEqual(m1[2], 110.0f) && isEqual(m1[3], 120.0f) && //
            isEqual(m1[4], 202.0f) && isEqual(m1[5], 228.0f) && isEqual(m1[6], 254.0f) && isEqual(m1[7], 280.0f) && //
            isEqual(m1[8], 314.0f) && isEqual(m1[9], 356.0f) && isEqual(m1[10], 398.0f) && isEqual(m1[11], 440.0f) && //
            isEqual(m1[12], 426.0f) && isEqual(m1[13], 484.0f) && isEqual(m1[14], 542.0f) && isEqual(m1[15], 600.0f));
    }
}
