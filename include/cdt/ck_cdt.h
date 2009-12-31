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


/*!
    @ingroup ckCdt
    A collision detection class, which provides collision detection functions.
*/
class CK_API ckCdt
{
public:
    /*!
        An axis-aligned bounding box.
    */
    class CK_API AABB
    {
        friend class ckCdt;

    public:
        //! @cond
        ckDefineException(ExceptionInvalidArgument);
        //! @endcond

        /*!
            Returns the minimum coordinates of this bounding box.
            @return The minimum coordinates of this bounding box.
        */
        const ckVec& getMin() const;

        /*!
            Returns the maximum coordinates of this bounding box.
            @return The maximum coordinates of this bounding box.
        */
        const ckVec& getMax() const;

        /*!
            Sets the coordinates of the bounding box.
            @param[in] min The minimum coordinates of this bounding box.
            @param[in] max The maximum coordinates of this bounding box.
        */
        void setBound(const ckVec& min, const ckVec& max);

    private:
        ckVec m_min;
        ckVec m_max;
    };

    /*!
        A sphere collision.
    */
    class CK_API Sph
    {
        friend class ckCdt;

    public:
        //! @cond
        ckDefineException(ExceptionInvalidArgument);
        //! @endcond

        /*!
            Constructs a sphere collision.
        */
        Sph();

        /*!
            Returns the aabb of this sphere collision.
            @return The aabb of this sphere collision.
        */
        const AABB& getAABB() const;

        /*!
            Returns the center of thie sphere collision.
            @return The center of thie sphere collision.
        */
        const ckVec& getPos() const;

        /*!
            Sets the center of this sphere collision.
            @param[in] pos The center of a sphere collision.
        */
        void setPos(const ckVec& pos);

        /*!
            Returns the radius of this sphere collision.
            @return The radius of this sphere collision.
        */
        r32 getRadius() const;

        /*!
            Sets the radius of this sphere collision.
            @param[in] radius The radius of a sphere collision.
        */
        void setRadius(r32 radius);

    private:
        void updateAABB();

        AABB m_aabb;
        ckVec m_pos;
        r32 m_radius;
    };

    /*!
        A box collision.
    */
    class CK_API Box
    {
        friend class ckCdt;

    public:
        //! @cond
        ckDefineException(ExceptionInvalidArgument);
        //! @endcond

        /*!
            Constructs a box collision.
        */
        Box();

        /*!
            Returns the aabb of this box collision.
            @return The aabb of this box collision.
        */
        const AABB& getAABB() const;

        /*!
            Returns the world of this box collision.
            @return The world of this box collision.
        */
        const ckMat& getWorld() const;

        /*!
            Sets the world of this box collision.
            @param[in] world The world of this box collision.
        */
        void setWorld(const ckMat& world);

        /*!
            Returns the width of this box collision.
            @return The width of this box collision.
        */
        r32 getWidth() const;

        /*!
            Returns the height of this box collision.
            @return The height of this box collision.
        */
        r32 getHeight() const;

        /*!
            Returns the depth of this box collision.
            @return The depth of this box collision.
        */
        r32 getDepth() const;

        /*!
            Returns the half size of this box collision.
            @return The half size of this box collision.
        */
        const ckVec& getHalfSize() const;

        /*!
            Sets the size of this box collision.
            @param[in] width The width of a box collision.
            @param[in] height The height of a box collision.
            @param[in] depth The depth of a box collision.
        */
        void setSize(r32 width, r32 height, r32 depth);

    private:
        void updateAABB();

        AABB m_aabb;
        ckMat m_world;
        ckVec m_half_size;
    };

    /*!
        A triangle collision.
    */
    class CK_API Tri
    {
        friend class ckCdt;

    public:
        /*!
            Constructs a triangle collision.
        */
        Tri();

        /*!
            Returns the aabb of this triangle collision.
            @return The aabb of this triangle collision.
        */
        const AABB& getAABB() const;

        /*!
            Returns the first vertex of this triangle collision.
            @return The first vertex of this triangle collision.
        */
        const ckVec& getPos1() const;

        /*!
            Returns the second vertex of this triangle collision.
            @return The second vertex of this triangle collision.
        */
        const ckVec& getPos2() const;

        /*!
            Returns the third vertex of this triangle collision.
            @return The third vertex of this triangle collision.
        */
        const ckVec& getPos3() const;

        /*!
            Sets the vertices of this triangle collision.
            @param[in] pos1 The first vertex of a triangle collision.
            @param[in] pos2 The second vertex of a triangle collision.
            @param[in] pos3 The third vertex of a triangle collision.
        */
        void setPos(const ckVec& pos1, const ckVec& pos2, const ckVec& pos3);

