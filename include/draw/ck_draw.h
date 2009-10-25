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
    @ingroup pgDraw
    The base class of the drawable-object classes.
*/
class PG_API pgDraw
{
    friend class pgScr;
    friend class pgDrawMgr;

public:
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionInvalidCall);
    pgDefineException(ExceptionNotInitialized);

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
        BLEND_OFF, //!< TODO
        BLEND_HALF, //!< TODO
        BLEND_ADD, //!< TODO
        BLEND_DEST_ALPHA //!< TODO
    };

    /*!
        The flags which specify the drawing attributes.
    */
    enum DrawFlag
    {
        FLAG_BOUND_CLIP, //!< TODO
        FLAG_SORT, //!< TODO
        FLAG_WRITE_RGB, //!< TODO
        FLAG_WRITE_ALPHA, //!< TODO
        FLAG_WRITE_DEPTH, //!< TODO
        FLAG_BACKFACE_CULLING, //!< TODO
        FLAG_BILINEAR //!< TODO
    };

    /*!
        Destructs this pgDraw.
    */
    virtual ~pgDraw();

    /*!
        Returns whether this pgDraw belongs to a screen directly.
        @return Whether this pgDraw belongs to a screen directly.
    */
    bool hasScreen() const;

    /*!
        Returns the ID of the screen which this pgDraw belongs to directly.
        If this pgDraw belongs to no screen, an exception occurs.
        @return The ID of the screen which this pgDraw belongs to directly.
    */
    pgID getScreenID() const;

    /*!
        Makes this pgDraw belong to the specified screen.
        @param[in] The id of a screen.
    */
    void setScreenID(pgID scr_id);

    /*!
        Returns whether this pgDraw has a parent pgDraw.
        @return Whether this pgDraw has a parent pgDraw.
    */
    bool hasParent() const;

    /*!
        Returns the parent pgDraw of this pgDraw.
        If this pgDraw has no parent pgDraw, returns NULL.
        @return The parent pgDraw of this pgDraw.
    */
    pgDraw* getParentN() const;

    /*!
        Sets the parent pgDraw of this pgDraw.
        This pgDraw belongs to the specified pgDraw as the last child.
        @param[in] parent A parent pgDraw.
    */
    void setParent(pgDraw* parent);

    /*!
        Returns the previous pgDraw of this pgDraw.
        If this pgDraw has no previous pgDraw, returns NULL.
        This method is used to follow the whole tree as list structure.
        @return The previous pgDraw of this pgDraw.
    */
    pgDraw* getPrevAllN() const;

    /*!
        Returns the next pgDraw of this pgDraw.
        If this pgDraw has no next pgDraw, returns NULL.
        This method is used to follow the whole tree as list structure.
        @return The previous pgDraw of this pgDraw.
    */
    pgDraw* getNextAllN() const;

    /*!
        Returns the previous sibling pgDraw of this pgDraw.
        If this pgDraw has no previous sibling pgDraw, returns NULL.
        @return The previous sibling pgDraw of this pgDraw.
    */
    pgDraw* getPrevSiblingN() const;

    /*!
        Returns the next sibling pgDraw of this pgDraw.
        If this pgDraw has no next sibling pgDraw, returns NULL.
        @return The next sibling pgDraw of this pgDraw.
    */
    pgDraw* getNextSiblingN() const;

    /*!
        Returns the last descendant pgDraw of this pgDraw.
        If this pgDraw has no descendant, returns this pgDraw.
        @return The last descendant pgDraw of this pgDraw.
    */
    pgDraw* getLastDescendant() const;

    /*!
        Returns whether this pgDraw has a child pgDraw.
        @return Whether this pgDraw has a child pgDraw.
    */
    bool hasChild() const;

    /*!
        Returns the first child pgDraw of this pgDraw.
        If this pgDraw has no child pgDraw, returns NULL.
        @return The first child pgDraw of this pgDraw.
    */
    pgDraw* getFirstChildN() const;

    /*!
        Returns the last child pgDraw of this pgDraw.
        If this pgDraw has no child pgDraw, returns NULL.
        @return The last child pgDraw of this pgDraw.
    */
    pgDraw* getLastChildN() const;

    /*!
        Changes the order of this pgDraw in the siblings to the first.
        This pgDraw must belong to a screen or have a parent.
    */
    void moveFirst();

    /*!
        Changes the order of this pgDraw in the siblings to the last.
        This pgDraw must belong to a screen or have a parent.
    */
    void moveLast();

    /*!
        Changes the order of this pgDraw to the previous of the specified pgDraw.
        The specified pgDraw must belong to a screen or have a parent.
        @param[in] draw A pgDraw.
    */
    void moveBefore(pgDraw* draw);

    /*!
        Changes the order of this pgDraw to the next of the specified pgDraw.
        The specified pgDraw must belong to a screen or have a parent.
        @param[in] draw A pgDraw.
    */
    void moveAfter(pgDraw* draw);

    /*!
        Returns the draw-type of this pgDraw.
        @return The draw-type of this pgDraw.
    */
    DrawType getType() const;

    /*!
        Returns whether this pgDraw is visible.
        @return Whether this pgDraw is visible.
    */
    bool isVisible() const;

    /*!
        Determines whether this pgDraw is visible.
        @param[in] is_visible Whether this pgDraw is visible.
    */
    void setVisible(bool is_visible);

    /*!
        Returns the filter color of this pgDraw.
        @return The filter color of this pgDraw.
    */
    pgCol getColor() const;

    /*!
        Sets the filter color of this pgDraw.
        @param[in] col A filter color.
    */
    void setColor(pgCol col);

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
    */
    void setBlendMode(BlendMode blend_mode);

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
        Copies the draw flags from the specified pgDraw.
        @param[in] src A source pgDraw.
    */
    void copyDrawFlag(const pgDraw* src);

    /*!
        Sets the type of depth test, the type of blend mode, and the draw flags to the standard blend off mode.
        This method equals to following code:
        setDepthTest(DEPTH_TEST_GEQUAL);
        setBlendMode(BLEND_OFF);

        m_draw_flag.clear();
        m_draw_flag.setOn(FLAG_WRITE_RGB);
        m_draw_flag.setOn(FLAG_WRITE_DEPTH);
        m_draw_flag.setOn(FLAG_BILINEAR);
    */
    void setPreset_defaultBlendOff();

    /*!
        Sets the type of depth test, the type of blend mode, and the draw flags to the standard blend half mode.
        This method equals to following code:
        setDepthTest(DEPTH_TEST_GEQUAL);
        setBlendMode(BLEND_HALF);

        m_draw_flag.clear();
        m_draw_flag.setOn(FLAG_SORT);
        m_draw_flag.setOn(FLAG_WRITE_RGB);
        m_draw_flag.setOn(FLAG_BILINEAR);
    */
    void setPreset_defaultBlendHalf();

    /*!
        Sets the type of depth test, the type of blend mode, and the draw flags to the standard blend add mode.
        This method equals to following code:
        setDepthTest(DEPTH_TEST_GEQUAL);
        setBlendMode(BLEND_ADD);

        m_draw_flag.clear();
        m_draw_flag.setOn(FLAG_SORT);
        m_draw_flag.setOn(FLAG_WRITE_RGB);
        m_draw_flag.setOn(FLAG_BILINEAR);
    */
    void setPreset_defaultBlendAdd();

    /*!
        Returns the minimum clip bound of this pgDraw.
        @return The minimum clip bound of this pgDraw.
    */
    const pgVec& getClipBoundMin() const;

    /*!
        Returns the maximum clip bound of this pgDraw.
        @return The maximum clip bound of this pgDraw.
    */
    const pgVec& getClipBoundMax() const;

    /*!
        Sets the clip bound of this pgDraw.
        @param[in] bound_min The minimum clip bound.
        @param[in] bound_max The maximum clip bound.
    */
    void setClipBound(const pgVec& bound_min, const pgVec& bound_max);

    /*!
        Returns the center position for sorting pgDraws.
        @return The center position for sorting pgDraws.
    */
    const pgVec& getSortCenter() const;

    /*!
        Sets the center position for sorting pgDraws.
        @param[in] The center position for sorting pgDraws.
    */
    void setSortCenter(const pgVec& sort_center);

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
        Returns the texture ID which this pgDraw uses.
        If no texture is used, returns pgID::ZERO.
        @return The texture ID which this pgDraw uses.
    */
    pgID getTextureID() const;

    /*!
        Sets the specified texture ID to this pgDraw.
        If pgID::ZERO is specified, uses no texture.
        @param tex_id A texture ID.
    */
    void setTextureID(pgID tex_id);

    /*!
        Returns the reference to the local matrix.
        @return The reference to the local matrix.
    */
    pgMat& local();

    /*!
        Calculates and returns the world matrix at present.
        @return The world matrix at present.
    */
    pgMat calcWorld() const;

    /*!
        Calculates and returns the final drawing color at present.
        @return The final drawing color at present.
    */
    pgCol calcFinalColor() const;

protected:
    enum PrivateFlag
    {
        FLAG_INITIALIZED, //
        FLAG_VISIBLE
    };

    pgDraw();

    void setupWorld();
    void setupFinalColor();
    void setupSortValue(const pgMat& view);
    void setupDrawState();

    virtual void render(const pgMat& view);

    pgTree<pgDraw> m_tree;
    pgType<u8, DrawType> m_type;
    pgType<u8, DepthTest> m_depth_test;
    pgType<u8, BlendMode> m_blend_mode;
    pgFlag<u8, PrivateFlag> m_private_flag;
    pgFlag<u16, DrawFlag> m_draw_flag;
    pgID m_scr_id;
    pgTex* m_tex;
    pgMat m_local;
    pgMat m_world;
    pgCol m_local_col;
    pgCol m_final_col;
    pgVec m_bound_min;
    pgVec m_bound_max;
    pgVec m_sort_center;
    r32 m_sort_offset;
    r32 m_sort_value;
    pgDraw* m_next_sort;

private:
    pgDraw(const pgDraw&);
    void operator=(const pgDraw&);
};
