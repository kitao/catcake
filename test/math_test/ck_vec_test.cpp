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


void pgVecTest()
{
    /*
        static const pgVec ZERO
        static const pgVec X_UNIT
        static const pgVec Y_UNIT
        static const pgVec Z_UNIT
    */
    {
        pgAssert(isEqual(pgVec::ZERO, pgVec(0.0f, 0.0f, 0.0f)));
        pgAssert(isEqual(pgVec::X_UNIT, pgVec(1.0f, 0.0f, 0.0f)));
        pgAssert(isEqual(pgVec::Y_UNIT, pgVec(0.0f, 1.0f, 0.0f)));
        pgAssert(isEqual(pgVec::Z_UNIT, pgVec(0.0f, 0.0f, 1.0f)));
    }

    /*
        pgVec()
        pgVec(r32 x_, r32 y_, r32 z_ = 0.0f)
        void set(r32 x_, r32 y_, r32 z_ = 0.0f)
    */
    {
        pgVec vec1;

        vec1.set(1.0f, 2.0f, 3.0f);
        pgAssert(isEqual(vec1.x, 1.0f) && isEqual(vec1.y, 2.0f) && isEqual(vec1.z, 3.0f));

        pgVec vec2 = vec1;
        pgAssert(isEqual(vec2.x, 1.0f) && isEqual(vec2.y, 2.0f) && isEqual(vec2.z, 3.0f));

        pgVec vec3(1.0f, 2.0f, 3.0f);
        pgAssert(isEqual(vec3, vec1));
    }

    /*
        pgVec operator+(const pgVec& vec) const
        void operator+=(const pgVec& vec)
    */
    {
        pgVec vec1(1.0f, 2.0f, 3.0f);
        pgVec vec2(3.0f, 4.0f, 5.0f);

        pgVec vec3 = vec1 + vec2;
        pgAssert(isEqual(vec3, pgVec(4.0f, 6.0f, 8.0f)));

        vec1 += vec2;
        pgAssert(isEqual(vec1, pgVec(4.0f, 6.0f, 8.0f)));
    }

    /*
        pgVec operator-() const
        pgVec operator-(const pgVec& vec) const
        void operator-=(const pgVec& vec)
    */
    {
        pgVec vec1(1.0f, 2.0f, 3.0f);
        pgVec vec2(3.0f, 5.0f, 7.0f);

        pgVec vec3 = vec1 - vec2;
        pgAssert(isEqual(vec3, pgVec(-2.0f, -3.0f, -4.0f)));

        vec1 -= vec2;
        pgAssert(isEqual(vec1, pgVec(-2.0f, -3.0f, -4.0f)));

        vec2 = -vec1;
        pgAssert(isEqual(vec2, pgVec(2.0f, 3.0f, 4.0f)));
    }

    /*
        pgVec operator*(r32 s) const
        friend pgVec operator*(r32 s, const pgVec& vec)
        void operator*=(r32 s)
    */
    {
        pgVec vec1(1.0f, 2.0f, 3.0f);

        pgVec vec2 = vec1 * 3.0f;
        pgAssert(isEqual(vec2, pgVec(3.0f, 6.0f, 9.0f)));

        pgVec vec3 = 3.0f * vec1;
        pgAssert(isEqual(vec3, pgVec(3.0f, 6.0f, 9.0f)));

        vec1 *= 3.0f;
        pgAssert(isEqual(vec1, pgVec(3.0f, 6.0f, 9.0f)));
    }

    /*
        pgVec operator/(r32 s) const
        void operator/=(r32 s)
    */
    {
        pgVec vec1(2.0f, 4.0f, 6.0f);

        pgVec vec2 = vec1 / 2.0f;
        pgAssert(isEqual(vec2, pgVec(1.0f, 2.0f, 3.0f)));

        vec1 /= 2.0f;
        pgAssert(isEqual(vec1, pgVec(1.0f, 2.0f, 3.0f)));
    }

    /*
        r32 length() const
        r32 sqLength() const
    */
    {
        pgVec vec(1.0f, 2.0f, 3.0);

        pgAssert(isEqual(vec.length(), pgMath::sqrt(14.0f)));

        pgAssert(isEqual(vec.sqLength(), 14.0f));
    }

    /*
        r32 dist(const pgVec& vec) const
        r32 sqDist(const pgVec& vec) const
    */
    {
        pgVec vec1(1.0f, 2.0f, 3.0);
        pgVec vec2(2.0f, 4.0f, 6.0);

        pgAssert(isEqual(vec1.dist(vec2), pgMath::sqrt(14.0f)));

        pgAssert(isEqual(vec1.sqDist(vec2), 14.0f));
    }

    /*
        r32 dot(const pgVec& vec) const
        pgVec cross(const pgVec& vec) const
    */
    {
        pgVec vec1(1.0f, 2.0f, 3.0);
        pgVec vec2(-2.0f, 3.0f, 4.0);

        pgAssert(isEqual(vec1.dot(vec2), 16.0f));

        pgAssert(isEqual(vec1.cross(vec2), pgVec(-1.0f, -10.0f, 7.0f)));
    }

    /*
        pgVec rotateX_r32(r32 deg) const
        pgVec rotateY_r32(r32 deg) const
        pgVec rotateZ_r32(r32 deg) const
    */
    {
        pgVec vec1(1.0f, 2.0f, 3.0f);

        pgVec vec2 = vec1.rotateX_r32(90.0f);
        pgAssert(isEqual(vec2, pgVec(1.0f, -3.0f, 2.0f)));

        pgVec vec3 = vec1.rotateY_r32(90.0f);
        pgAssert(isEqual(vec3, pgVec(3.0f, 2.0f, -1.0f)));

        pgVec vec4 = vec1.rotateZ_r32(90.0f);
        pgAssert(isEqual(vec4, pgVec(-2.0f, 1.0f, 3.0f)));
    }

    /*
        pgVec rotateX_s32(s32 deg) const
        pgVec rotateY_s32(s32 deg) const
        pgVec rotateZ_s32(s32 deg) const
    */
    {
        pgVec vec1(1.0f, 2.0f, 3.0f);

        pgVec vec2 = vec1.rotateX_s32(90);
        pgAssert(isEqual(vec2, pgVec(1.0f, -3.0f, 2.0f)));

        pgVec vec3 = vec1.rotateY_s32(90);
        pgAssert(isEqual(vec3, pgVec(3.0f, 2.0f, -1.0f)));

        pgVec vec4 = vec1.rotateZ_s32(90);
        pgAssert(isEqual(vec4, pgVec(-2.0f, 1.0f, 3.0f)));
    }

    /*
        pgVec normalize() const
    */
    {
        pgVec vec1(1.0f, 2.0f, 3.0f);

        pgVec vec2 = vec1.normalize();
        pgAssert(isEqual(vec2.length(), 1.0f));
        pgAssert(isEqual(vec2 * vec1.length(), vec1));
    }

    /*
        pgVec interp(const pgVec& to, r32 ratio) const
    */
    {
        pgVec vec1(1.0f, 2.0f, 3.0f);
        pgVec vec2(3.0f, 4.0f, 5.0f);

        pgVec vec3 = vec1.interp(vec2, 0.0f);
        pgAssert(isEqual(vec3, vec1));

        pgVec vec4 = vec1.interp(vec2, 0.25f);
        pgAssert(isEqual(vec4, pgVec(1.5f, 2.5f, 3.5f)));

        pgVec vec5 = vec1.interp(vec2, 1.0f);
        pgAssert(isEqual(vec5, vec2));
    }

    /*
        pgVec slerp(const pgVec& to, r32 ratio) const
    */
    {
        pgVec vec1 = pgVec(1.0f, 1.0f, 1.0f).normalize();
        pgVec vec2 = pgVec(-1.0f, 1.0f, -1.0f).normalize();

        pgVec vec3 = vec1.slerp(vec2, 0.0f);
        pgAssert(isEqual(vec3, vec1));

        pgVec vec4 = vec1.slerp(vec2, 0.25f).slerp(vec1.slerp(vec2, 0.75f), 0.5f);
        pgAssert(isEqual(vec4, pgVec(0.0f, 1.0f, 0.0f)));

        pgVec vec5 = vec1.slerp(vec2, 1.0f);
        pgAssert(isEqual(vec5, vec2));
    }

    /*
        pgVec toLocalOf(const pgMat& mat) const
        pgVec toGlobalFrom(const pgMat& mat) const
    */
    {
        pgMat mat( //
            pgVec(0.0f, 0.5f, 0.0f), //
            pgVec(0.0f, 0.0f, 3.0f), //
            pgVec(-0.5f, 0.0f, 0.0f), //
            pgVec(10.0f, 11.0f, 12.0f));

        pgVec vec1(9.0f, 12.0f, 15.0f);

        pgVec vec2 = vec1.toLocalOf(mat);
        pgAssert(isEqual(vec2, pgVec(2.0f, 1.0f, 2.0f)));

        pgVec vec3 = vec2.toGlobalFrom(mat);
        pgAssert(isEqual(vec3, vec1));
    }

    /*
        pgVec toLocalOf_noTrans(const pgMat& mat) const
        pgVec toGlobalFrom_noTrans(const pgMat& mat) const
    */
    {
        pgMat mat( //
            pgVec(0.0f, 0.5f, 0.0f), //
            pgVec(0.0f, 0.0f, 3.0f), //
            pgVec(-0.5f, 0.0f, 0.0f), //
            pgVec(10.0f, 11.0f, 12.0f));

        pgVec vec1(9.0f, 12.0f, 15.0f);

        pgVec vec2 = vec1.toLocalOf_noTrans(mat);
        pgAssert(isEqual(vec2, pgVec(24.0f, 5.0f, -18.0f)));

        pgVec vec3 = vec2.toGlobalFrom_noTrans(mat);
        pgAssert(isEqual(vec3, vec1));
    }

    /*
        void toR32x4(r32* r32x4, r32 w) const
        static pgVec fromR32x4(const r32* r32x4)
    */
    {
        pgVec vec1(0.1f, 0.2f, 0.3f);

        r32 v[4];
        vec1.toR32x4(v, 0.4f);
        pgAssert(isEqual(v[0], 0.1f) && isEqual(v[1], 0.2f) && isEqual(v[2], 0.3f) && isEqual(v[3], 0.4f));

        pgVec vec2 = pgVec::fromR32x4(v);
        pgAssert(isEqual(vec1, vec2));
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
        pgVec::mulR32x4(v2, m, v1);
        pgAssert(isEqual(v2[0], 9.0f) && isEqual(v2[1], 10.0f) && isEqual(v2[2], 11.0f) && isEqual(v2[3], 12.0f));

        pgVec::mulR32x4(v1, m, v1);
        pgAssert(isEqual(v1[0], 9.0f) && isEqual(v1[1], 10.0f) && isEqual(v1[2], 11.0f) && isEqual(v1[3], 12.0f));
    }
}
