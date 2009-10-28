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


#include "ck_draw_all.h"

#include "ck_private_macro.h"


ckLit* ckLit::getPrevN() const
{
    ckList<ckLit>::Item* prev = m_item.getPrevN();

    return prev ? prev->getSelf() : NULL;
}


ckLit* ckLit::getNextN() const
{
    ckList<ckLit>::Item* next = m_item.getNextN();

    return next ? next->getSelf() : NULL;
}


ckID ckLit::getID() const
{
    return m_id;
}


bool ckLit::isEnabled() const
{
    return m_is_enabled.getType();
}


void ckLit::setEnabled(bool is_enabled)
{
    m_is_enabled = is_enabled;
}


const ckVec& ckLit::getPos() const
{
    return m_pos;
}


void ckLit::setPos(const ckVec& pos)
{
    m_pos = pos;

    updateBound();
}


r32 ckLit::getInnerRadius() const
{
    return m_inner_rad;
}


r32 ckLit::getOuterRadius() const
{
    return m_outer_rad;
}


void ckLit::setRadius(r32 inner_rad, r32 outer_rad)
{
    if (inner_rad < 0.0f || outer_rad < 0.0 || inner_rad > outer_rad)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_inner_rad = inner_rad;
    m_outer_rad = outer_rad;

    m_sq_inner_rad = m_inner_rad * m_inner_rad;
    m_sq_outer_rad = m_outer_rad * m_outer_rad;

    updateBound();
}


ckCol ckLit::getColor() const
{
    return m_col;
}


void ckLit::setColor(ckCol col)
{
    m_col = col;
    m_col_int = m_col.r + m_col.g + m_col.b;
}


ckLit::ckLit(ckID lts_id)
{
    m_id = lts_id;

    m_item.init(this);

    setEnabled(true);
    setPos(ckVec::ZERO);
    setRadius(0.0f, 0.0f);
    setColor(ckCol(255, 255, 0));
}


CK_DEFINE_OPERATOR_EQUAL(ckLit)


void ckLit::updateBound()
{
    m_max_bound.set(m_pos.x + m_outer_rad, m_pos.y + m_outer_rad, m_pos.z + m_outer_rad);
    m_min_bound.set(m_pos.x - m_outer_rad, m_pos.y - m_outer_rad, m_pos.z - m_outer_rad);
}
