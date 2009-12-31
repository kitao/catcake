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
