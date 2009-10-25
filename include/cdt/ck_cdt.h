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
    @ingroup pgCdt
    TODO
*/
class PG_API pgCdt
{
public:
    /*!
        TODO
    */
    class PG_API AABB
    {
        friend class pgCdt;

    public:
        pgDefineException(ExceptionInvalidArgument);

        const pgVec& getMin() const;
        const pgVec& getMax() const;
        void setBound(const pgVec& min, const pgVec& max);

    private:
        pgVec m_min;
        pgVec m_max;
    };

    /*!
        TODO
    */
    class PG_API Sph
    {
        friend class pgCdt;

    public:
        pgDefineException(ExceptionInvalidArgument);

        Sph();

        const AABB& getAABB() const;

        const pgVec& getPos() const;
        void setPos(const pgVec& pos);

        r32 getRadius() const;
        void setRadius(r32 radius);

    private:
        void updateAABB();

        AABB m_aabb;
        pgVec m_pos;
        r32 m_radius;
    };

    /*!
        TODO
    */
    class PG_API Box
    {
        friend class pgCdt;

    public:
        pgDefineException(ExceptionInvalidArgument);

        Box();

        const AABB& getAABB() const;

        const pgMat& getWorld() const;
        void setWorld(const pgMat& world);

        r32 getWidth() const;
        r32 getHeight() const;
        r32 getDepth() const;
        const pgVec& getHalfSize() const;
        void setSize(r32 width, r32 height, r32 depth);

    private:
        void updateAABB();

        AABB m_aabb;
        pgMat m_world;
        pgVec m_half_size;
    };

    /*!
        TODO
    */
    class PG_API Tri
    {
        friend class pgCdt;

    public:
        Tri();

        const AABB& getAABB() const;

        const pgVec& getPos1() const;
        const pgVec& getPos2() const;
        const pgVec& getPos3() const;
        void setPos(const pgVec& pos1, const pgVec& pos2, const pgVec& pos3);

    private:
        void updateAABB();

        AABB m_aabb;
        pgVec m_pos1;
        pgVec m_pos2;
        pgVec m_pos3;
    };

    /*!
        TODO
    */
    class PG_API Ray
    {
        friend class pgCdt;

    public:
        Ray();

        const AABB& getAABB() const;

        const pgVec& getFrom() const;
        const pgVec& getTo() const;
        void setPos(const pgVec& from, const pgVec& to);

    private:
        void updateAABB();

        AABB m_aabb;
        pgVec m_from;
        pgVec m_to;
    };

    /*!
        TODO
    */
    struct CdtInfo
    {
        pgVec pos;
        pgVec back_dir;
        r32 back_dist;
    };

    static bool checkTouch(const AABB& aabb1, const AABB& aabb2);

    static bool collide(CdtInfo* cdt_info, const Sph& sph1, const Sph& sph2);
    static bool collide(CdtInfo* cdt_info, const Sph& sph, const Box& box);
    static bool collide(CdtInfo* cdt_info, const Sph& sph, const Tri& tri);

    static bool collide(CdtInfo* cdt_info, const Box& box1, const Box& box2);
    static bool collide(CdtInfo* cdt_info, const Box& box, const Sph& sph);
    static bool collide(CdtInfo* cdt_info, const Box& box, const Tri& tri);

    static bool intersect(pgVec* pos, const Ray& ray, const Sph& sph);
    static bool intersect(pgVec* pos, const Ray& ray, const Box& box);
    static bool intersect(pgVec* pos, const Ray& ray, const Tri& tri);

private:
    static bool intersectLocalBox(r32* min_dist, r32* max_dist, const pgVec& local_ray_pos, const pgVec& local_ray_dir, const pgVec& box_half_size);
    static bool intersectTri(r32* dist, const pgVec& ray_pos, const pgVec& ray_dir, const Tri& tri);
};
