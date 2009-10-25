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


void pgColTest()
{
    /*
        static const pgCol ZERO
        static const pgCol FULL
    */
    {
        pgAssert(pgCol::ZERO.r == 0 && pgCol::ZERO.g == 0 && pgCol::ZERO.b == 0 && pgCol::ZERO.a == 0);
        pgAssert(pgCol::FULL.r == 255 && pgCol::FULL.g == 255 && pgCol::FULL.b == 255 && pgCol::FULL.a == 255);
    }

    /*
        pgCol()
        pgCol(const pgCol& col)
        pgCol(u8 r_, u8 g_, u8 b_, u8 a_ = 255)
        void set(u8 r_, u8 g_, u8 b_, u8 a_ = 255)
        bool operator==(pgCol col) const
        bool operator!=(pgCol col) const
    */
    {
        pgCol col1;

        col1.set(1, 2, 3, 4);
        pgAssert(col1.r == 1 && col1.g == 2 && col1.b == 3 && col1.a == 4);

        pgCol col2 = col1;
        pgAssert(col2.r == 1 && col2.g == 2 && col2.b == 3 && col2.a == 4);

        pgCol col3(5, 6, 7, 8);
        pgAssert(col3.r == 5 && col3.g == 6 && col3.b == 7 && col3.a == 8);

        pgAssert(col1 == col2);
        pgAssert(!(col1 == col3));

        pgAssert(col1 != col3);
        pgAssert(!(col1 != col2));
    }

    /*
        pgCol operator+(pgCol col) const
        void operator+=(pgCol col)
    */
    {
        pgCol col1(80, 90, 100, 110);
        pgCol col2(100, 110, 120, 130);

        pgCol col3 = col1 + col2;
        pgAssert(col3.r == 180 && col3.g == 200 && col3.b == 220 && col3.a == 240);

        pgCol col4 = col2 + col2 + col2;
        pgAssert(col4.r == 255 && col4.g == 255 && col4.b == 255 && col4.a == 255);

        col1 += col2;
        pgAssert(col1.r == 180 && col1.g == 200 && col1.b == 220 && col1.a == 240);

        col1 += col2;
        pgAssert(col1.r == 255 && col1.g == 255 && col1.b == 255 && col1.a == 255);
    }

    /*
        pgCol operator-(pgCol col) const
        void operator-=(pgCol col)
    */
    {
        pgCol col1(100, 110, 120, 130);
        pgCol col2(80, 90, 100, 110);

        pgCol col3 = col1 - col2;
        pgAssert(col3.r == 20 && col3.g == 20 && col3.b == 20 && col3.a == 20);

        pgCol col4 = col1 - col2 - col2;
        pgAssert(col4.r == 0 && col4.g == 0 && col4.b == 0 && col4.a == 0);

        col1 -= col2;
        pgAssert(col1.r == 20 && col1.g == 20 && col1.b == 20 && col1.a == 20);

        col1 -= col1;
        pgAssert(col1.r == 0 && col1.g == 0 && col1.b == 0 && col1.a == 0);
    }

    /*
        pgCol operator*(pgCol col) const
        void operator*=(pgCol col)
        pgCol operator*(r32 s) const
        friend pgCol operator*(r32 s, pgCol col)
        void operator*=(r32 s)
    */
    {
        pgCol col1(32, 64, 128, 255);
        pgCol col2(255, 128, 64, 32);
        pgCol col3(255, 255, 255, 255);

        pgCol col4 = col1 * col2;
        pgAssert(col4.r == 32 && col4.g == 32 && col4.b == 32 && col4.a == 32);

        pgCol col5 = col1 * col3;
        pgAssert(col5 == col1);

        pgCol col6 = col3 * col1;
        pgAssert(col6 == col1);

        col1 *= col2;
        pgAssert(col1.r == 32 && col1.g == 32 && col1.b == 32 && col1.a == 32);

        col1 = col2;
        col1 *= col3;
        pgAssert(col1 == col2);

        col3 *= col2;
        pgAssert(col3 == col2);

        pgCol col7 = col2 * 0.5f;
        pgAssert(col7.r == 127 && col7.g == 64 && col7.b == 32 && col7.a == 16);

        pgCol col8 = 0.5f * col2;
        pgAssert(col8.r == 127 && col8.g == 64 && col8.b == 32 && col8.a == 16);

        col2 *= 0.5f;
        pgAssert(col2.r == 127 && col2.g == 64 && col2.b == 32 && col2.a == 16);
    }

    /*
        pgCol operator/(r32 s) const
        void operator/=(r32 s)
    */
    {
        pgCol col1(32, 64, 128, 255);

        pgCol col2 = col1 / 2.0f;
        pgAssert(col2.r == 16 && col2.g == 32 && col2.b == 64 && col2.a == 127);

        pgCol col3 = col1 / 0.5f;
        pgAssert(col3.r == 64 && col3.g == 128 && col3.b == 255 && col3.a == 255);

        pgCol col4 = col1 / 255;
        pgAssert(col4.r == 0 && col4.g == 0 && col4.b == 0 && col4.a == 1);

        col1 /= 2.0f;
        pgAssert(col1.r == 16 && col1.g == 32 && col1.b == 64 && col1.a == 127);

        col1 /= 0.25f;
        pgAssert(col1.r == 64 && col1.g == 128 && col1.b == 255 && col1.a == 255);

        col1 /= 256.0f;
        pgAssert(col1.r == 0 && col1.g == 0 && col1.b == 0 && col1.a == 0);
    }

    /*
        pgCol interp(pgCol to, r32 ratio) const
    */
    {
        pgCol col1(16, 32, 64, 128);
        pgCol col2(32, 64, 128, 0);

        pgCol col3 = col1.interp(col2, 0.0f);
        pgAssert(col3.r == 16 && col3.g == 32 && col3.b == 64 && col3.a == 128);

        pgCol col4 = col1.interp(col2, 0.25f);
        pgAssert(col4.r == 20 && col4.g == 40 && col4.b == 80 && col4.a == 96);

        pgCol col5 = col1.interp(col2, 1.0f);
        pgAssert(col5.r == 32 && col5.g == 64 && col5.b == 128 && col5.a == 0);
    }
}
