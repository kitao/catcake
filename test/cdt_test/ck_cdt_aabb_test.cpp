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


void ckCdtAABBTest()
{
    /*
        const ckVec& getMin() const
        const ckVec& getMax() const
        void setBound(const ckVec& min, const ckVec& max)
    */
    {
        ckCdt::AABB aabb;

        aabb.setBound(ckVec(-1.0f, -2.0f, -3.0f), ckVec(4.0f, 5.0f, 6.0f));
        ckAssert(isEqual(aabb.getMin(), ckVec(-1.0f, -2.0f, -3.0f)) && isEqual(aabb.getMax(), ckVec(4.0f, 5.0f, 6.0f)));

        aabb.setBound(ckVec::ZERO, ckVec::ZERO);
        ckAssert(isEqual(aabb.getMin(), ckVec::ZERO) && isEqual(aabb.getMax(), ckVec::ZERO));

        ckAssertThrow(aabb.setBound(ckVec(0.0f, 0.0f, 0.0f), ckVec(-0.1f, 0.0f, 0.0f)), ckCdt::AABB::ExceptionInvalidArgument);
        ckAssertThrow(aabb.setBound(ckVec(0.0f, 0.0f, 0.0f), ckVec(0.0f, -0.1f, 0.0f)), ckCdt::AABB::ExceptionInvalidArgument);
        ckAssertThrow(aabb.setBound(ckVec(0.0f, 0.0f, 0.0f), ckVec(0.0f, 0.0f, -0.1f)), ckCdt::AABB::ExceptionInvalidArgument);
    }
}
