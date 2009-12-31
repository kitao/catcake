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


static void assertEqual(const ckQuat& quat1, const ckQuat& quat2)
{
    ckAssert(isEqual(quat1.x, quat2.x) && isEqual(quat1.y, quat2.y) && isEqual(quat1.z, quat2.z) && isEqual(quat1.w, quat2.w));
}


void ckQuatTest()
{
    /*
        ckQuat()
        ckQuat(r32 x_, r32 y_, r32 z_, r32 w_)
        void set(r32 x_, r32 y_, r32 z_, r32 w_)
    */
    {
        ckQuat quat1;

        quat1.set(1.0f, 2.0f, 3.0f, 4.0f);
        assertEqual(quat1, ckQuat(1.0f, 2.0f, 3.0f, 4.0f));

        ckQuat quat2 = quat1;
        assertEqual(quat2, quat1);

        ckQuat quat3(1.0f, 2.0f, 3.0f, 4.0f);
        assertEqual(quat3, quat1);
    }

    /*
        ckQuat slerp(const ckQuat& to, r32 ratio) const
        ckMat toMat(const ckVec& trans) const
        static ckQuat fromMat(const ckMat& mat)
    */
    {
        for (s32 i = -360; i <= 360; i += 10)
        {
            r32 deg = static_cast<r32>(i) / 2.1f;
            r32 ratio = static_cast<r32>((i / 10 + 36) % 11) / 10.0f;

            ckMat mat1 = ckMat::UNIT.rotateX_s32(i).rotateY_s32(i * 2).rotateZ_s32(i * 3).translate(0.1f * i, 0.2f * i, 0.3f * i);
            ckMat mat2 = mat1.rotateX_r32(deg);
            ckMat mat3 = mat1.rotateX_r32(deg * ratio);

            ckQuat quat1 = ckQuat::fromMat(mat1);
            ckQuat quat2 = ckQuat::fromMat(mat2);
            ckQuat quat3 = ckQuat::fromMat(mat3);

            ckAssert(isEqual(mat1, quat1.toMat(mat1.trans)));
            ckAssert(isEqual(mat2, quat2.toMat(mat2.trans)));
            ckAssert(isEqual(mat3, quat3.toMat(mat3.trans)));

            ckAssert(isEqual(quat1.slerp(quat2, ratio).toMat(ckVec::ZERO), quat3.toMat(ckVec::ZERO)));
        }
    }
}
