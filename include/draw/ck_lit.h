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


class ckLts;


/*!
    @ingroup ckDraw
    A point light.
*/
class CK_API ckLit
{
    friend class ckLts;

public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    //! @endcond

    /*!
        Returns the previous point light. If the previous point light doesn't exist, returns NULL.
        @return The previous point light.
    */
    ckLit* getPrevN() const;

    /*!
        Returns the next point light. If the next point light doesn't exist, returns NULL.
        @return The next point light.
    */
    ckLit* getNextN() const;

    /*!
        Returns the ID of this point light.
        @return The ID of this point light.
    */
    ckID getID() const;

    /*!
        Returns whether this point light is active.
        @return Whether this point light is active.
    */
    bool isActive() const;

    /*!
        Determines whether this point light is active.
        @param[in] is_active Whether this point light is active.
    */
    void setActive(bool is_active);

    /*!
        Returns the position of this point light.
        @return The position of this point light.
    */
    const ckVec& getPos() const;

    /*!
        Sets the position of this point light.
        @param[in] pos A position.
    */
    void setPos(const ckVec& pos);

    /*!
        Returns the inner radius of this point light.
        @return The inner radius of this point light.
    */
    r32 getInnerRadius() const;

    /*!
        Returns the outer radius of this point light.
        @return The outer radius of this point light.
    */
    r32 getOuterRadius() const;

    /*!
        Sets the radii of this point light.
        @param[in] inner_rad The inner radius.
        @param[in] outer_rad The outer radius.
    */
    void setRadius(r32 inner_rad, r32 outer_rad);

    /*!
        Returns the color of this point light.
        @return The color of this point light.
    */
    ckCol getColor() const;

    /*!
        Sets the color of this point light.
        @param[in] col A color.
    */
    void setColor(ckCol col);

private:
    ckLit(ckID lts_id);
    void operator=(const ckLit&);

    void updateBound();

    ckList<ckLit>::Item m_item;
    ckID m_id;
    ckType<u8, bool> m_is_active;
    ckVec m_pos;
    r32 m_inner_rad;
    r32 m_outer_rad;
    ckCol m_col;
    u16 m_col_int;
    ckVec m_min_bound;
    ckVec m_max_bound;
    r32 m_sq_inner_rad;
    r32 m_sq_outer_rad;
};
