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


void ckMathTest()
{
    /*
        static const r32 EPSILON
        static const r32 PI
        static const r32 DEG_TO_RAD
        static const r32 RAD_TO_DEG
    */
    {
        ckAssert(ckMath::abs(ckMath::EPSILON - 0.0001f) < 1e-6f);
        ckAssert(isEqual(ckMath::PI, 3.1415926535f));
        ckAssert(isEqual(ckMath::DEG_TO_RAD, ckMath::PI / 180.0f));
        ckAssert(isEqual(ckMath::RAD_TO_DEG, 180.0f / ckMath::PI));
    }

    /*
        template<class T> static T abs(T x)
    */
    {
        ckAssert(ckMath::abs(100) == 100 && ckMath::abs(-100) == 100);
        ckAssert(isEqual(ckMath::abs(100.0f), 100.0f));
        ckAssert(isEqual(ckMath::abs(-100.0f), 100.0f));
    }

    /*
        template<class T> static T min(T a, T b)
    */
    {
        ckAssert(ckMath::min(200, 100) == 100 && ckMath::min(-200, -100) == -200);
        ckAssert(isEqual(ckMath::min(200.0f, 100.0f), 100.0f));
        ckAssert(isEqual(ckMath::min(-200.0f, -100.0f), -200.0f));
    }

    /*
        template<class T> static T max(T a, T b)
    */
    {
        ckAssert(ckMath::max(200, 100) == 200 && ckMath::max(-200, -100) == -100);
        ckAssert(isEqual(ckMath::max(200.0f, 100.0f), 200.0f));
        ckAssert(isEqual(ckMath::max(-200.0f, -100.0f), -100.0f));
    }

    /*
        template<class T> static T clamp(T x, T min, T max)
    */
    {
        ckAssert(ckMath::clamp(-101, -100, 50) == -100);
        ckAssert(ckMath::clamp(101, -50, 100) == 100);
        ckAssert(ckMath::clamp(0, -50, 50) == 0);
    }

    /*
        static r32 sqrt(r32 x)
    */
    {
        ckAssert(isEqual(ckMath::sqrt(4.0f), 2.0f));
        ckAssert(isEqual(ckMath::sqrt(100.0f), 10.0f));
    }

    /*
        static r32 sin_r32(r32 deg)
    */
    {
        ckAssert(isEqual(ckMath::sin_r32(-360.0f), 0.0f));
        ckAssert(isEqual(ckMath::sin_r32(-270.0f), 1.0f));
        ckAssert(isEqual(ckMath::sin_r32(-180.0f), 0.0f));
        ckAssert(isEqual(ckMath::sin_r32(-90.0f), -1.0f));
        ckAssert(isEqual(ckMath::sin_r32(0.0f), 0.0f));
        ckAssert(isEqual(ckMath::sin_r32(90.0f), 1.0f));
        ckAssert(isEqual(ckMath::sin_r32(180.0f), 0.0f));
        ckAssert(isEqual(ckMath::sin_r32(270.0f), -1.0f));
        ckAssert(isEqual(ckMath::sin_r32(360.0f), 0.0f));
    }

    /*
        static r32 cos_r32(r32 deg)
    */
    {
        ckAssert(isEqual(ckMath::cos_r32(-360.0f), 1.0f));
        ckAssert(isEqual(ckMath::cos_r32(-270.0f), 0.0f));
        ckAssert(isEqual(ckMath::cos_r32(-180.0f), -1.0f));
        ckAssert(isEqual(ckMath::cos_r32(-90.0f), 0.0f));
        ckAssert(isEqual(ckMath::cos_r32(0.0f), 1.0f));
        ckAssert(isEqual(ckMath::cos_r32(90.0f), 0.0f));
        ckAssert(isEqual(ckMath::cos_r32(180.0f), -1.0f));
        ckAssert(isEqual(ckMath::cos_r32(270.0f), 0.0f));
        ckAssert(isEqual(ckMath::cos_r32(360.0f), 1.0f));
    }

    /*
        static r32 sin_s32(s32 deg)
    */
    {
        for (s32 i = -720; i <= 720; i++)
        {
            ckAssert(isEqual(ckMath::sin_s32(i), ckMath::sin_r32(static_cast<r32>(i))));
        }
    }

    /*
        static r32 cos_s32(s32 deg)
    */
    {
        for (s32 i = -720; i <= 720; i++)
        {
            ckAssert(isEqual(ckMath::cos_s32(i), ckMath::cos_r32(static_cast<r32>(i))));
        }
    }

    /*
        static r32 asin(r32 x)
    */
    {
        ckAssert(isEqual(ckMath::asin(-1.0f), -90.0f));
        ckAssert(isEqual(ckMath::asin(0.0f), 0.0f));
        ckAssert(isEqual(ckMath::asin(1.0f), 90.0f));
    }

    /*
        static r32 acos(r32 x)
    */
    {
        ckAssert(isEqual(ckMath::acos(-1.0f), 180.0f));
        ckAssert(isEqual(ckMath::acos(0.0f), 90.0f));
        ckAssert(isEqual(ckMath::acos(1.0f), 0.0f));
    }

    /*
        static r32 atan2(r32 y, r32 x)
    */
    {
        ckAssert(isEqual(ckMath::atan2(0.0f, 2.0f), 0.0f));
        ckAssert(isEqual(ckMath::atan2(2.0f, 0.0f), 90.0f));
        ckAssert(isEqual(ckMath::atan2(0.0f, -2.0f), 180.0f));
        ckAssert(isEqual(ckMath::atan2(-2.0f, 0.0f), -90.0f));
    }

    /*
        static void srand(u32 seed)
        static s32 rand(s32 from, s32 to)
    */
    {
        u32 flag1 = 0, flag2 = 0;

        for (s32 i = 0; i < 1000; i++)
        {
            ckMath::srand(i);

            {
                s32 n = ckMath::rand(-1, 2);
                ckAssert(n == -1 || n == 0 || n == 1 || n == 2);

                flag1 |= 1 << (n + 1);
            }

            {
                s32 n = ckMath::rand(1, -2);
                ckAssert(n == -2 || n == -1 || n == 0 || n == 1);

                flag2 |= 1 << (n + 2);
            }
        }

        ckAssert(flag1 == 0x0f && flag2 == 0x0f);
    }

    /*
        static r32 rand(r32 from, r32 to, r32 interval)
    */
    {
        u32 flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0, flag5 = 0;

        for (s32 i = 0; i < 1000; i++)
        {
            ckMath::srand(i);

            {
                r32 r = ckMath::rand(-2.0f, 2.0f, 2.0f);
                ckAssert(isEqual(r, -2.0f) || isEqual(r, 0.0f) || isEqual(r, 2.0f));

                flag1 |= 1 << (static_cast<s32>(r) + 2);
            }

            {
                r32 r = ckMath::rand(-1.0f, 4.0f, 2.0f);
                ckAssert(isEqual(r, -1.0f) || isEqual(r, 1.0f) || isEqual(r, 3.0f));

                flag2 |= 1 << (static_cast<s32>(r) + 1);
            }

            {
                r32 r = ckMath::rand(-1.0f, 4.0f, -2.0f);
                ckAssert(isEqual(r, -1.0f) || isEqual(r, 1.0f) || isEqual(r, 3.0f));

                flag3 |= 1 << (static_cast<s32>(r) + 1);
            }

            {
                r32 r = ckMath::rand(4.0f, -1.0f, 2.0f);
                ckAssert(isEqual(r, 4.0f) || isEqual(r, 2.0f) || isEqual(r, 0.0f));

                flag4 |= 1 << static_cast<s32>(r);
            }

            {
                r32 r = ckMath::rand(4.0f, -1.0f, -2.0f);
                ckAssert(isEqual(r, 4.0f) || isEqual(r, 2.0f) || isEqual(r, 0.0f));

                flag5 |= 1 << static_cast<s32>(r);
            }
        }

        ckAssert(flag1 == 0x15 && flag2 == 0x15 && flag3 == 0x15 && flag4 == 0x15 && flag5 == 0x15);
    }

    /*
        static r32 interp(r32 from, r32 to, r32 ratio)
    */
    {
        ckAssert(isEqual(ckMath::interp(1.0f, -3.0f, 0.0f), 1.0f));
        ckAssert(isEqual(ckMath::interp(1.0f, -3.0f, 0.25f), 0.0f));
        ckAssert(isEqual(ckMath::interp(1.0f, -3.0f, 1.0f), -3.0f));
    }
}
