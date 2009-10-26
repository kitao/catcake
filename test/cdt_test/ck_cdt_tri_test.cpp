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


void ckCdtTriTest()
{
    /*
        Tri()
        const AABB& getAABB() const
        const ckVec& getPos1() const
        const ckVec& getPos2() const
        const ckVec& getPos3() const
        void setPos(const ckVec& pos1, const ckVec& pos2, const ckVec& pos3)
    */
    {
        ckCdt::Tri tri;
        ckAssert(isEqual(tri.getPos1(), ckVec::ZERO) && isEqual(tri.getPos2(), ckVec::ZERO) && isEqual(tri.getPos3(), ckVec::ZERO));

        ckCdt::AABB aabb = tri.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec::ZERO) && isEqual(aabb.getMax(), ckVec::ZERO));

        tri.setPos(ckVec(1.0f, 2.0f, 3.0f), ckVec(-2.0f, -3.0f, -4.0f), ckVec(-1.0f, 20.0f, -3.0f));
        ckAssert(isEqual(tri.getPos1(), ckVec(1.0f, 2.0f, 3.0f)) && isEqual(tri.getPos2(), ckVec(-2.0f, -3.0f, -4.0f)) && isEqual(tri.getPos3(), ckVec(-1.0f, 20.0f, -3.0f)));

        aabb = tri.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec(-2.0f, -3.0f, -4.0f)) && isEqual(aabb.getMax(), ckVec(1.0f, 20.0f, 3.0f)));

        tri.setPos(ckVec::ZERO, ckVec::ZERO, ckVec::ZERO);
        ckAssert(isEqual(tri.getPos1(), ckVec::ZERO) && isEqual(tri.getPos2(), ckVec::ZERO) && isEqual(tri.getPos3(), ckVec::ZERO));

        aabb = tri.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec::ZERO) && isEqual(aabb.getMax(), ckVec::ZERO));
    }
}
