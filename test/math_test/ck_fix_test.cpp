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


void pgFixTest()
{
    /*
        pgFix()
        pgFix(s32 n)
        pgFix(r32 r)
        s32 toS32() const
        r32 toR32() const
    */
    {
        pgFix fix1;

        pgFix fix2 = 123;
        pgAssert(fix2.toS32() == 123);

        pgFix fix3 = 0.123f;
        pgAssert(isEqual(fix3.toR32(), 0.123f));

        pgFix fix4 = fix2;
        pgAssert(fix4.toS32() == 123);

        pgAssertThrow(pgFix(33000), pgFix::ExceptionOverflow);
        pgAssertThrow(pgFix(-33000), pgFix::ExceptionOverflow);
        pgAssertThrow(pgFix(33000.0f), pgFix::ExceptionOverflow);
        pgAssertThrow(pgFix(-33000.0f), pgFix::ExceptionOverflow);
        pgAssertThrow(pgFix(0.00001f), pgFix::ExceptionUnderflow);
    }

    /*
        bool operator==(pgFix fix) const
        bool operator!=(pgFix fix) const
        bool operator<(pgFix fix) const
        bool operator>(pgFix fix) const
        bool operator<=(pgFix fix) const
        bool operator>=(pgFix fix) const
    */
    {
        pgFix fix1 = 123;
        pgFix fix2 = 321;

        pgAssert(fix1 == fix1);
        pgAssert(!(fix1 == fix2));

        pgAssert(fix1 != fix2);
        pgAssert(!(fix1 != fix1));

        pgAssert(fix1 < fix2);
        pgAssert(!(fix1 < fix1));
        pgAssert(!(fix2 < fix1));

        pgAssert(fix2 > fix1);
        pgAssert(!(fix1 > fix1));
        pgAssert(!(fix1 > fix2));

        pgAssert(fix1 <= fix2);
        pgAssert(fix1 <= fix1);
        pgAssert(!(fix2 <= fix1));

        pgAssert(fix2 >= fix1);
        pgAssert(fix1 >= fix1);
        pgAssert(!(fix1 >= fix2));
    }

    /*
        pgFix operator+(pgFix fix) const
        friend pgFix operator+(s32 n, pgFix fix)
        friend pgFix operator+(r32 r, pgFix fix)
        void operator+=(pgFix fix)
    */
    {
        pgFix fix1 = 123;
        pgFix fix2 = 321;

        pgAssert(fix1 + fix2 == 444);
        pgAssert(123 + fix2 == 444);
        pgAssert(123.0f + fix2 == 444);

        fix1 += 321;
        pgAssert(fix1 == 444);

        pgAssertThrow(pgFix(20000) + pgFix(20000), pgFix::ExceptionOverflow);
        pgAssertThrow(pgFix(-20000) + pgFix(-20000), pgFix::ExceptionOverflow);
        pgAssertThrow(20000 + pgFix(20000), pgFix::ExceptionOverflow);
        pgAssertThrow(-20000 + pgFix(-20000), pgFix::ExceptionOverflow);
        pgAssertThrow(20000.0f + pgFix(20000), pgFix::ExceptionOverflow);
        pgAssertThrow(-20000.0f + pgFix(-20000), pgFix::ExceptionOverflow);
        pgAssertThrow(0.00001f + pgFix(20000), pgFix::ExceptionUnderflow);

        pgFix fix3 = 20000;
        pgAssertThrow(fix3 += 20000, pgFix::ExceptionOverflow);

        pgFix fix4 = -20000;
        pgAssertThrow(fix4 += -20000, pgFix::ExceptionOverflow);
    }

    /*
        pgFix operator-() const
        pgFix operator-(pgFix fix) const
        friend pgFix operator-(s32 n, pgFix fix)
        friend pgFix operator-(r32 r, pgFix fix)
        void operator-=(pgFix fix)
    */
    {
        pgFix fix1 = 200;
        pgFix fix2 = 300;

        pgAssert(-fix1 == -200);
        pgAssert(fix1 - fix2 == -100);
        pgAssert(200 - fix2 == -100);
        pgAssert(200.0f - fix2 == -100);

        fix1 -= 300;
        pgAssert(fix1 == -100);

        pgAssertThrow(-pgFix(-32768), pgFix::ExceptionOverflow);
        pgAssertThrow(pgFix(20000) - pgFix(-20000), pgFix::ExceptionOverflow);
        pgAssertThrow(pgFix(-20000) - pgFix(20000), pgFix::ExceptionOverflow);
        pgAssertThrow(20000 - pgFix(-20000), pgFix::ExceptionOverflow);
        pgAssertThrow(-20000 - pgFix(20000), pgFix::ExceptionOverflow);
        pgAssertThrow(20000.0f - pgFix(-20000), pgFix::ExceptionOverflow);
        pgAssertThrow(-20000.0f - pgFix(20000), pgFix::ExceptionOverflow);
        pgAssertThrow(0.00001f - pgFix(20000), pgFix::ExceptionUnderflow);

        pgFix fix3 = 20000;
        pgAssertThrow(fix3 -= -20000, pgFix::ExceptionOverflow);

        pgFix fix4 = -20000;
        pgAssertThrow(fix4 -= 20000, pgFix::ExceptionOverflow);
    }

    /*
        pgFix operator*(pgFix fix) const
        friend pgFix operator*(s32 n, pgFix fix)
        friend pgFix operator*(r32 r, pgFix fix)
        void operator*=(pgFix fix)
    */
    {
        pgFix fix1 = 22.2f;
        pgFix fix2 = 0.5f;

        pgAssert(isEqual((fix1 * fix2).toR32(), 11.1f));
        pgAssert(222 * fix2 == 111);
        pgAssert(isEqual((22.2f * fix2).toR32(), 11.1f));

        fix1 *= fix2;
        pgAssert(isEqual(fix1.toR32(), 11.1f));

        pgAssertThrow(pgFix(200) * pgFix(200), pgFix::ExceptionOverflow);
        pgAssertThrow(pgFix(-200) * pgFix(200), pgFix::ExceptionOverflow);
        pgAssertThrow(200 * pgFix(200), pgFix::ExceptionOverflow);
        pgAssertThrow(-200 * pgFix(200), pgFix::ExceptionOverflow);
        pgAssertThrow(200.0f * pgFix(200), pgFix::ExceptionOverflow);
        pgAssertThrow(-200.0f * pgFix(200), pgFix::ExceptionOverflow);
        pgAssertThrow(0.001f * pgFix(0.001f), pgFix::ExceptionUnderflow);

        pgFix fix3 = 200;
        pgAssertThrow(fix3 *= 200, pgFix::ExceptionOverflow);

        pgFix fix4 = -200;
        pgAssertThrow(fix4 *= 200, pgFix::ExceptionOverflow);
    }

    /*
        pgFix operator/(pgFix fix) const
        PG_API friend pgFix operator/(s32 n, pgFix fix)
        PG_API friend pgFix operator/(r32 r, pgFix fix)
        void operator/=(pgFix fix)
    */
    {
        pgFix fix1 = 22.2f;
        pgFix fix2 = 0.5f;

        pgAssert(isEqual((fix1 / fix2).toR32(), 44.4f));
        pgAssert(222 / fix2 == 444);
        pgAssert(isEqual((22.2f / fix2).toR32(), 44.4f));

        fix1 /= fix2;
        pgAssert(isEqual(fix1.toR32(), 44.4f));

        pgAssertThrow(pgFix(400) / pgFix(0.01f), pgFix::ExceptionOverflow);
        pgAssertThrow(pgFix(-400) / pgFix(0.01f), pgFix::ExceptionOverflow);
        pgAssertThrow(400 / pgFix(0.01f), pgFix::ExceptionOverflow);
        pgAssertThrow(-400 / pgFix(0.01f), pgFix::ExceptionOverflow);
        pgAssertThrow(400.0f / pgFix(0.01f), pgFix::ExceptionOverflow);
        pgAssertThrow(-400.0f / pgFix(0.01f), pgFix::ExceptionOverflow);
        pgAssertThrow(0.001f / pgFix(1000), pgFix::ExceptionUnderflow);

        pgFix fix3 = 400;
        pgAssertThrow(fix3 /= 0.01f, pgFix::ExceptionOverflow);

        pgFix fix4 = -400;
        pgAssertThrow(fix4 /= 0.01f, pgFix::ExceptionOverflow);
    }

    /*
        s32 getValue() const
        static pgFix fromValue(s32 value)
    */
    {
        pgFix fix = 123;

        pgAssert(fix.getValue() == 123 << 16);

        pgAssert(pgFix::fromValue(fix.getValue()) == fix);
    }
}
