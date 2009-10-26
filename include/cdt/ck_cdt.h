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


/*!
    @ingroup ckCdt
    TODO
*/
class CK_API ckCdt
{
public:
    /*!
        TODO
    */
    class CK_API AABB
    {
        friend class ckCdt;

    public:
        ckDefineException(ExceptionInvalidArgument);

        const ckVec& getMin() const;
        const ckVec& getMax() const;
        void setBound(const ckVec& min, const ckVec& max);

    private:
        ckVec m_min;
        ckVec m_max;
    };

    /*!
        TODO
    */
    class CK_API Sph
    {
        friend class ckCdt;

    public:
        ckDefineException(ExceptionInvalidArgument);

        Sph();

        const AABB& getAABB() const;

        const ckVec& getPos() const;
        void setPos(const ckVec& pos);

        r32 getRadius() const;
        void setRadius(r32 radius);

    private:
        void updateAABB();

        AABB m_aabb;
        ckVec m_pos;
        r32 m_radius;
    };

    /*!
        TODO
    */
    class CK_API Box
    {
        friend class ckCdt;

    public:
        ckDefineException(ExceptionInvalidArgument);

        Box();

        const AABB& getAABB() const;

        const ckMat& getWorld() const;
        void setWorld(const ckMat& world);

        r32 getWidth() const;
        r32 getHeight() const;
        r32 getDepth() const;
        const ckVec& getHalfSize() const;
        void setSize(r32 width, r32 height, r32 depth);

    private:
        void updateAABB();

        AABB m_aabb;
        ckMat m_world;
        ckVec m_half_size;
    };

    /*!
        TODO
    */
    class CK_API Tri
    {
        friend class ckCdt;

    public:
        Tri();

        const AABB& getAABB() const;

        const ckVec& getPos1() const;
        const ckVec& getPos2() const;
        const ckVec& getPos3() const;
        void setPos(const ckVec& pos1, const ckVec& pos2, const ckVec& pos3);

    private:
        void updateAABB();

        AABB m_aabb;
        ckVec m_pos1;
        ckVec m_pos2;
        ckVec m_pos3;
    };

    /*!
        TODO
    */
    class CK_API Ray
    {
        friend class ckCdt;

    public:
        Ray();

        const AABB& getAABB() const;

        const ckVec& getFrom() const;
        const ckVec& getTo() const;
        void setPos(const ckVec& from, const ckVec& to);

    private:
        void updateAABB();

        AABB m_aabb;
        ckVec m_from;
        ckVec m_to;
    };

    /*!
        TODO
    */
    struct CdtInfo
    {
        ckVec pos;
        ckVec back_dir;
        r32 back_dist;
    };

    static bool checkTouch(const AABB& aabb1, const AABB& aabb2);

    static bool collide(CdtInfo* cdt_info, const Sph& sph1, const Sph& sph2);
    static bool collide(CdtInfo* cdt_info, const Sph& sph, const Box& box);
    static bool collide(CdtInfo* cdt_info, const Sph& sph, const Tri& tri);

    static bool collide(CdtInfo* cdt_info, const Box& box1, const Box& box2);
    static bool collide(CdtInfo* cdt_info, const Box& box, const Sph& sph);
    static bool collide(CdtInfo* cdt_info, const Box& box, const Tri& tri);

    static bool intersect(ckVec* pos, const Ray& ray, const Sph& sph);
    static bool intersect(ckVec* pos, const Ray& ray, const Box& box);
    static bool intersect(ckVec* pos, const Ray& ray, const Tri& tri);

private:
    static bool intersectLocalBox(r32* min_dist, r32* max_dist, const ckVec& local_ray_pos, const ckVec& local_ray_dir, const ckVec& box_half_size);
    static bool intersectTri(r32* dist, const ckVec& ray_pos, const ckVec& ray_dir, const Tri& tri);
};
