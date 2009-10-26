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
