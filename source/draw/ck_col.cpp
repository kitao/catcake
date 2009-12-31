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


#include "ck_draw_all.h"


const ckCol ckCol::ZERO = ckCol(0, 0, 0, 0);
const ckCol ckCol::FULL = ckCol(255, 255, 255, 255);


ckCol::ckCol() {}


ckCol::ckCol(const ckCol& col)
{
    r = col.r;
    g = col.g;
    b = col.b;
    a = col.a;
}


ckCol::ckCol(u8 r_, u8 g_, u8 b_, u8 a_)
{
    r = r_;
    g = g_;
    b = b_;
    a = a_;
}


void ckCol::set(u8 r_, u8 g_, u8 b_, u8 a_)
{
    r = r_;
    g = g_;
    b = b_;
    a = a_;
}


bool ckCol::operator==(ckCol col) const
{
    return (r == col.r && g == col.g && b == col.b && a == col.a);
}


bool ckCol::operator!=(ckCol col) const
{
    return (r != col.r || g != col.g || b != col.b || a != col.a);
}


ckCol ckCol::operator+(ckCol col) const
{
    return ckCol( //
        ckMath::min(r + col.r, 255), //
        ckMath::min(g + col.g, 255), //
        ckMath::min(b + col.b, 255), //
        ckMath::min(a + col.a, 255));
}


void ckCol::operator+=(ckCol col)
{
    *this = *this + col;
}


ckCol ckCol::operator-(ckCol col) const
{
    return ckCol( //
        ckMath::max(r - col.r, 0), //
        ckMath::max(g - col.g, 0), //
        ckMath::max(b - col.b, 0), //
        ckMath::max(a - col.a, 0));
}


void ckCol::operator-=(ckCol col)
{
    *this = *this - col;
}


ckCol ckCol::operator*(ckCol col) const
{
    if (*this == FULL)
    {
        return col;
    }
    else if (col == FULL)
    {
        return *this;
    }
    else
    {
        return ckCol( //
            ((r + 1) * (col.r + 1) - 1) >> 8, //
            ((g + 1) * (col.g + 1) - 1) >> 8, //
            ((b + 1) * (col.b + 1) - 1) >> 8, //
            ((a + 1) * (col.a + 1) - 1) >> 8);
    }
}


void ckCol::operator*=(ckCol col)
{
    if (*this == FULL)
    {
        *this = col;
    }
    else if (col == FULL)
    {
        // do nothing
    }
    else
    {
        r = ((r + 1) * (col.r + 1) - 1) >> 8;
        g = ((g + 1) * (col.g + 1) - 1) >> 8;
        b = ((b + 1) * (col.b + 1) - 1) >> 8;
        a = ((a + 1) * (col.a + 1) - 1) >> 8;
    }
}


ckCol ckCol::operator*(r32 s) const
{
    s32 new_r = static_cast<s32>(r * s);
    s32 new_g = static_cast<s32>(g * s);
    s32 new_b = static_cast<s32>(b * s);
    s32 new_a = static_cast<s32>(a * s);

    if (new_r > 255)
    {
        new_r = 255;
    }
    else if (new_r < 0)
    {
        new_r = 0;
    }

    if (new_g > 255)
    {
        new_g = 255;
    }
    else if (new_g < 0)
    {
        new_g = 0;
    }

    if (new_b > 255)
    {
        new_b = 255;
    }
    else if (new_b < 0)
    {
        new_b = 0;
    }

    if (new_a > 255)
    {
        new_a = 255;
    }
    else if (new_a < 0)
    {
        new_a = 0;
    }

    return ckCol(new_r, new_g, new_b, new_a);
}


ckCol operator*(r32 s, ckCol col)
{
    return col * s;
}


void ckCol::operator*=(r32 s)
{
    *this = *this * s;
}


ckCol ckCol::operator/(r32 s) const
{
    return *this * (1.0f / s);
}


void ckCol::operator/=(r32 s)
{
    *this = *this / s;
}


ckCol ckCol::interp(ckCol to, r32 ratio) const
{
    if (ratio < ckMath::EPSILON)
    {
        return *this;
    }
    else if (ratio > 1.0f - ckMath::EPSILON)
    {
        return to;
    }
    else
    {
        return *this * (1.0f - ratio) + to * ratio;
    }
}
