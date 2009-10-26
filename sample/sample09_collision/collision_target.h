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


#ifndef COLLISION_TARGET_H_
#define COLLISION_TARGET_H_


class CollisionTarget
{
public:
    enum Target1Type
    {
        TARGET1_SPHERE, //
        TARGET1_BOX, //
        TARGET1_RAY, //
        TARGET1_NUM
    };

    enum Target2Type
    {
        TARGET2_SPHERE, //
        TARGET2_BOX, //
        TARGET2_TRIANGLE, //
        TARGET2_NUM
    };

    struct Target1
    {
        Target1Type type;
        ckCdt::Sph sph;
        ckCdt::Box box;
        ckCdt::Ray ray;
    };

    struct Target2
    {
        Target2Type type;
        ckCdt::Sph sph;
        ckCdt::Box box;
        ckCdt::Tri tri;
    };

    static bool checkTargetHit(const Target1& tgt1, const ckCdt::Ray& hit_ray);
    static bool checkTargetHit(const Target2& tgt2, const ckCdt::Ray& hit_ray);

    static void updateTarget(Target1* tgt1, const ckMat& world, const ckVec& size);
    static void updateTarget(Target2* tgt2, const ckMat& world, const ckVec& size);

    static void drawTarget(const Target1& tgt1, ckCol poly_col, ckCol line_col, ckCol aabb_col);
    static void drawTarget(const Target2& tgt2, ckCol poly_col, ckCol line_col, ckCol aabb_col);

    static bool collide(ckCdt::CdtInfo* cdt_info, const Target1& tgt1, const Target2& tgt2);
    static bool intersect(ckVec* pos, const Target1& tgt1, const Target2& tgt2);

    static void drawPos(const ckVec& pos, ckCol col);
    static void drawAxis(ckCol col);
};


#endif // !COLLISION_TARGET_H_
