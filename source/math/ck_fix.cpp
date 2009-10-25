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


#include "pg_math_all.h"


pgFix::pgFix() {}


pgFix::pgFix(s32 n)
{
    s64 value = static_cast<s64>(n) << FRACTION_BIT_NUM;
    checkOverflow(value);

    m_value = static_cast<s32>(value);
}


pgFix::pgFix(r32 r)
{
    s64 value = static_cast<s64>(r * (1 << FRACTION_BIT_NUM));
    checkOverflow(value);

    if (r != 0.0f && value == 0)
    {
        pgThrow(ExceptionUnderflow);
    }

    m_value = static_cast<s32>(value);
}


bool pgFix::operator==(pgFix fix) const
{
    return (m_value == fix.m_value);
}


bool pgFix::operator!=(pgFix fix) const
{
    return (m_value != fix.m_value);
}


bool pgFix::operator<(pgFix fix) const
{
    return (m_value < fix.m_value);
}


bool pgFix::operator>(pgFix fix) const
{
    return (m_value > fix.m_value);
}


bool pgFix::operator<=(pgFix fix) const
{
    return (m_value <= fix.m_value);
}


bool pgFix::operator>=(pgFix fix) const
{
    return (m_value >= fix.m_value);
}


pgFix pgFix::operator+(pgFix fix) const
{
    s64 value = static_cast<s64>(m_value) + static_cast<s64>(fix.m_value);
    checkOverflow(value);

    return fromValue(static_cast<s32>(value));
}


pgFix operator+(s32 n, pgFix fix)
{
    return pgFix(n) + fix;
}


pgFix operator+(r32 r, pgFix fix)
{
    return pgFix(r) + fix;
}


void pgFix::operator+=(pgFix fix)
{
    *this = *this + fix;
}


pgFix pgFix::operator-() const
{
    s64 value = -static_cast<s64>(m_value);
    checkOverflow(value);

    return fromValue(static_cast<s32>(value));
}


pgFix pgFix::operator-(pgFix fix) const
{
    s64 value = static_cast<s64>(m_value) - static_cast<s64>(fix.m_value);
    checkOverflow(value);

    return fromValue(static_cast<s32>(value));
}


pgFix operator-(s32 n, pgFix fix)
{
    return pgFix(n) - fix;
}


pgFix operator-(r32 r, pgFix fix)
{
    return pgFix(r) - fix;
}


void pgFix::operator-=(pgFix fix)
{
    *this = *this - fix;
}


pgFix pgFix::operator*(pgFix fix) const
{
    s64 value = (static_cast<s64>(m_value) * static_cast<s64>(fix.m_value)) >> FRACTION_BIT_NUM;
    checkOverflow(value);

    if (m_value != 0 && fix.m_value != 0 && value == 0)
    {
        pgThrow(ExceptionUnderflow);
    }

    return fromValue(static_cast<s32>(value));
}


pgFix operator*(s32 n, pgFix fix)
{
    return pgFix(n) * fix;
}


pgFix operator*(r32 r, pgFix fix)
{
    return pgFix(r) * fix;
}


void pgFix::operator*=(pgFix fix)
{
    *this = *this * fix;
}


pgFix pgFix::operator/(pgFix fix) const
{
    s64 value = (static_cast<s64>(m_value) << FRACTION_BIT_NUM) / static_cast<s64>(fix.m_value);
    checkOverflow(value);

    if (m_value != 0 && fix.m_value != 0 && value == 0)
    {
        pgThrow(ExceptionUnderflow);
    }

    return fromValue(static_cast<s32>(value));
}


pgFix operator/(s32 n, pgFix fix)
{
    return pgFix(n) / fix;
}


pgFix operator/(r32 r, pgFix fix)
{
    return pgFix(r) / fix;
}


void pgFix::operator/=(pgFix fix)
{
    *this = *this / fix;
}


s32 pgFix::toS32() const
{
    return m_value >> FRACTION_BIT_NUM;
}


r32 pgFix::toR32() const
{
    return static_cast<r32>(m_value) / (1 << FRACTION_BIT_NUM);
}


s32 pgFix::getValue() const
{
    return m_value;
}


pgFix pgFix::fromValue(s32 value)
{
    pgFix fix;

    fix.m_value = value;

    return fix;
}


void pgFix::checkOverflow(s64 value)
{
    const s64 max_value = (static_cast<s64>(1) << (BIT_NUM - 1)) - 1;

    if (value > max_value || value < -max_value)
    {
        pgThrow(ExceptionOverflow);
    }
}
