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


#include "test.h"


void ckCdtBoxTest()
{
    /*
        const AABB& getAABB() const
        const ckMat& getWorld() const
        void setWorld(const ckMat& world)
        r32 getWidth() const
        r32 getHeight() const
        r32 getDepth() const
        const ckVec& getHalfSize() const
        void setSize(r32 width, r32 height, r32 depth)
    */
    {
        ckCdt::Box box;
        ckAssert(isEqual(box.getWorld(), ckMat::UNIT));
        ckAssert(isEqual(box.getWidth(), 0.0f) && isEqual(box.getHeight(), 0.0f) && isEqual(box.getDepth(), 0.0f));
        ckAssert(isEqual(box.getHalfSize(), ckVec::ZERO));

        ckCdt::AABB aabb = box.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec::ZERO) && isEqual(aabb.getMax(), ckVec::ZERO));

        ckMat mat = ckMat::UNIT.rotateX_s32(90).rotateY_s32(90).translate(1.0f, 2.0f, 3.0f);
        box.setWorld(mat);
        ckAssert(isEqual(box.getWorld(), mat));

        aabb = box.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec(3.0f, 1.0f, 2.0f)) && isEqual(aabb.getMax(), ckVec(3.0f, 1.0f, 2.0f)));

        box.setSize(10.0f, 20.0f, 30.0f);
        ckAssert(isEqual(box.getWidth(), 10.0f) && isEqual(box.getHeight(), 20.0f) && isEqual(box.getDepth(), 30.0f));
        ckAssert(isEqual(box.getHalfSize(), ckVec(5.0f, 10.0f, 15.0f)));

        aabb = box.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec(-12.0f, -4.0f, -8.0f)) && isEqual(aabb.getMax(), ckVec(18.0f, 6.0f, 12.0f)));

        box.setSize(0.0f, 0.0f, 0.0f);
        ckAssert(isEqual(box.getWidth(), 0.0f) && isEqual(box.getHeight(), 0.0f) && isEqual(box.getDepth(), 0.0f));
        ckAssert(isEqual(box.getHalfSize(), ckVec::ZERO));

        aabb = box.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec(3.0f, 1.0f, 2.0f)) && isEqual(aabb.getMax(), ckVec(3.0f, 1.0f, 2.0f)));

        box.setSize(10.0f, 20.0f, 30.0f);

        box.setWorld(ckMat::UNIT.rotateX_s32(45));
        aabb = box.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec(-5.0f, -17.6777f, -17.6777f)) && isEqual(aabb.getMax(), ckVec(5.0f, 17.6777f, 17.6777f)));

        box.setWorld(ckMat::UNIT.rotateY_s32(45));
        aabb = box.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec(-14.1421f, -10.0f, -14.1421f)) && isEqual(aabb.getMax(), ckVec(14.1421f, 10.0f, 14.1421f)));

        box.setWorld(ckMat::UNIT.rotateZ_s32(45));
        aabb = box.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec(-10.6067f, -10.6067f, -15.0f)) && isEqual(aabb.getMax(), ckVec(10.60670f, 10.6067f, 15.0f)));

        ckAssertThrow(box.setSize(-0.1f, 0.0f, 0.0f), ckCdt::Box::ExceptionInvalidArgument);
        ckAssertThrow(box.setSize(0.0f, -0.1f, 0.0f), ckCdt::Box::ExceptionInvalidArgument);
        ckAssertThrow(box.setSize(0.0f, 0.0f, -0.1f), ckCdt::Box::ExceptionInvalidArgument);
    }
}
