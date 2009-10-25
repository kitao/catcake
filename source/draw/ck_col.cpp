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


#include "pg_draw_all.h"


const pgCol pgCol::ZERO = pgCol(0, 0, 0, 0);
const pgCol pgCol::FULL = pgCol(255, 255, 255, 255);


pgCol::pgCol() {}


pgCol::pgCol(const pgCol& col)
{
    r = col.r;
    g = col.g;
    b = col.b;
    a = col.a;
}


pgCol::pgCol(u8 r_, u8 g_, u8 b_, u8 a_)
{
    r = r_;
    g = g_;
    b = b_;
    a = a_;
}


void pgCol::set(u8 r_, u8 g_, u8 b_, u8 a_)
{
    r = r_;
    g = g_;
    b = b_;
    a = a_;
}


bool pgCol::operator==(pgCol col) const
{
    return (r == col.r && g == col.g && b == col.b && a == col.a);
}


bool pgCol::operator!=(pgCol col) const
{
    return (r != col.r || g != col.g || b != col.b || a != col.a);
}


pgCol pgCol::operator+(pgCol col) const
{
    return pgCol( //
        pgMath::min(r + col.r, 255), //
        pgMath::min(g + col.g, 255), //
        pgMath::min(b + col.b, 255), //
        pgMath::min(a + col.a, 255));
}


void pgCol::operator+=(pgCol col)
{
    *this = *this + col;
}


pgCol pgCol::operator-(pgCol col) const
{
    return pgCol( //
        pgMath::max(r - col.r, 0), //
        pgMath::max(g - col.g, 0), //
        pgMath::max(b - col.b, 0), //
        pgMath::max(a - col.a, 0));
}


void pgCol::operator-=(pgCol col)
{
    *this = *this - col;
}


pgCol pgCol::operator*(pgCol col) const
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
        return pgCol( //
            ((r + 1) * (col.r + 1) - 1) >> 8, //
            ((g + 1) * (col.g + 1) - 1) >> 8, //
            ((b + 1) * (col.b + 1) - 1) >> 8, //
            ((a + 1) * (col.a + 1) - 1) >> 8);
    }
}


void pgCol::operator*=(pgCol col)
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


pgCol pgCol::operator*(r32 s) const
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

    return pgCol(new_r, new_g, new_b, new_a);
}


pgCol operator*(r32 s, pgCol col)
{
    return col * s;
}


void pgCol::operator*=(r32 s)
{
    *this = *this * s;
}


pgCol pgCol::operator/(r32 s) const
{
    return *this * (1.0f / s);
}


void pgCol::operator/=(r32 s)
{
    *this = *this / s;
}


pgCol pgCol::interp(pgCol to, r32 ratio) const
{
    if (ratio < pgMath::EPSILON)
    {
        return *this;
    }
    else if (ratio > 1.0f - pgMath::EPSILON)
    {
        return to;
    }
    else
    {
        return *this * (1.0f - ratio) + to * ratio;
    }
}