    private:
        void updateAABB();

        AABB m_aabb;
        ckVec m_pos1;
        ckVec m_pos2;
        ckVec m_pos3;
    };

    /*!
        A ray collision.
    */
    class CK_API Ray
    {
        friend class ckCdt;

    public:
        /*!
            Constructs a ray collision.
        */
        Ray();

        /*!
            Returns the aabb of this ray collision.
            @return The aabb of this ray collision.
        */
        const AABB& getAABB() const;

        /*!
            Returns the start point of this ray collision.
            @return The start point of this ray collision.
        */
        const ckVec& getFrom() const;

        /*!
            Returns the end point of this ray collision.
            @return The end point of this ray collision.
        */
        const ckVec& getTo() const;

        /*!
            Sets the points of this ray collision.
            @param[in] from The start point of a ray collision.
            @param[in] to The end point of a ray collision.
        */
        void setPos(const ckVec& from, const ckVec& to);

    private:
        void updateAABB();

        AABB m_aabb;
        ckVec m_from;
        ckVec m_to;
    };

    /*!
        A result of collision detection.
    */
    struct CdtInfo
    {
        ckVec pos; //!< The position of collision.
        ckVec back_dir; //!< The direction of push back.
        r32 back_dist; //!< The distance of push back.
    };

    /*!
        Returns whether two aabb-s touch each other.
        @param[in] aabb1 An aabb.
        @param[in] aabb2 An aabb.
        @return Whether two aabb-s touch each other.
    */
    static bool checkTouch(const AABB& aabb1, const AABB& aabb2);

    /*!
        Calculates the result of collision.
        @param[out] cdt_info The result of collision.
        @param[in] sph1 A sphere collision.
        @param[in] sph2 A sphere collision.
        @return Whether two collision touch each other.
    */
    static bool collide(CdtInfo* cdt_info, const Sph& sph1, const Sph& sph2);

    /*!
        Calculates the result of collision.
        @param[out] cdt_info The result of collision.
        @param[in] sph A sphere collision.
        @param[in] box A box collision.
        @return Whether two collision touch each other.
    */
    static bool collide(CdtInfo* cdt_info, const Sph& sph, const Box& box);

    /*!
        Calculates the result of collision.
        @param[out] cdt_info The result of collision.
        @param[in] sph A sphere collision.
        @param[in] tri A triangle collision.
        @return Whether two collision touch each other.
    */
    static bool collide(CdtInfo* cdt_info, const Sph& sph, const Tri& tri);

    /*!
        Calculates the result of collision.
        @param[out] cdt_info The result of collision.
        @param[in] box1 A box collision.
        @param[in] box2 A box collision.
        @return Whether two collision touch each other.
    */
    static bool collide(CdtInfo* cdt_info, const Box& box1, const Box& box2);

    /*!
        Calculates the result of collision.
        @param[out] cdt_info The result of collision.
        @param[in] box A box collision.
        @param[in] sph A sphere collision.
        @return Whether two collision touch each other.
    */
    static bool collide(CdtInfo* cdt_info, const Box& box, const Sph& sph);

    /*!
        Calculates the result of collision.
        @param[out] cdt_info The result of collision.
        @param[in] box A box collision.
        @param[in] tri A triangle collision.
        @return Whether two collision touch each other.
    */
    static bool collide(CdtInfo* cdt_info, const Box& box, const Tri& tri);

    /*!
        Calculates the intersection.
        @param[out] pos The intersection.
        @param[in] ray A ray collision.
        @param[in] sph A sphere collision.
        @return Whether intersects.
    */
    static bool intersect(ckVec* pos, const Ray& ray, const Sph& sph);

    /*!
        Calculates the intersection.
        @param[out] pos The intersection.
        @param[in] ray A ray collision.
        @param[in] box A box collision.
        @return Whether intersects.
    */
    static bool intersect(ckVec* pos, const Ray& ray, const Box& box);

    /*!
        Calculates the intersection.
        @param[out] pos The intersection.
        @param[in] ray A ray collision.
        @param[in] tri A triangle collision.
        @return Whether intersects.
    */
    static bool intersect(ckVec* pos, const Ray& ray, const Tri& tri);

private:
    static bool intersectLocalBox(r32* min_dist, r32* max_dist, const ckVec& local_ray_pos, const ckVec& local_ray_dir, const ckVec& box_half_size);
    static bool intersectTri(r32* dist, const ckVec& ray_pos, const ckVec& ray_dir, const Tri& tri);
};
