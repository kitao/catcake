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
    @ingroup ckDraw
    A drawing element, the base class of the drawing elements.
*/
class CK_API ckDraw
{
    friend class ckScr;
    friend class ckDrawMgr;

public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionNotInitialized);
    //! @endcond

    /*!
        The types of the drawable-objects.
    */
    enum DrawType
    {
        TYPE_NODE, //!< The node drawable-object.
        TYPE_PRIM, //!< The primitive drawable-object.
        TYPE_SPRT //!< The sprite drawable-object.
    };

    /*!
        The types of the depth tests.
    */
    enum DepthTest
    {
        DEPTH_TEST_ALWAYS, //!< Always passes.
        DEPTH_TEST_LESS, //!< Passes if the incoming depth value is less than the stored value.
        DEPTH_TEST_GREATER, //!< Passes if the incoming depth value is greater than the stored value.
        DEPTH_TEST_LEQUAL, //!< Passes if the incoming depth value is less than or equal to the stored value.
        DEPTH_TEST_GEQUAL //!< Passes if the incoming depth value is greater than or equal to the stored value.
    };

    /*!
        The types of the alpha-blending mode.
    */
    enum BlendMode
    {
        BLEND_OFF, //!< (Rs, Gs, Bs, As)
        BLEND_HALF, //!< (Rs, Gs, Bs, As) * As + (Rd, Gd, Bd, Ad) * (1 - As)
        BLEND_ADD, //!< (Rs, Gs, Bs, As) + (Rd, Gd, Bd, Ad)
        BLEND_DEST_ALPHA //!< (Rs, Gs, Bs, As) * Ad + (Rd, Gd, Bd, Ad) * (1 - Ad)
    };

    /*!
        The flags which specify the drawing attributes.
    */
    enum DrawFlag
    {
        FLAG_BOUND_CLIP, //!< Enables bound-clipping.
        FLAG_SORT, //!< Sorts the drawing object by the distance from the viewpoint.
        FLAG_WRITE_RGB, //!< Writes RGB components to the framebuffer.
        FLAG_WRITE_ALPHA, //!< Writes an alpha components to the framebuffer.
        FLAG_WRITE_DEPTH, //!< Writes a depth value to the depth-buffer.
        FLAG_BACKFACE_CULLING, //!< Enables backface-culling.
        FLAG_BILINEAR //!< Enables bilinear filter.
    };

    /*!
        Destructs this ckDraw.
    */
    virtual ~ckDraw();

    /*!
        Returns whether this ckDraw belongs to a screen directly.
        @return Whether this ckDraw belongs to a screen directly.
    */
    bool hasScreen() const;

    /*!
        Returns the ID of the screen which this ckDraw belongs to directly.
        If this ckDraw belongs to no screen, an exception occurs.
        @return The ID of the screen which this ckDraw belongs to directly.
    */
    ckID getScreenID() const;

    /*!
        Makes this ckDraw belong to the specified screen.
        @param[in] scr_id The id of a screen.
    */
    void setScreenID(ckID scr_id);

    /*!
        Returns whether this ckDraw has a parent ckDraw.
        @return Whether this ckDraw has a parent ckDraw.
    */
    bool hasParent() const;

    /*!
        Returns the parent ckDraw of this ckDraw.
        If this ckDraw has no parent ckDraw, returns NULL.
        @return The parent ckDraw of this ckDraw.
    */
    ckDraw* getParentN() const;

    /*!
        Sets the parent ckDraw of this ckDraw.
        This ckDraw belongs to the specified ckDraw as the last child.
        @param[in] parent A parent ckDraw.
    */
    void setParent(ckDraw* parent);

    /*!
        Returns the previous ckDraw of this ckDraw.
        If this ckDraw has no previous ckDraw, returns NULL.
        This method is used to follow the whole tree as list structure.
        @return The previous ckDraw of this ckDraw.
    */
    ckDraw* getPrevAllN() const;

    /*!
        Returns the next ckDraw of this ckDraw.
        If this ckDraw has no next ckDraw, returns NULL.
        This method is used to follow the whole tree as list structure.
        @return The previous ckDraw of this ckDraw.
    */
    ckDraw* getNextAllN() const;

    /*!
        Returns the previous sibling ckDraw of this ckDraw.
        If this ckDraw has no previous sibling ckDraw, returns NULL.
        @return The previous sibling ckDraw of this ckDraw.
    */
    ckDraw* getPrevSiblingN() const;

    /*!
        Returns the next sibling ckDraw of this ckDraw.
        If this ckDraw has no next sibling ckDraw, returns NULL.
        @return The next sibling ckDraw of this ckDraw.
    */
    ckDraw* getNextSiblingN() const;

    /*!
        Returns the last descendant ckDraw of this ckDraw.
        If this ckDraw has no descendant, returns this ckDraw.
        @return The last descendant ckDraw of this ckDraw.
    */
    ckDraw* getLastDescendant() const;

    /*!
        Returns whether this ckDraw has a child ckDraw.
        @return Whether this ckDraw has a child ckDraw.
    */
    bool hasChild() const;

    /*!
        Returns the first child ckDraw of this ckDraw.
        If this ckDraw has no child ckDraw, returns NULL.
        @return The first child ckDraw of this ckDraw.
    */
    ckDraw* getFirstChildN() const;

    /*!
        Returns the last child ckDraw of this ckDraw.
        If this ckDraw has no child ckDraw, returns NULL.
        @return The last child ckDraw of this ckDraw.
    */
    ckDraw* getLastChildN() const;

    /*!
        Changes the order of this ckDraw in the siblings to the first.
        This ckDraw must belong to a screen or have a parent.
    */
    void moveFirst();

    /*!
        Changes the order of this ckDraw in the siblings to the last.
        This ckDraw must belong to a screen or have a parent.
    */
    void moveLast();

    /*!
        Changes the order of this ckDraw to the previous of the specified ckDraw.
        The specified ckDraw must belong to a screen or have a parent.
        @param[in] draw A ckDraw.
    */
    void moveBefore(ckDraw* draw);

    /*!
        Changes the order of this ckDraw to the next of the specified ckDraw.
        The specified ckDraw must belong to a screen or have a parent.
        @param[in] draw A ckDraw.
    */
    void moveAfter(ckDraw* draw);

    /*!
        Returns the draw-type of this ckDraw.
        @return The draw-type of this ckDraw.
    */
    DrawType getType() const;

    /*!
        Returns whether this ckDraw is visible.
        @return Whether this ckDraw is visible.
    */
    bool isVisible() const;

    /*!
        Determines whether this ckDraw is visible.
        @param[in] is_visible Whether this ckDraw is visible.
    */
    void setVisible(bool is_visible);

    /*!
        Returns the filter color of this ckDraw.
        @return The filter color of this ckDraw.
    */
    ckCol getColor() const;

    /*!
        Sets the filter color of this ckDraw.
        @param[in] col A filter color.
    */
    void setColor(ckCol col);

    /*!
        Returns the type of depth test.
        @return The type of depth test.
    */
    DepthTest getDepthTest() const;

    /*!
        Sets the type of depth test.
        @param[in] depth_test The type of depth test.
    */
    void setDepthTest(DepthTest depth_test);

    /*!
        Returns the type of blend mode.
        @return The type of blend mode.
    */
    BlendMode getBlendMode() const;

    /*!
        Sets the type of blend mode.
        @param[in] blend_mode The type of blend mode.
        @param[in] is_preset_setting If true, sets other draw flags and the type of depth test automatically.
    */
    void setBlendMode(BlendMode blend_mode, bool is_preset_setting);

    /*!
        Returns whether the specified draw flag is on.
        @param[in] draw_flag A draw flag.
        @return Whether the specified draw flag is on.
    */
    bool isDrawFlag(DrawFlag draw_flag) const;

    /*!
        Sets the specified draw flag to the specified value.
        @param[in] draw_flag A draw flag.
        @param[in] is_on If true, the flag is set on. Otherwise, the flag is set off.
    */
    void setDrawFlag(DrawFlag draw_flag, bool is_on);

    /*!
        Sets the all draw flags to off.
    */
    void clearDrawFlag();

    /*!
        Copies the draw flags from the specified ckDraw.
        @param[in] src A source ckDraw.
    */
    void copyDrawFlag(const ckDraw* src);

    /*!
        Returns the minimum clip bound of this ckDraw.
        @return The minimum clip bound of this ckDraw.
    */
    const ckVec& getClipBoundMin() const;

    /*!
        Returns the maximum clip bound of this ckDraw.
        @return The maximum clip bound of this ckDraw.
    */
    const ckVec& getClipBoundMax() const;

    /*!
        Sets the clip bound of this ckDraw.
        @param[in] bound_min The minimum clip bound.
        @param[in] bound_max The maximum clip bound.
    */
    void setClipBound(const ckVec& bound_min, const ckVec& bound_max);

    /*!
        Returns the center position for sorting ckDraws.
        @return The center position for sorting ckDraws.
    */
    const ckVec& getSortCenter() const;

    /*!
        Sets the center position for sorting ckDraws.
        @param[in] sort_center The center position for sorting ckDraws.
    */
    void setSortCenter(const ckVec& sort_center);

    /*!
        Returns the value of sort offset.
        @return The value of sort offset.
    */
    r32 getSortOffset() const;

    /*!
        Sets the specified value of sort offset.
        @param sort_offset The value of sort offset.
    */
    void setSortOffset(r32 sort_offset);

    /*!
        Returns the texture ID which this ckDraw uses.
        If no texture is used, returns ckID::ZERO.
        @return The texture ID which this ckDraw uses.
    */
    ckID getTextureID() const;

    /*!
        Sets the specified texture ID to this ckDraw.
        If ckID::ZERO is specified, uses no texture.
        @param tex_id A texture ID.
    */
    void setTextureID(ckID tex_id);

    /*!
        Returns the reference to the local matrix.
        @return The reference to the local matrix.
    */
    ckMat& local();

    /*!
        Calculates and returns the world matrix at present.
        @return The world matrix at present.
    */
    ckMat calcWorld() const;

    /*!
        Calculates and returns the final drawing color at present.
        @return The final drawing color at present.
    */
    ckCol calcFinalColor() const;

protected:
    //! @cond
    enum PrivateFlag
    {
        FLAG_INITIALIZED, //
        FLAG_VISIBLE
    };

    ckDraw();

    void setupWorld();
    void setupFinalColor();
    void setupSortValue(const ckMat& view);
    void setupDrawState();

    virtual void render(const ckMat& view);

    ckTree<ckDraw> m_tree;
    ckType<u8, DrawType> m_type;
    ckType<u8, DepthTest> m_depth_test;
    ckType<u8, BlendMode> m_blend_mode;
    ckFlag<u8, PrivateFlag> m_private_flag;
    ckFlag<u16, DrawFlag> m_draw_flag;
    ckID m_scr_id;
    ckTex* m_tex;
    ckMat m_local;
    ckMat m_world;
    ckCol m_local_col;
    ckCol m_final_col;
    ckVec m_bound_min;
    ckVec m_bound_max;
    ckVec m_sort_center;
    r32 m_sort_offset;
    r32 m_sort_value;
    ckDraw* m_next_sort;
    //! @endcond

private:
    ckDraw(const ckDraw&);
    void operator=(const ckDraw&);
};
