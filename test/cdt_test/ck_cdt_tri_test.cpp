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


#include "test.h"


void pgCdtTriTest()
{
    /*
        Tri()
        const AABB& getAABB() const
        const pgVec& getPos1() const
        const pgVec& getPos2() const
        const pgVec& getPos3() const
        void setPos(const pgVec& pos1, const pgVec& pos2, const pgVec& pos3)
    */
    {
        pgCdt::Tri tri;
        pgAssert(isEqual(tri.getPos1(), pgVec::ZERO) && isEqual(tri.getPos2(), pgVec::ZERO) && isEqual(tri.getPos3(), pgVec::ZERO));

        pgCdt::AABB aabb = tri.getAABB();
        pgAssert(isEqual(aabb.getMin(), pgVec::ZERO) && isEqual(aabb.getMax(), pgVec::ZERO));

        tri.setPos(pgVec(1.0f, 2.0f, 3.0f), pgVec(-2.0f, -3.0f, -4.0f), pgVec(-1.0f, 20.0f, -3.0f));
        pgAssert(isEqual(tri.getPos1(), pgVec(1.0f, 2.0f, 3.0f)) && isEqual(tri.getPos2(), pgVec(-2.0f, -3.0f, -4.0f)) && isEqual(tri.getPos3(), pgVec(-1.0f, 20.0f, -3.0f)));

        aabb = tri.getAABB();
        pgAssert(isEqual(aabb.getMin(), pgVec(-2.0f, -3.0f, -4.0f)) && isEqual(aabb.getMax(), pgVec(1.0f, 20.0f, 3.0f)));

        tri.setPos(pgVec::ZERO, pgVec::ZERO, pgVec::ZERO);
        pgAssert(isEqual(tri.getPos1(), pgVec::ZERO) && isEqual(tri.getPos2(), pgVec::ZERO) && isEqual(tri.getPos3(), pgVec::ZERO));

        aabb = tri.getAABB();
        pgAssert(isEqual(aabb.getMin(), pgVec::ZERO) && isEqual(aabb.getMax(), pgVec::ZERO));
    }
}
