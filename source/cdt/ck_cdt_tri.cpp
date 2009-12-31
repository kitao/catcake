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


#include "ck_cdt_all.h"


ckCdt::Tri::Tri()
{
    m_pos1 = m_pos2 = m_pos3 = ckVec::ZERO;

    updateAABB();
}


const ckCdt::AABB& ckCdt::Tri::getAABB() const
{
    return m_aabb;
}


const ckVec& ckCdt::Tri::getPos1() const
{
    return m_pos1;
}


const ckVec& ckCdt::Tri::getPos2() const
{
    return m_pos2;
}


const ckVec& ckCdt::Tri::getPos3() const
{
    return m_pos3;
}


void ckCdt::Tri::setPos(const ckVec& pos1, const ckVec& pos2, const ckVec& pos3)
{
    m_pos1 = pos1;
    m_pos2 = pos2;
    m_pos3 = pos3;

    updateAABB();
}


void ckCdt::Tri::updateAABB()
{
#define MIN3(a, b, c) ((a < b && a < c) ? a : ((b < c) ? b : c))
#define MAX3(a, b, c) ((a > b && a > c) ? a : ((b > c) ? b : c))

    m_aabb.setBound( //
        ckVec(MIN3(m_pos1.x, m_pos2.x, m_pos3.x), MIN3(m_pos1.y, m_pos2.y, m_pos3.y), MIN3(m_pos1.z, m_pos2.z, m_pos3.z)), //
        ckVec(MAX3(m_pos1.x, m_pos2.x, m_pos3.x), MAX3(m_pos1.y, m_pos2.y, m_pos3.y), MAX3(m_pos1.z, m_pos2.z, m_pos3.z)));

#undef MIN3
#undef MAX3
}
