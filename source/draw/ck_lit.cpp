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


bool ckLit::isActive() const
{
    return m_is_active.getType();
}


void ckLit::setActive(bool is_active)
{
    m_is_active = is_active;
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

    setActive(true);
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
