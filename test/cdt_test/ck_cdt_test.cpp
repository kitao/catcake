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


void ckCdtTest()
{
    /*
        static bool checkTouch(const AABB& aabb1, const AABB& aabb2)
    */
    {
        ckCdt::AABB aabb1, aabb2;

        ckVec half_size1(10.0f, 20.0f, 30.0f);
        ckVec pos1(11.0f, -22.0f, 33.0f);

        aabb1.setBound(pos1 - half_size1, pos1 + half_size1);

        s8 offset1[26][3] =
        {
            { /**/ 1, 0, 0 /**/ }, /**/ { /**/ -1, 0, 0 /**/ }, /**/ { /**/ 0, 1, 0 /**/ },
            { /**/ 0, -1, 0 /**/ }, /**/ { /**/ 0, 0, 1 /**/ }, /**/ { /**/ 0, 0, -1 /**/ },
            //
            { /**/ 1, 1, 0 /**/ }, /**/ { /**/ 1, -1, 0 /**/ }, /**/ { /**/ -1, 1, 0 /**/ }, /**/ { /**/ -1, -1, 0 /**/ },
            { /**/ 1, 0, 1, /**/ }, /**/ { /**/ 1, 0, -1 /**/ }, /**/ { /**/ -1, 0, 1 /**/ }, /**/ { /**/ -1, 0, -1 /**/ },
            { /**/ 0, 1, 1 /**/ }, /**/ { /**/ 0, 1, -1 /**/ }, /**/ { /**/ 0, -1, 1 /**/ }, /**/ { /**/ 0, -1, -1 /**/ },
            //
            { /**/ 1, 1, 1 /**/ }, /**/ { /**/ 1, 1, -1 /**/ }, /**/ { /**/ 1, -1, 1 /**/ }, /**/ { /**/ 1, -1, -1 /**/ },
            { /**/ -1, 1, 1 /**/ }, /**/ { /**/ -1, 1, -1 /**/ }, /**/ { /**/ -1, -1, 1 /**/ }, /**/ { /**/ -1, -1, -1 /**/ }
        };

        for (s32 i = 0; i < 26; i++)
        {
            for (s32 j = 0; j < 2; j++)
            {
                r32 offset = (j % 2 == 0) ? 0.0f : ckMath::EPSILON;

                ckVec half_size2(40.0f, 50.0f, 60.0f);
                ckVec pos2( //
                    pos1.x + offset1[i][0] * (half_size1.x + half_size2.x + offset), //
                    pos1.y + offset1[i][1] * (half_size1.y + half_size2.y + offset), //
                    pos1.z + offset1[i][2] * (half_size1.z + half_size2.z + offset));

                aabb2.setBound(pos2 - half_size2, pos2 + half_size2);

                if (j % 2 == 0)
                {
                    ckAssert(ckCdt::checkTouch(aabb1, aabb2));
                }
                else
                {
                    ckAssert(!ckCdt::checkTouch(aabb1, aabb2));
                }
            }
        }

        aabb2.setBound(pos1 - ckVec(1.0f, 1.0f, 1.0f), pos1 + ckVec(1.0f, 1.0f, 1.0f));
        ckAssert(ckCdt::checkTouch(aabb1, aabb2));

        aabb2.setBound(pos1 - ckVec(100.0f, 100.0f, 100.0f), pos1 + ckVec(100.0f, 100.0f, 100.0f));
        ckAssert(ckCdt::checkTouch(aabb1, aabb2));
    }

    /*
        static bool collide(CdtInfo* cdt_info, const Sph& sph1, const Sph& sph2)
        static bool collide(CdtInfo* cdt_info, const Sph& sph, const Box& box)
        static bool collide(CdtInfo* cdt_info, const Sph& sph, const Tri& tri)
        static bool collide(CdtInfo* cdt_info, const Box& box1, const Box& box2)
        static bool collide(CdtInfo* cdt_info, const Box& box, const Sph& sph)
        static bool collide(CdtInfo* cdt_info, const Box& box, const Tri& tri)
    */
    {
        ckCdt::Sph sph1, sph2;
        ckCdt::Box box1, box2;
        ckCdt::Tri tri;
        ckCdt::CdtInfo cdt_info;

        sph1.setPos(ckVec(10.0f, 20.0f, 30.0f));
        sph1.setRadius(50.0f);

        sph2.setPos(ckVec(40.0f, 50.0f, 60.0f));
        sph2.setRadius(100.0f);

        box1.setWorld(ckMat::UNIT);
        box1.setSize(100.0f, 200.0f, 300.0f);

        box2.setWorld(ckMat::UNIT.translate(-100.0f, -200.0f, -300.0f));
        box2.setSize(400.0f, 500.0f, 600.0f);

        tri.setPos(ckVec::ZERO, ckVec(50.0f, 60.0f, 70.0f), ckVec(-50.0f, 60.0f, -70.0f));

        ckAssert(ckCdt::collide(NULL, sph1, sph2) && ckCdt::collide(&cdt_info, sph1, sph2));
        ckAssert(ckCdt::collide(NULL, sph1, box1) && ckCdt::collide(&cdt_info, sph1, box1));
        ckAssert(ckCdt::collide(NULL, sph1, tri) && ckCdt::collide(&cdt_info, sph1, tri));
        ckAssert(ckCdt::collide(NULL, box1, box2) && ckCdt::collide(&cdt_info, box1, box2));
        ckAssert(ckCdt::collide(NULL, box1, sph1) && ckCdt::collide(&cdt_info, box1, sph1));
        ckAssert(ckCdt::collide(NULL, box1, tri) && ckCdt::collide(&cdt_info, box1, tri));
    }

    /*
        static bool intersect(ckVec* pos, const Ray& ray, const Sph& sph)
        static bool intersect(ckVec* pos, const Ray& ray, const Box& box
        static bool intersect(ckVec* pos, const Ray& ray, const Tri& tri)
    */
    {
        ckCdt::Ray ray;
        ckCdt::Sph sph;
        ckCdt::Box box;
        ckCdt::Tri tri;
        ckVec pos;

        ray.setPos(ckVec(100.0f, 200.0f, 300.0f), ckVec(-100.0f, -200.0f, -300.0f));

        sph.setPos(ckVec(10.0f, 20.0f, 30.0f));
        sph.setRadius(50.0f);

        box.setWorld(ckMat::UNIT.translate(-10.0f, -20.0f, -30.0f));
        box.setSize(100.0f, 150.0f, 200.0f);

        tri.setPos(ckVec(0.0f, 100.0f, 0.0f), ckVec(-50.0f, -100.0f, 0.0f), ckVec(50.0f, -100.0f, 0.0f));

        ckAssert(ckCdt::intersect(NULL, ray, sph) && ckCdt::intersect(&pos, ray, sph));
        ckAssert(ckCdt::intersect(NULL, ray, box) && ckCdt::intersect(&pos, ray, box));
        ckAssert(ckCdt::intersect(NULL, ray, tri) && ckCdt::intersect(&pos, ray, tri));
    }
}
