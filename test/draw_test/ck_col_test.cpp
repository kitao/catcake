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


void ckColTest()
{
    /*
        static const ckCol ZERO
        static const ckCol FULL
    */
    {
        ckAssert(ckCol::ZERO.r == 0 && ckCol::ZERO.g == 0 && ckCol::ZERO.b == 0 && ckCol::ZERO.a == 0);
        ckAssert(ckCol::FULL.r == 255 && ckCol::FULL.g == 255 && ckCol::FULL.b == 255 && ckCol::FULL.a == 255);
    }

    /*
        ckCol()
        ckCol(const ckCol& col)
        ckCol(u8 r_, u8 g_, u8 b_, u8 a_ = 255)
        void set(u8 r_, u8 g_, u8 b_, u8 a_ = 255)
        bool operator==(ckCol col) const
        bool operator!=(ckCol col) const
    */
    {
        ckCol col1;

        col1.set(1, 2, 3, 4);
        ckAssert(col1.r == 1 && col1.g == 2 && col1.b == 3 && col1.a == 4);

        ckCol col2 = col1;
        ckAssert(col2.r == 1 && col2.g == 2 && col2.b == 3 && col2.a == 4);

        ckCol col3(5, 6, 7, 8);
        ckAssert(col3.r == 5 && col3.g == 6 && col3.b == 7 && col3.a == 8);

        ckAssert(col1 == col2);
        ckAssert(!(col1 == col3));

        ckAssert(col1 != col3);
        ckAssert(!(col1 != col2));
    }

    /*
        ckCol operator+(ckCol col) const
        void operator+=(ckCol col)
    */
    {
        ckCol col1(80, 90, 100, 110);
        ckCol col2(100, 110, 120, 130);

        ckCol col3 = col1 + col2;
        ckAssert(col3.r == 180 && col3.g == 200 && col3.b == 220 && col3.a == 240);

        ckCol col4 = col2 + col2 + col2;
        ckAssert(col4.r == 255 && col4.g == 255 && col4.b == 255 && col4.a == 255);

        col1 += col2;
        ckAssert(col1.r == 180 && col1.g == 200 && col1.b == 220 && col1.a == 240);

        col1 += col2;
        ckAssert(col1.r == 255 && col1.g == 255 && col1.b == 255 && col1.a == 255);
    }

    /*
        ckCol operator-(ckCol col) const
        void operator-=(ckCol col)
    */
    {
        ckCol col1(100, 110, 120, 130);
        ckCol col2(80, 90, 100, 110);

        ckCol col3 = col1 - col2;
        ckAssert(col3.r == 20 && col3.g == 20 && col3.b == 20 && col3.a == 20);

        ckCol col4 = col1 - col2 - col2;
        ckAssert(col4.r == 0 && col4.g == 0 && col4.b == 0 && col4.a == 0);

        col1 -= col2;
        ckAssert(col1.r == 20 && col1.g == 20 && col1.b == 20 && col1.a == 20);

        col1 -= col1;
        ckAssert(col1.r == 0 && col1.g == 0 && col1.b == 0 && col1.a == 0);
    }

    /*
        ckCol operator*(ckCol col) const
        void operator*=(ckCol col)
        ckCol operator*(r32 s) const
        friend ckCol operator*(r32 s, ckCol col)
        void operator*=(r32 s)
    */
    {
        ckCol col1(32, 64, 128, 255);
        ckCol col2(255, 128, 64, 32);
        ckCol col3(255, 255, 255, 255);

        ckCol col4 = col1 * col2;
        ckAssert(col4.r == 32 && col4.g == 32 && col4.b == 32 && col4.a == 32);

        ckCol col5 = col1 * col3;
        ckAssert(col5 == col1);

        ckCol col6 = col3 * col1;
        ckAssert(col6 == col1);

        col1 *= col2;
        ckAssert(col1.r == 32 && col1.g == 32 && col1.b == 32 && col1.a == 32);

        col1 = col2;
        col1 *= col3;
        ckAssert(col1 == col2);

        col3 *= col2;
        ckAssert(col3 == col2);

        ckCol col7 = col2 * 0.5f;
        ckAssert(col7.r == 127 && col7.g == 64 && col7.b == 32 && col7.a == 16);

        ckCol col8 = 0.5f * col2;
        ckAssert(col8.r == 127 && col8.g == 64 && col8.b == 32 && col8.a == 16);

        col2 *= 0.5f;
        ckAssert(col2.r == 127 && col2.g == 64 && col2.b == 32 && col2.a == 16);
    }

    /*
        ckCol operator/(r32 s) const
        void operator/=(r32 s)
    */
    {
        ckCol col1(32, 64, 128, 255);

        ckCol col2 = col1 / 2.0f;
        ckAssert(col2.r == 16 && col2.g == 32 && col2.b == 64 && col2.a == 127);

        ckCol col3 = col1 / 0.5f;
        ckAssert(col3.r == 64 && col3.g == 128 && col3.b == 255 && col3.a == 255);

        ckCol col4 = col1 / 255;
        ckAssert(col4.r == 0 && col4.g == 0 && col4.b == 0 && col4.a == 1);

        col1 /= 2.0f;
        ckAssert(col1.r == 16 && col1.g == 32 && col1.b == 64 && col1.a == 127);

        col1 /= 0.25f;
        ckAssert(col1.r == 64 && col1.g == 128 && col1.b == 255 && col1.a == 255);

        col1 /= 256.0f;
        ckAssert(col1.r == 0 && col1.g == 0 && col1.b == 0 && col1.a == 0);
    }

    /*
        ckCol interp(ckCol to, r32 ratio) const
    */
    {
        ckCol col1(16, 32, 64, 128);
        ckCol col2(32, 64, 128, 0);

        ckCol col3 = col1.interp(col2, 0.0f);
        ckAssert(col3.r == 16 && col3.g == 32 && col3.b == 64 && col3.a == 128);

        ckCol col4 = col1.interp(col2, 0.25f);
        ckAssert(col4.r == 20 && col4.g == 40 && col4.b == 80 && col4.a == 96);

        ckCol col5 = col1.interp(col2, 1.0f);
        ckAssert(col5.r == 32 && col5.g == 64 && col5.b == 128 && col5.a == 0);
    }
}
