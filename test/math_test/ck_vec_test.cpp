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


void ckVecTest()
{
    /*
        static const ckVec ZERO
        static const ckVec X_UNIT
        static const ckVec Y_UNIT
        static const ckVec Z_UNIT
    */
    {
        ckAssert(isEqual(ckVec::ZERO, ckVec(0.0f, 0.0f, 0.0f)));
        ckAssert(isEqual(ckVec::X_UNIT, ckVec(1.0f, 0.0f, 0.0f)));
        ckAssert(isEqual(ckVec::Y_UNIT, ckVec(0.0f, 1.0f, 0.0f)));
        ckAssert(isEqual(ckVec::Z_UNIT, ckVec(0.0f, 0.0f, 1.0f)));
    }

    /*
        ckVec()
        ckVec(r32 x_, r32 y_, r32 z_ = 0.0f)
        void set(r32 x_, r32 y_, r32 z_ = 0.0f)
    */
    {
        ckVec vec1;

        vec1.set(1.0f, 2.0f, 3.0f);
        ckAssert(isEqual(vec1.x, 1.0f) && isEqual(vec1.y, 2.0f) && isEqual(vec1.z, 3.0f));

        ckVec vec2 = vec1;
        ckAssert(isEqual(vec2.x, 1.0f) && isEqual(vec2.y, 2.0f) && isEqual(vec2.z, 3.0f));

        ckVec vec3(1.0f, 2.0f, 3.0f);
        ckAssert(isEqual(vec3, vec1));
    }

    /*
        ckVec operator+(const ckVec& vec) const
        void operator+=(const ckVec& vec)
    */
    {
        ckVec vec1(1.0f, 2.0f, 3.0f);
        ckVec vec2(3.0f, 4.0f, 5.0f);

        ckVec vec3 = vec1 + vec2;
        ckAssert(isEqual(vec3, ckVec(4.0f, 6.0f, 8.0f)));

        vec1 += vec2;
        ckAssert(isEqual(vec1, ckVec(4.0f, 6.0f, 8.0f)));
    }

    /*
        ckVec operator-() const
        ckVec operator-(const ckVec& vec) const
        void operator-=(const ckVec& vec)
    */
    {
        ckVec vec1(1.0f, 2.0f, 3.0f);
        ckVec vec2(3.0f, 5.0f, 7.0f);

        ckVec vec3 = vec1 - vec2;
        ckAssert(isEqual(vec3, ckVec(-2.0f, -3.0f, -4.0f)));

        vec1 -= vec2;
        ckAssert(isEqual(vec1, ckVec(-2.0f, -3.0f, -4.0f)));

        vec2 = -vec1;
        ckAssert(isEqual(vec2, ckVec(2.0f, 3.0f, 4.0f)));
    }

    /*
        ckVec operator*(r32 s) const
        friend ckVec operator*(r32 s, const ckVec& vec)
        void operator*=(r32 s)
    */
    {
        ckVec vec1(1.0f, 2.0f, 3.0f);

        ckVec vec2 = vec1 * 3.0f;
        ckAssert(isEqual(vec2, ckVec(3.0f, 6.0f, 9.0f)));

        ckVec vec3 = 3.0f * vec1;
        ckAssert(isEqual(vec3, ckVec(3.0f, 6.0f, 9.0f)));

        vec1 *= 3.0f;
        ckAssert(isEqual(vec1, ckVec(3.0f, 6.0f, 9.0f)));
    }

    /*
        ckVec operator/(r32 s) const
        void operator/=(r32 s)
    */
    {
        ckVec vec1(2.0f, 4.0f, 6.0f);

        ckVec vec2 = vec1 / 2.0f;
        ckAssert(isEqual(vec2, ckVec(1.0f, 2.0f, 3.0f)));

        vec1 /= 2.0f;
        ckAssert(isEqual(vec1, ckVec(1.0f, 2.0f, 3.0f)));
    }

    /*
        r32 length() const
        r32 sqLength() const
    */
    {
        ckVec vec(1.0f, 2.0f, 3.0);

        ckAssert(isEqual(vec.length(), ckMath::sqrt(14.0f)));

        ckAssert(isEqual(vec.sqLength(), 14.0f));
    }

    /*
        r32 dist(const ckVec& vec) const
        r32 sqDist(const ckVec& vec) const
    */
    {
        ckVec vec1(1.0f, 2.0f, 3.0);
        ckVec vec2(2.0f, 4.0f, 6.0);

        ckAssert(isEqual(vec1.dist(vec2), ckMath::sqrt(14.0f)));

        ckAssert(isEqual(vec1.sqDist(vec2), 14.0f));
    }

    /*
        r32 dot(const ckVec& vec) const
        ckVec cross(const ckVec& vec) const
    */
    {
        ckVec vec1(1.0f, 2.0f, 3.0);
        ckVec vec2(-2.0f, 3.0f, 4.0);

        ckAssert(isEqual(vec1.dot(vec2), 16.0f));

        ckAssert(isEqual(vec1.cross(vec2), ckVec(-1.0f, -10.0f, 7.0f)));
    }

    /*
        ckVec rotateX_r32(r32 deg) const
        ckVec rotateY_r32(r32 deg) const
        ckVec rotateZ_r32(r32 deg) const
    */
    {
        ckVec vec1(1.0f, 2.0f, 3.0f);

        ckVec vec2 = vec1.rotateX_r32(90.0f);
        ckAssert(isEqual(vec2, ckVec(1.0f, -3.0f, 2.0f)));

        ckVec vec3 = vec1.rotateY_r32(90.0f);
        ckAssert(isEqual(vec3, ckVec(3.0f, 2.0f, -1.0f)));

        ckVec vec4 = vec1.rotateZ_r32(90.0f);
        ckAssert(isEqual(vec4, ckVec(-2.0f, 1.0f, 3.0f)));
    }

    /*
        ckVec rotateX_s32(s32 deg) const
        ckVec rotateY_s32(s32 deg) const
        ckVec rotateZ_s32(s32 deg) const
    */
    {
        ckVec vec1(1.0f, 2.0f, 3.0f);

        ckVec vec2 = vec1.rotateX_s32(90);
        ckAssert(isEqual(vec2, ckVec(1.0f, -3.0f, 2.0f)));

        ckVec vec3 = vec1.rotateY_s32(90);
        ckAssert(isEqual(vec3, ckVec(3.0f, 2.0f, -1.0f)));

        ckVec vec4 = vec1.rotateZ_s32(90);
        ckAssert(isEqual(vec4, ckVec(-2.0f, 1.0f, 3.0f)));
    }

    /*
        ckVec normalize() const
    */
    {
        ckVec vec1(1.0f, 2.0f, 3.0f);

        ckVec vec2 = vec1.normalize();
        ckAssert(isEqual(vec2.length(), 1.0f));
        ckAssert(isEqual(vec2 * vec1.length(), vec1));
    }

    /*
        ckVec interp(const ckVec& to, r32 ratio) const
    */
    {
        ckVec vec1(1.0f, 2.0f, 3.0f);
        ckVec vec2(3.0f, 4.0f, 5.0f);

        ckVec vec3 = vec1.interp(vec2, 0.0f);
        ckAssert(isEqual(vec3, vec1));

        ckVec vec4 = vec1.interp(vec2, 0.25f);
        ckAssert(isEqual(vec4, ckVec(1.5f, 2.5f, 3.5f)));

        ckVec vec5 = vec1.interp(vec2, 1.0f);
        ckAssert(isEqual(vec5, vec2));
    }

    /*
        ckVec slerp(const ckVec& to, r32 ratio) const
    */
    {
        ckVec vec1 = ckVec(1.0f, 1.0f, 1.0f).normalize();
        ckVec vec2 = ckVec(-1.0f, 1.0f, -1.0f).normalize();

        ckVec vec3 = vec1.slerp(vec2, 0.0f);
        ckAssert(isEqual(vec3, vec1));

        ckVec vec4 = vec1.slerp(vec2, 0.25f).slerp(vec1.slerp(vec2, 0.75f), 0.5f);
        ckAssert(isEqual(vec4, ckVec(0.0f, 1.0f, 0.0f)));

        ckVec vec5 = vec1.slerp(vec2, 1.0f);
        ckAssert(isEqual(vec5, vec2));
    }

    /*
        ckVec toLocalOf(const ckMat& mat) const
        ckVec toGlobalFrom(const ckMat& mat) const
    */
    {
        ckMat mat( //
            ckVec(0.0f, 0.5f, 0.0f), //
            ckVec(0.0f, 0.0f, 3.0f), //
            ckVec(-0.5f, 0.0f, 0.0f), //
            ckVec(10.0f, 11.0f, 12.0f));

        ckVec vec1(9.0f, 12.0f, 15.0f);

        ckVec vec2 = vec1.toLocalOf(mat);
        ckAssert(isEqual(vec2, ckVec(2.0f, 1.0f, 2.0f)));

        ckVec vec3 = vec2.toGlobalFrom(mat);
        ckAssert(isEqual(vec3, vec1));
    }

    /*
        ckVec toLocalOf_noTrans(const ckMat& mat) const
        ckVec toGlobalFrom_noTrans(const ckMat& mat) const
    */
    {
        ckMat mat( //
            ckVec(0.0f, 0.5f, 0.0f), //
            ckVec(0.0f, 0.0f, 3.0f), //
            ckVec(-0.5f, 0.0f, 0.0f), //
            ckVec(10.0f, 11.0f, 12.0f));

        ckVec vec1(9.0f, 12.0f, 15.0f);

        ckVec vec2 = vec1.toLocalOf_noTrans(mat);
        ckAssert(isEqual(vec2, ckVec(24.0f, 5.0f, -18.0f)));

        ckVec vec3 = vec2.toGlobalFrom_noTrans(mat);
        ckAssert(isEqual(vec3, vec1));
    }

    /*
        void toR32x4(r32* r32x4, r32 w) const
        static ckVec fromR32x4(const r32* r32x4)
    */
    {
        ckVec vec1(0.1f, 0.2f, 0.3f);

        r32 v[4];
        vec1.toR32x4(v, 0.4f);
        ckAssert(isEqual(v[0], 0.1f) && isEqual(v[1], 0.2f) && isEqual(v[2], 0.3f) && isEqual(v[3], 0.4f));

        ckVec vec2 = ckVec::fromR32x4(v);
        ckAssert(isEqual(vec1, vec2));
    }

    /*
        static void mulR32x4(r32* res_r32x4, const r32* lhs_r32x16, const r32* rhs_r32x4)
    */
    {
        r32 v1[4] =
        {
            0.1f, 0.2f, 0.3f, 0.4f
        };

        r32 m[16] =
        {
            1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f
        };

        r32 v2[4];
        ckVec::mulR32x4(v2, m, v1);
        ckAssert(isEqual(v2[0], 9.0f) && isEqual(v2[1], 10.0f) && isEqual(v2[2], 11.0f) && isEqual(v2[3], 12.0f));

        ckVec::mulR32x4(v1, m, v1);
        ckAssert(isEqual(v1[0], 9.0f) && isEqual(v1[1], 10.0f) && isEqual(v1[2], 11.0f) && isEqual(v1[3], 12.0f));
    }
}
