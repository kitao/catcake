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


void ckCdtRayTest()
{
    /*
        Ray()
        const AABB& getAABB() const
        const ckVec& getFrom() const
        const ckVec& getTo() const
        void setPos(const ckVec& from, const ckVec& to)
    */
    {
        ckCdt::Ray ray;
        ckAssert(isEqual(ray.getFrom(), ckVec::ZERO) && isEqual(ray.getTo(), ckVec::ZERO));

        ckCdt::AABB aabb = ray.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec::ZERO) && isEqual(aabb.getMax(), ckVec::ZERO));

        ray.setPos(ckVec(-1.0f, 2.0f, -3.0f), ckVec(4.0f, -5.0f, 6.0f));
        ckAssert(isEqual(ray.getFrom(), ckVec(-1.0f, 2.0f, -3.0f)) && isEqual(ray.getTo(), ckVec(4.0f, -5.0f, 6.0f)));

        aabb = ray.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec(-1.0f, -5.0f, -3.0f)) && isEqual(aabb.getMax(), ckVec(4.0f, 2.0f, 6.0f)));

        ray.setPos(ckVec::ZERO, ckVec::ZERO);
        ckAssert(isEqual(ray.getFrom(), ckVec::ZERO) && isEqual(ray.getTo(), ckVec::ZERO));

        aabb = ray.getAABB();
        ckAssert(isEqual(aabb.getMin(), ckVec::ZERO) && isEqual(aabb.getMax(), ckVec::ZERO));
    }
}
