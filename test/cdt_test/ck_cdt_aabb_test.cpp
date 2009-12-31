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
