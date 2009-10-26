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
