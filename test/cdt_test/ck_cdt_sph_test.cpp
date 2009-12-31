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
