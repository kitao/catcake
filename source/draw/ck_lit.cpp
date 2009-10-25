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

#include "pg_private_macro.h"


pgLit* pgLit::getPrevN() const
{
    pgList<pgLit>::Item* prev = m_item.getPrevN();

    return prev ? prev->getSelf() : NULL;
}


pgLit* pgLit::getNextN() const
{
    pgList<pgLit>::Item* next = m_item.getNextN();

    return next ? next->getSelf() : NULL;
}


pgID pgLit::getID() const
{
    return m_id;
}


bool pgLit::isActive() const
{
    return m_is_active.getType();
}


void pgLit::setActive(bool is_active)
{
    m_is_active = is_active;
}


const pgVec& pgLit::getPos() const
{
    return m_pos;
}


void pgLit::setPos(const pgVec& pos)
{
    m_pos = pos;

    updateBound();
}


r32 pgLit::getInnerRadius() const
{
    return m_inner_rad;
}


r32 pgLit::getOuterRadius() const
{
    return m_outer_rad;
}


void pgLit::setRadius(r32 inner_rad, r32 outer_rad)
{
    if (inner_rad < 0.0f || outer_rad < 0.0 || inner_rad > outer_rad)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_inner_rad = inner_rad;
    m_outer_rad = outer_rad;

    m_sq_inner_rad = m_inner_rad * m_inner_rad;
    m_sq_outer_rad = m_outer_rad * m_outer_rad;

    updateBound();
}


pgCol pgLit::getColor() const
{
    return m_col;
}


void pgLit::setColor(pgCol col)
{
    m_col = col;
    m_col_int = m_col.r + m_col.g + m_col.b;
}


pgLit::pgLit(pgID lts_id)
{
    m_id = lts_id;

    m_item.init(this);

    setActive(true);
    setPos(pgVec::ZERO);
    setRadius(0.0f, 0.0f);
    setColor(pgCol(255, 255, 0));
}


PG_DEFINE_OPERATOR_EQUAL(pgLit)


void pgLit::updateBound()
{
    m_max_bound.set(m_pos.x + m_outer_rad, m_pos.y + m_outer_rad, m_pos.z + m_outer_rad);
    m_min_bound.set(m_pos.x - m_outer_rad, m_pos.y - m_outer_rad, m_pos.z - m_outer_rad);
}
