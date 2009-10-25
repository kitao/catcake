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


pgCdt::Tri::Tri()
{
    m_pos1 = m_pos2 = m_pos3 = pgVec::ZERO;

    updateAABB();
}


const pgCdt::AABB& pgCdt::Tri::getAABB() const
{
    return m_aabb;
}


const pgVec& pgCdt::Tri::getPos1() const
{
    return m_pos1;
}


const pgVec& pgCdt::Tri::getPos2() const
{
    return m_pos2;
}


const pgVec& pgCdt::Tri::getPos3() const
{
    return m_pos3;
}


void pgCdt::Tri::setPos(const pgVec& pos1, const pgVec& pos2, const pgVec& pos3)
{
    m_pos1 = pos1;
    m_pos2 = pos2;
    m_pos3 = pos3;

    updateAABB();
}


void pgCdt::Tri::updateAABB()
{
#define MIN3(a, b, c) ((a < b && a < c) ? a : ((b < c) ? b : c))
#define MAX3(a, b, c) ((a > b && a > c) ? a : ((b > c) ? b : c))

    m_aabb.setBound( //
        pgVec(MIN3(m_pos1.x, m_pos2.x, m_pos3.x), MIN3(m_pos1.y, m_pos2.y, m_pos3.y), MIN3(m_pos1.z, m_pos2.z, m_pos3.z)), //
        pgVec(MAX3(m_pos1.x, m_pos2.x, m_pos3.x), MAX3(m_pos1.y, m_pos2.y, m_pos3.y), MAX3(m_pos1.z, m_pos2.z, m_pos3.z)));

#undef MIN3
#undef MAX3
}
