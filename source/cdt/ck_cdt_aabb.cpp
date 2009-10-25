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


#include "pg_cdt_all.h"


const pgVec& pgCdt::AABB::getMin() const
{
    return m_min;
}


const pgVec& pgCdt::AABB::getMax() const
{
    return m_max;
}


void pgCdt::AABB::setBound(const pgVec& min, const pgVec& max)
{
    if (min.x > max.x || min.y > max.y || min.z > max.z)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_min = min;
    m_max = max;
}


bool pgCdt::checkTouch(const AABB& aabb1, const AABB& aabb2)
{
    if (aabb1.m_min.x <= aabb2.m_max.x && aabb1.m_min.y <= aabb2.m_max.y && aabb1.m_min.z <= aabb2.m_max.z && //
        aabb1.m_max.x >= aabb2.m_min.x && aabb1.m_max.y >= aabb2.m_min.y && aabb1.m_max.z >= aabb2.m_min.z)
    {
        return true;
    }
    else
    {
        return false;
    }
}
