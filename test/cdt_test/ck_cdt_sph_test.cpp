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


void ckCdtSphTest()
{
    /*
        Sph()
        const AABB& getAABB() const
        const ckVec& getPos() const
        void setPos(const ckVec& pos)
        r32 getRadius() const
        void setRadius(r32 radius)
    */
    {
        ckCdt::Sph sph;
        ckAssert(isEqual(sph.getPos(), ckVec::ZERO));
        ckAssert(isEqual(sph.getRadius(), 0.0f));

        ckCdt::AABB aabb = sph.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec::ZERO) && isEqual(aabb.getMax(), ckVec::ZERO));

        sph.setPos(ckVec(1.0f, 2.0f, 3.0f));
        ckAssert(isEqual(sph.getPos(), ckVec(1.0f, 2.0f, 3.0f)));

        aabb = sph.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec(1.0f, 2.0f, 3.0f)) && isEqual(aabb.getMax(), ckVec(1.0f, 2.0f, 3.0f)));

        sph.setRadius(123.0f);
        ckAssert(isEqual(sph.getRadius(), 123.0f));

        aabb = sph.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec(-122.0f, -121.0f, -120.0f)) && isEqual(aabb.getMax(), ckVec(124.0f, 125.0f, 126.0f)));

        sph.setRadius(0.0f);
        ckAssert(isEqual(sph.getRadius(), 0.0f));

        aabb = sph.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec(1.0f, 2.0f, 3.0f)) && isEqual(aabb.getMax(), ckVec(1.0f, 2.0f, 3.0f)));

        ckAssertThrow(sph.setRadius(-0.1f), ckCdt::Sph::ExceptionInvalidArgument);
    }
}
