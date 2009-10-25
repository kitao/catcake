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


void pgMathTest()
{
    /*
        static const r32 EPSILON
        static const r32 PI
        static const r32 DEG_TO_RAD
        static const r32 RAD_TO_DEG
    */
    {
        pgAssert(pgMath::abs(pgMath::EPSILON - 0.0001f) < 1e-6f);
        pgAssert(isEqual(pgMath::PI, 3.1415926535f));
        pgAssert(isEqual(pgMath::DEG_TO_RAD, pgMath::PI / 180.0f));
        pgAssert(isEqual(pgMath::RAD_TO_DEG, 180.0f / pgMath::PI));
    }

    /*
        template<class T> static T abs(T x)
    */
    {
        pgAssert(pgMath::abs(100) == 100 && pgMath::abs(-100) == 100);
        pgAssert(isEqual(pgMath::abs(100.0f), 100.0f));
        pgAssert(isEqual(pgMath::abs(-100.0f), 100.0f));
    }

    /*
        template<class T> static T min(T a, T b)
    */
    {
        pgAssert(pgMath::min(200, 100) == 100 && pgMath::min(-200, -100) == -200);
        pgAssert(isEqual(pgMath::min(200.0f, 100.0f), 100.0f));
        pgAssert(isEqual(pgMath::min(-200.0f, -100.0f), -200.0f));
    }

    /*
        template<class T> static T max(T a, T b)
    */
    {
        pgAssert(pgMath::max(200, 100) == 200 && pgMath::max(-200, -100) == -100);
        pgAssert(isEqual(pgMath::max(200.0f, 100.0f), 200.0f));
        pgAssert(isEqual(pgMath::max(-200.0f, -100.0f), -100.0f));
    }

    /*
        template<class T> static T clamp(T x, T min, T max)
    */
    {
        pgAssert(pgMath::clamp(-101, -100, 50) == -100);
        pgAssert(pgMath::clamp(101, -50, 100) == 100);
        pgAssert(pgMath::clamp(0, -50, 50) == 0);
    }

    /*
        static r32 sqrt(r32 x)
    */
    {
        pgAssert(isEqual(pgMath::sqrt(4.0f), 2.0f));
        pgAssert(isEqual(pgMath::sqrt(100.0f), 10.0f));
    }

    /*
        static r32 sin_r32(r32 deg)
    */
    {
        pgAssert(isEqual(pgMath::sin_r32(-360.0f), 0.0f));
        pgAssert(isEqual(pgMath::sin_r32(-270.0f), 1.0f));
        pgAssert(isEqual(pgMath::sin_r32(-180.0f), 0.0f));
        pgAssert(isEqual(pgMath::sin_r32(-90.0f), -1.0f));
        pgAssert(isEqual(pgMath::sin_r32(0.0f), 0.0f));
        pgAssert(isEqual(pgMath::sin_r32(90.0f), 1.0f));
        pgAssert(isEqual(pgMath::sin_r32(180.0f), 0.0f));
        pgAssert(isEqual(pgMath::sin_r32(270.0f), -1.0f));
        pgAssert(isEqual(pgMath::sin_r32(360.0f), 0.0f));
    }

    /*
        static r32 cos_r32(r32 deg)
    */
    {
        pgAssert(isEqual(pgMath::cos_r32(-360.0f), 1.0f));
        pgAssert(isEqual(pgMath::cos_r32(-270.0f), 0.0f));
        pgAssert(isEqual(pgMath::cos_r32(-180.0f), -1.0f));
        pgAssert(isEqual(pgMath::cos_r32(-90.0f), 0.0f));
        pgAssert(isEqual(pgMath::cos_r32(0.0f), 1.0f));
        pgAssert(isEqual(pgMath::cos_r32(90.0f), 0.0f));
        pgAssert(isEqual(pgMath::cos_r32(180.0f), -1.0f));
        pgAssert(isEqual(pgMath::cos_r32(270.0f), 0.0f));
        pgAssert(isEqual(pgMath::cos_r32(360.0f), 1.0f));
    }

    /*
        static r32 sin_s32(s32 deg)
    */
    {
        for (s32 i = -720; i <= 720; i++)
        {
            pgAssert(isEqual(pgMath::sin_s32(i), pgMath::sin_r32(static_cast<r32>(i))));
        }
    }

    /*
        static r32 cos_s32(s32 deg)
    */
    {
        for (s32 i = -720; i <= 720; i++)
        {
            pgAssert(isEqual(pgMath::cos_s32(i), pgMath::cos_r32(static_cast<r32>(i))));
        }
    }

    /*
        static r32 asin(r32 x)
    */
    {
        pgAssert(isEqual(pgMath::asin(-1.0f), -90.0f));
        pgAssert(isEqual(pgMath::asin(0.0f), 0.0f));
        pgAssert(isEqual(pgMath::asin(1.0f), 90.0f));
    }

    /*
        static r32 acos(r32 x)
    */
    {
        pgAssert(isEqual(pgMath::acos(-1.0f), 180.0f));
        pgAssert(isEqual(pgMath::acos(0.0f), 90.0f));
        pgAssert(isEqual(pgMath::acos(1.0f), 0.0f));
    }

    /*
        static r32 atan2(r32 y, r32 x)
    */
    {
        pgAssert(isEqual(pgMath::atan2(0.0f, 2.0f), 0.0f));
        pgAssert(isEqual(pgMath::atan2(2.0f, 0.0f), 90.0f));
        pgAssert(isEqual(pgMath::atan2(0.0f, -2.0f), 180.0f));
        pgAssert(isEqual(pgMath::atan2(-2.0f, 0.0f), -90.0f));
    }

    /*
        static void srand(u32 seed)
        static s32 rand(s32 from, s32 to)
    */
    {
        u32 flag1 = 0, flag2 = 0;

        for (s32 i = 0; i < 1000; i++)
        {
            pgMath::srand(i);

            {
                s32 n = pgMath::rand(-1, 2);
                pgAssert(n == -1 || n == 0 || n == 1 || n == 2);

                flag1 |= 1 << (n + 1);
            }

            {
                s32 n = pgMath::rand(1, -2);
                pgAssert(n == -2 || n == -1 || n == 0 || n == 1);

                flag2 |= 1 << (n + 2);
            }
        }

        pgAssert(flag1 == 0x0f && flag2 == 0x0f);
    }

    /*
        static r32 rand(r32 from, r32 to, r32 interval)
    */
    {
        u32 flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0, flag5 = 0;

        for (s32 i = 0; i < 1000; i++)
        {
            pgMath::srand(i);

            {
                r32 r = pgMath::rand(-2.0f, 2.0f, 2.0f);
                pgAssert(isEqual(r, -2.0f) || isEqual(r, 0.0f) || isEqual(r, 2.0f));

                flag1 |= 1 << (static_cast<s32>(r) + 2);
            }

            {
                r32 r = pgMath::rand(-1.0f, 4.0f, 2.0f);
                pgAssert(isEqual(r, -1.0f) || isEqual(r, 1.0f) || isEqual(r, 3.0f));

                flag2 |= 1 << (static_cast<s32>(r) + 1);
            }

            {
                r32 r = pgMath::rand(-1.0f, 4.0f, -2.0f);
                pgAssert(isEqual(r, -1.0f) || isEqual(r, 1.0f) || isEqual(r, 3.0f));

                flag3 |= 1 << (static_cast<s32>(r) + 1);
            }

            {
                r32 r = pgMath::rand(4.0f, -1.0f, 2.0f);
                pgAssert(isEqual(r, 4.0f) || isEqual(r, 2.0f) || isEqual(r, 0.0f));

                flag4 |= 1 << static_cast<s32>(r);
            }

            {
                r32 r = pgMath::rand(4.0f, -1.0f, -2.0f);
                pgAssert(isEqual(r, 4.0f) || isEqual(r, 2.0f) || isEqual(r, 0.0f));

                flag5 |= 1 << static_cast<s32>(r);
            }
        }

        pgAssert(flag1 == 0x15 && flag2 == 0x15 && flag3 == 0x15 && flag4 == 0x15 && flag5 == 0x15);
    }

    /*
        static r32 interp(r32 from, r32 to, r32 ratio)
    */
    {
        pgAssert(isEqual(pgMath::interp(1.0f, -3.0f, 0.0f), 1.0f));
        pgAssert(isEqual(pgMath::interp(1.0f, -3.0f, 0.25f), 0.0f));
        pgAssert(isEqual(pgMath::interp(1.0f, -3.0f, 1.0f), -3.0f));
    }
}
