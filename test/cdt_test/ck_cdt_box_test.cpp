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


void pgCdtBoxTest()
{
    /*
        const AABB& getAABB() const
        const pgMat& getWorld() const
        void setWorld(const pgMat& world)
        r32 getWidth() const
        r32 getHeight() const
        r32 getDepth() const
        const pgVec& getHalfSize() const
        void setSize(r32 width, r32 height, r32 depth)
    */
    {
        pgCdt::Box box;
        pgAssert(isEqual(box.getWorld(), pgMat::UNIT));
        pgAssert(isEqual(box.getWidth(), 0.0f) && isEqual(box.getHeight(), 0.0f) && isEqual(box.getDepth(), 0.0f));
        pgAssert(isEqual(box.getHalfSize(), pgVec::ZERO));

        pgCdt::AABB aabb = box.getAABB();
        pgAssert(isEqual(aabb.getMin(), pgVec::ZERO) && isEqual(aabb.getMax(), pgVec::ZERO));

        pgMat mat = pgMat::UNIT.rotateX_s32(90).rotateY_s32(90).translate(1.0f, 2.0f, 3.0f);
        box.setWorld(mat);
        pgAssert(isEqual(box.getWorld(), mat));

        aabb = box.getAABB();
        pgAssert(isEqual(aabb.getMin(), pgVec(3.0f, 1.0f, 2.0f)) && isEqual(aabb.getMax(), pgVec(3.0f, 1.0f, 2.0f)));

        box.setSize(10.0f, 20.0f, 30.0f);
        pgAssert(isEqual(box.getWidth(), 10.0f) && isEqual(box.getHeight(), 20.0f) && isEqual(box.getDepth(), 30.0f));
        pgAssert(isEqual(box.getHalfSize(), pgVec(5.0f, 10.0f, 15.0f)));

        aabb = box.getAABB();
        pgAssert(isEqual(aabb.getMin(), pgVec(-12.0f, -4.0f, -8.0f)) && isEqual(aabb.getMax(), pgVec(18.0f, 6.0f, 12.0f)));

        box.setSize(0.0f, 0.0f, 0.0f);
        pgAssert(isEqual(box.getWidth(), 0.0f) && isEqual(box.getHeight(), 0.0f) && isEqual(box.getDepth(), 0.0f));
        pgAssert(isEqual(box.getHalfSize(), pgVec::ZERO));

        aabb = box.getAABB();
        pgAssert(isEqual(aabb.getMin(), pgVec(3.0f, 1.0f, 2.0f)) && isEqual(aabb.getMax(), pgVec(3.0f, 1.0f, 2.0f)));

        box.setSize(10.0f, 20.0f, 30.0f);

        box.setWorld(pgMat::UNIT.rotateX_s32(45));
        aabb = box.getAABB();
        pgAssert(isEqual(aabb.getMin(), pgVec(-5.0f, -17.6777f, -17.6777f)) && isEqual(aabb.getMax(), pgVec(5.0f, 17.6777f, 17.6777f)));

        box.setWorld(pgMat::UNIT.rotateY_s32(45));
        aabb = box.getAABB();
        pgAssert(isEqual(aabb.getMin(), pgVec(-14.1421f, -10.0f, -14.1421f)) && isEqual(aabb.getMax(), pgVec(14.1421f, 10.0f, 14.1421f)));

        box.setWorld(pgMat::UNIT.rotateZ_s32(45));
        aabb = box.getAABB();
        pgAssert(isEqual(aabb.getMin(), pgVec(-10.6067f, -10.6067f, -15.0f)) && isEqual(aabb.getMax(), pgVec(10.60670f, 10.6067f, 15.0f)));

        pgAssertThrow(box.setSize(-0.1f, 0.0f, 0.0f), pgCdt::Box::ExceptionInvalidArgument);
        pgAssertThrow(box.setSize(0.0f, -0.1f, 0.0f), pgCdt::Box::ExceptionInvalidArgument);
        pgAssertThrow(box.setSize(0.0f, 0.0f, -0.1f), pgCdt::Box::ExceptionInvalidArgument);
    }
}
