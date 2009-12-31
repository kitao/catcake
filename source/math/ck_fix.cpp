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


#include "ck_math_all.h"


ckFix::ckFix() {}


ckFix::ckFix(s32 n)
{
    s64 value = static_cast<s64>(n) << FRACTION_BIT_NUM;
    checkOverflow(value);

    m_value = static_cast<s32>(value);
}


ckFix::ckFix(r32 r)
{
    s64 value = static_cast<s64>(r * (1 << FRACTION_BIT_NUM));
    checkOverflow(value);

    if (r != 0.0f && value == 0)
    {
        ckThrow(ExceptionUnderflow);
    }

    m_value = static_cast<s32>(value);
}


bool ckFix::operator==(ckFix fix) const
{
    return (m_value == fix.m_value);
}


bool ckFix::operator!=(ckFix fix) const
{
    return (m_value != fix.m_value);
}


bool ckFix::operator<(ckFix fix) const
{
    return (m_value < fix.m_value);
}


bool ckFix::operator>(ckFix fix) const
{
    return (m_value > fix.m_value);
}


bool ckFix::operator<=(ckFix fix) const
{
    return (m_value <= fix.m_value);
}


bool ckFix::operator>=(ckFix fix) const
{
    return (m_value >= fix.m_value);
}


ckFix ckFix::operator+(ckFix fix) const
{
    s64 value = static_cast<s64>(m_value) + static_cast<s64>(fix.m_value);
    checkOverflow(value);

    return fromValue(static_cast<s32>(value));
}


ckFix operator+(s32 n, ckFix fix)
{
    return ckFix(n) + fix;
}


ckFix operator+(r32 r, ckFix fix)
{
    return ckFix(r) + fix;
}


void ckFix::operator+=(ckFix fix)
{
    *this = *this + fix;
}


ckFix ckFix::operator-() const
{
    s64 value = -static_cast<s64>(m_value);
    checkOverflow(value);

    return fromValue(static_cast<s32>(value));
}


ckFix ckFix::operator-(ckFix fix) const
{
    s64 value = static_cast<s64>(m_value) - static_cast<s64>(fix.m_value);
    checkOverflow(value);

    return fromValue(static_cast<s32>(value));
}


ckFix operator-(s32 n, ckFix fix)
{
    return ckFix(n) - fix;
}


ckFix operator-(r32 r, ckFix fix)
{
    return ckFix(r) - fix;
}


void ckFix::operator-=(ckFix fix)
{
    *this = *this - fix;
}


ckFix ckFix::operator*(ckFix fix) const
{
    s64 value = (static_cast<s64>(m_value) * static_cast<s64>(fix.m_value)) >> FRACTION_BIT_NUM;
    checkOverflow(value);

    if (m_value != 0 && fix.m_value != 0 && value == 0)
    {
        ckThrow(ExceptionUnderflow);
    }

    return fromValue(static_cast<s32>(value));
}


ckFix operator*(s32 n, ckFix fix)
{
    return ckFix(n) * fix;
}


ckFix operator*(r32 r, ckFix fix)
{
    return ckFix(r) * fix;
}


void ckFix::operator*=(ckFix fix)
{
    *this = *this * fix;
}


ckFix ckFix::operator/(ckFix fix) const
{
    s64 value = (static_cast<s64>(m_value) << FRACTION_BIT_NUM) / static_cast<s64>(fix.m_value);
    checkOverflow(value);

    if (m_value != 0 && fix.m_value != 0 && value == 0)
    {
        ckThrow(ExceptionUnderflow);
    }

    return fromValue(static_cast<s32>(value));
}


ckFix operator/(s32 n, ckFix fix)
{
    return ckFix(n) / fix;
}


ckFix operator/(r32 r, ckFix fix)
{
    return ckFix(r) / fix;
}


void ckFix::operator/=(ckFix fix)
{
    *this = *this / fix;
}


s32 ckFix::toS32() const
{
    return m_value >> FRACTION_BIT_NUM;
}


r32 ckFix::toR32() const
{
    return static_cast<r32>(m_value) / (1 << FRACTION_BIT_NUM);
}


s32 ckFix::getValue() const
{
    return m_value;
}


ckFix ckFix::fromValue(s32 value)
{
    ckFix fix;

    fix.m_value = value;

    return fix;
}


void ckFix::checkOverflow(s64 value)
{
    const s64 max_value = (static_cast<s64>(1) << (BIT_NUM - 1)) - 1;

    if (value > max_value || value < -max_value)
    {
        ckThrow(ExceptionOverflow);
    }
}
