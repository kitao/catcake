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
