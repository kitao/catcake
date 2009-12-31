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


void ckFixTest()
{
    /*
        ckFix()
        ckFix(s32 n)
        ckFix(r32 r)
        s32 toS32() const
        r32 toR32() const
    */
    {
        ckFix fix1;

        ckFix fix2 = 123;
        ckAssert(fix2.toS32() == 123);

        ckFix fix3 = 0.123f;
        ckAssert(isEqual(fix3.toR32(), 0.123f));

        ckFix fix4 = fix2;
        ckAssert(fix4.toS32() == 123);

        ckAssertThrow(ckFix(33000), ckFix::ExceptionOverflow);
        ckAssertThrow(ckFix(-33000), ckFix::ExceptionOverflow);
        ckAssertThrow(ckFix(33000.0f), ckFix::ExceptionOverflow);
        ckAssertThrow(ckFix(-33000.0f), ckFix::ExceptionOverflow);
        ckAssertThrow(ckFix(0.00001f), ckFix::ExceptionUnderflow);
    }

    /*
        bool operator==(ckFix fix) const
        bool operator!=(ckFix fix) const
        bool operator<(ckFix fix) const
        bool operator>(ckFix fix) const
        bool operator<=(ckFix fix) const
        bool operator>=(ckFix fix) const
    */
    {
        ckFix fix1 = 123;
        ckFix fix2 = 321;

        ckAssert(fix1 == fix1);
        ckAssert(!(fix1 == fix2));

        ckAssert(fix1 != fix2);
        ckAssert(!(fix1 != fix1));

        ckAssert(fix1 < fix2);
        ckAssert(!(fix1 < fix1));
        ckAssert(!(fix2 < fix1));

        ckAssert(fix2 > fix1);
        ckAssert(!(fix1 > fix1));
        ckAssert(!(fix1 > fix2));

        ckAssert(fix1 <= fix2);
        ckAssert(fix1 <= fix1);
        ckAssert(!(fix2 <= fix1));

        ckAssert(fix2 >= fix1);
        ckAssert(fix1 >= fix1);
        ckAssert(!(fix1 >= fix2));
    }

    /*
        ckFix operator+(ckFix fix) const
        friend ckFix operator+(s32 n, ckFix fix)
        friend ckFix operator+(r32 r, ckFix fix)
        void operator+=(ckFix fix)
    */
    {
        ckFix fix1 = 123;
        ckFix fix2 = 321;

        ckAssert(fix1 + fix2 == 444);
        ckAssert(123 + fix2 == 444);
        ckAssert(123.0f + fix2 == 444);

        fix1 += 321;
        ckAssert(fix1 == 444);

        ckAssertThrow(ckFix(20000) + ckFix(20000), ckFix::ExceptionOverflow);
        ckAssertThrow(ckFix(-20000) + ckFix(-20000), ckFix::ExceptionOverflow);
        ckAssertThrow(20000 + ckFix(20000), ckFix::ExceptionOverflow);
        ckAssertThrow(-20000 + ckFix(-20000), ckFix::ExceptionOverflow);
        ckAssertThrow(20000.0f + ckFix(20000), ckFix::ExceptionOverflow);
        ckAssertThrow(-20000.0f + ckFix(-20000), ckFix::ExceptionOverflow);
        ckAssertThrow(0.00001f + ckFix(20000), ckFix::ExceptionUnderflow);

        ckFix fix3 = 20000;
        ckAssertThrow(fix3 += 20000, ckFix::ExceptionOverflow);

        ckFix fix4 = -20000;
        ckAssertThrow(fix4 += -20000, ckFix::ExceptionOverflow);
    }

    /*
        ckFix operator-() const
        ckFix operator-(ckFix fix) const
        friend ckFix operator-(s32 n, ckFix fix)
        friend ckFix operator-(r32 r, ckFix fix)
        void operator-=(ckFix fix)
    */
    {
        ckFix fix1 = 200;
        ckFix fix2 = 300;

        ckAssert(-fix1 == -200);
        ckAssert(fix1 - fix2 == -100);
        ckAssert(200 - fix2 == -100);
        ckAssert(200.0f - fix2 == -100);

        fix1 -= 300;
        ckAssert(fix1 == -100);

        ckAssertThrow(-ckFix(-32768), ckFix::ExceptionOverflow);
        ckAssertThrow(ckFix(20000) - ckFix(-20000), ckFix::ExceptionOverflow);
        ckAssertThrow(ckFix(-20000) - ckFix(20000), ckFix::ExceptionOverflow);
        ckAssertThrow(20000 - ckFix(-20000), ckFix::ExceptionOverflow);
        ckAssertThrow(-20000 - ckFix(20000), ckFix::ExceptionOverflow);
        ckAssertThrow(20000.0f - ckFix(-20000), ckFix::ExceptionOverflow);
        ckAssertThrow(-20000.0f - ckFix(20000), ckFix::ExceptionOverflow);
        ckAssertThrow(0.00001f - ckFix(20000), ckFix::ExceptionUnderflow);

        ckFix fix3 = 20000;
        ckAssertThrow(fix3 -= -20000, ckFix::ExceptionOverflow);

        ckFix fix4 = -20000;
        ckAssertThrow(fix4 -= 20000, ckFix::ExceptionOverflow);
    }

    /*
        ckFix operator*(ckFix fix) const
        friend ckFix operator*(s32 n, ckFix fix)
        friend ckFix operator*(r32 r, ckFix fix)
        void operator*=(ckFix fix)
    */
    {
        ckFix fix1 = 22.2f;
        ckFix fix2 = 0.5f;

        ckAssert(isEqual((fix1 * fix2).toR32(), 11.1f));
        ckAssert(222 * fix2 == 111);
        ckAssert(isEqual((22.2f * fix2).toR32(), 11.1f));

        fix1 *= fix2;
        ckAssert(isEqual(fix1.toR32(), 11.1f));

        ckAssertThrow(ckFix(200) * ckFix(200), ckFix::ExceptionOverflow);
        ckAssertThrow(ckFix(-200) * ckFix(200), ckFix::ExceptionOverflow);
        ckAssertThrow(200 * ckFix(200), ckFix::ExceptionOverflow);
        ckAssertThrow(-200 * ckFix(200), ckFix::ExceptionOverflow);
        ckAssertThrow(200.0f * ckFix(200), ckFix::ExceptionOverflow);
        ckAssertThrow(-200.0f * ckFix(200), ckFix::ExceptionOverflow);
        ckAssertThrow(0.001f * ckFix(0.001f), ckFix::ExceptionUnderflow);

        ckFix fix3 = 200;
        ckAssertThrow(fix3 *= 200, ckFix::ExceptionOverflow);

        ckFix fix4 = -200;
        ckAssertThrow(fix4 *= 200, ckFix::ExceptionOverflow);
    }

    /*
        ckFix operator/(ckFix fix) const
        CK_API friend ckFix operator/(s32 n, ckFix fix)
        CK_API friend ckFix operator/(r32 r, ckFix fix)
        void operator/=(ckFix fix)
    */
    {
        ckFix fix1 = 22.2f;
        ckFix fix2 = 0.5f;

        ckAssert(isEqual((fix1 / fix2).toR32(), 44.4f));
        ckAssert(222 / fix2 == 444);
        ckAssert(isEqual((22.2f / fix2).toR32(), 44.4f));

        fix1 /= fix2;
        ckAssert(isEqual(fix1.toR32(), 44.4f));

        ckAssertThrow(ckFix(400) / ckFix(0.01f), ckFix::ExceptionOverflow);
        ckAssertThrow(ckFix(-400) / ckFix(0.01f), ckFix::ExceptionOverflow);
        ckAssertThrow(400 / ckFix(0.01f), ckFix::ExceptionOverflow);
        ckAssertThrow(-400 / ckFix(0.01f), ckFix::ExceptionOverflow);
        ckAssertThrow(400.0f / ckFix(0.01f), ckFix::ExceptionOverflow);
        ckAssertThrow(-400.0f / ckFix(0.01f), ckFix::ExceptionOverflow);
        ckAssertThrow(0.001f / ckFix(1000), ckFix::ExceptionUnderflow);

        ckFix fix3 = 400;
        ckAssertThrow(fix3 /= 0.01f, ckFix::ExceptionOverflow);

        ckFix fix4 = -400;
        ckAssertThrow(fix4 /= 0.01f, ckFix::ExceptionOverflow);
    }

    /*
        s32 getValue() const
        static ckFix fromValue(s32 value)
    */
    {
        ckFix fix = 123;

        ckAssert(fix.getValue() == 123 << 16);

        ckAssert(ckFix::fromValue(fix.getValue()) == fix);
    }
}
