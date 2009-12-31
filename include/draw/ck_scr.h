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
    A screen, which has a rectangle area and an order for drawing.
*/
class CK_API ckScr
{
    friend class ckDraw;
    friend class ckDrawMgr;

public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    //! @endcond

    /*!
        The default focal length.
    */
    static const u32 DEFAULT_FOCUS_DIST = 1000;

    /*!
        The default distance of near clipping.
    */
    static const u32 DEFAULT_NEAR_CLIP_DIST = 10;

    /*!
        The default distance of far clipping.
    */
    static const u32 DEFAULT_FAR_CLIP_DIST = 100000;

    /*!
        The number of the guest screens.
    */
    static const u32 GUEST_SCREEN_NUM = 4;

    /*!
        Returns the previous screen. If the first screen doesn't exist, returns NULL.
    */
    ckScr* getPrevN() const;

    /*!
        Returns the next screen. If the next screen doesn't exit, returns NULL.
    */
    ckScr* getNextN() const;

    /*!
        Changes the order of this screen to the first.
    */
    void moveFirst();

    /*!
        Changes the order of this screen to the last.
    */
    void moveLast();

    /*!
        Changes the order of this screen to the previous of the specified screen.
        @param[in] id A screen ID.
    */
    void moveBefore(ckID id);

    /*!
        Changes the order of this screen to the next of the specified screen.
        @param[in] id A screen ID.
    */
    void moveAfter(ckID id);

    /*!
        Returns the first drawing object. If the firs drawing object doesn't exist, return. NULL.
    */
    ckDraw* getFirstDrawN();

    /*!
        Returns the last drawing object. If the last drawing object doesn't exist, returns NULL.
    */
    ckDraw* getLastDrawN();

    /*!
        Returns the ID of this screen.
        @return The ID of this screen.
    */
    ckID getID() const;

    /*!
        Returns the left position of this screen in the framebuffer.
        @return The left position of this screen in the framebuffer.
    */
    s16 getLeftInFramebuffer() const;

    /*!
        Returns the top position of this screen in the framebuffer.
        @return The top position of this screen in the framebuffer.
    */
    s16 getTopInFramebuffer() const;

    /*!
        Returns the width of this screen in the framebuffer.
        @return The width of this screen in the framebuffer.
    */
    s16 getWidthInFramebuffer() const;

    /*!
        Returns the height of this screen in the framebuffer.
        @return The height of this screen in the framebuffer.
    */
    s16 getHeightInFramebuffer() const;

    /*!
        Sets the size of this screen in the framebuffer.
        @param[in] left The left position of a screen.
        @param[in] top The top position of a screen.
        @param[in] width The width of a screen.
        @param[in] height The height of a screen.
    */
    void setAreaInFramebuffer(s16 left, s16 top, u16 width, u16 height);

    /*!
        Returns the width of the view.
        @return The width of the view.
    */
    r32 getViewWidth() const;

    /*!
        Returns the height of the view.
        @return The height of the view.
    */
    r32 getViewHeight() const;

    /*!
        Sets the size of the view.
        @param[in] width The width of the view.
        @param[in] height The height of the view.
    */
    void setViewSize(r32 width, r32 height);

    /*!
        Returns whether this screen is active.
        @return Whether this screen is active.
    */
    bool isActive() const;

    /*!
        Determines whether this screen is active.
        @param[in] is_active Whether this screen is active.
    */
    void setActive(bool is_active);

    /*!
        Returns whether clears color when this screen is updated.
        @return Whether clears color when this screen is updated.
    */
    bool isClearColor() const;

    /*!
        Returns whether clears depthbuffer when this screen is updated.
        @return Whether clears depthbuffer when this screen is updated.
    */
    bool isClearDepth() const;

    /*!
        Sets the clear modes.
        @param[in] is_clear_color Whether clears color.
        @param[in] is_clear_depth Whether clears depthbuffer.
    */
    void setClearMode(bool is_clear_color, bool is_clear_depth);

    /*!
        Returns the clear color.
        @return The clear color.
    */
    ckCol getClearColor() const;

    /*!
        Sets the clear color.
        @param[in] col A clear color.
    */
    void setClearColor(ckCol col);

    /*!
        Returns whether perspective is on.
        @return Whether perspective is on.
    */
    bool isPerspective() const;

    /*!
        Determines whether pserspetive is on.
        @param[in] is_perspective Whether pserspetive is on.
    */
    void setPerspective(bool is_perspective);

    /*!
        Returns the focal length.
        @return The focal length.
    */
    r32 getFocusDist() const;

    /*!
        Sets the focal length.
        @param[in] focus_dist A focal length.
    */
    void setFocusDist(r32 focus_dist);

    /*!
        Returns the distance of near clipping.
        @return The distance of near clipping.
    */
    r32 getNearClipDist() const;

    /*!
        Returns the distance of far clipping.
        @return The distance of far clipping.
    */
    r32 getFarClipDist() const;

    /*!
        Sets the distances of clipping.
        @param[in] near The distance of near clipping.
        @param[in] far The distance of far clipping.
    */
    void setClipDist(r32 near, r32 far);

    /*!
        Returns the reference to the view of this screen.
        @return The reference to the view of this screen.
    */
    ckMat& view();

    /*!
        Returns whether this screen has the texture.
        @return Whether this screen has the texture.
    */
    bool hasScreenTexture() const;

    /*!
        Returns the texture ID of this screen.
        @return The texture ID of this screen.
    */
    ckID getScreenTextureID() const;

    /*!
        Creates the texture for this screen.
        @param[in] format The format type of a texture.
    */
    void attachScreenTexture(ckTex::TexFormat format);

    /*!
        Deletes the texture of this screen.
    */
    void detachScreenTexture();

    /*!
        Updates the texture of this screen.
        @param[in] is_frame_skip_reset Whether disallow frame skip.
    */
    void updateScreenTexture(bool is_frame_skip_reset);

    /*!
        Returns the ID of the specified guest screen.
        @param[in] index The index of a guest screen.
    */
    ckID getGuestScreenID(u8 index) const;

    /*!
        Sets the ID of the specified guest screen.
        @param[in] index The index of a guest screen.
        @param[in] scr_id A screen ID.
    */
    void setGuestScreenID(u8 index, ckID scr_id);

    /*!
        Calculates the x-coordinate in the framebuffer from the specified x-coordinate in this screen.
        @param[in] x_in_screen A x-coordinate in this screen.
        @return The x-coordinate in the framebuffer.
    */
    r32 screenXToFramebufferX(r32 x_in_screen) const;

    /*!
        Calculates the y-coordinates in the framebuffer from the specified y-coordinate in this screen.
        @param[in] y_in_screen A y-coordinate in this screen.
        @return The y-coordinate in the framebuffer.
    */
    r32 screenYToFramebufferY(r32 y_in_screen) const;

    /*!
        Calculates the x-coordinates in this screen from the specified x-coordinate in the framebuffer.
        @param[in] x_in_framebuffer A x-coordinate in the framebuffer.
        @return The x-coordinate in this screen.
    */
    r32 framebufferXToScreenX(r32 x_in_framebuffer) const;

    /*!
        Calculates the y-coordinates in this screen from the specified y-coordinate in the framebuffer.
        @param[in] y_in_framebuffer A y-coordinate in the framebuffer.
        @return The y-coordinate in this screen.
    */
    r32 framebufferYToScreenY(r32 y_in_framebuffer) const;

    /*!
        Calculates the posision in screen from the specified world position.
        @param[in] pos_in_world A world position.
        @return The position in this screen.
    */
    ckVec worldToScreen(const ckVec& pos_in_world);

    /*!
        Calculates the posision in the framebuffer from the specified world position.
        @param[in] pos_in_world A world position.
        @return The position in the framebuffer.
    */
    ckVec worldToFramebuffer(const ckVec& pos_in_world);

    /*!
        Calculates the position in the specified plane from the position in this screen.
        @param[in] x_in_screen A x-coordinate in this screen.
        @param[in] y_in_screen A y-coordinate in this screen.
        @param[in] xy_plane A xy-plane.
        @return The position in the specified xy-plane.
    */
    ckVec screenToPlane(r32 x_in_screen, r32 y_in_screen, const ckMat& xy_plane) const;

    /*!
        Calculates the position in the specified plane from the position in the framebuffer.
        @param[in] x_in_framebuffer A x-coordinate in the framebuffer.
        @param[in] y_in_framebuffer A y-coordinate in the framebuffer.
        @param[in] xy_plane A xy-plane.
        @return The position in the specified xy-plane.
    */
    ckVec framebufferToPlane(r32 x_in_framebuffer, r32 y_in_framebuffer, const ckMat& xy_plane) const;

    /*!
        Returns whether the specified position in the world is in this screen.
        @param[in] pos_in_world A position in the world.
        @return Whether the specified position in the world is in this screen.
    */
    bool isInScreen(const ckVec& pos_in_world);

    /*!
        Returns whether clipping by bounding box is possible.
        @param[in] world The world matrix of a bounding box.
        @param[in] bound_max The maximum bound clip.
        @param[in] bound_min The minimum bound clip.
        @return Whether clipping by bounding box is possible.
    */
    bool canBoundClip(const ckMat& world, const ckVec& bound_max, const ckVec& bound_min);

    /*!
        Calculates the visible direction from the specified line.
        @param[in] pos1 A position of a line.
        @param[in] pos2 A position of a line.
        @return The visible direction.
    */
    ckVec calcVisibleVector(const ckVec& pos1, const ckVec& pos2);

private:
    enum ScrFlag
    {
        FLAG_ACTIVE, //
        FLAG_CLEAR_COLOR, //
        FLAG_CLEAR_DEPTH, //
        FLAG_PERSPECTIVE, //
        FLAG_COPY_SCREEN
    };

    ckScr(ckID scr_id);
    ~ckScr();
    void operator=(const ckScr&);

    void calcProjection();
    void setupProjection();
    ckVec worldToClip_noCalcProjection(const ckVec& pos_in_world) const;
    bool canBoundClip_noCalcProjection(const ckMat& world, const ckVec& bound_max, const ckVec& bound_min) const;
    void copyScreenTexture();

    ckFlag<u8, ScrFlag> m_flag;
    ckID m_id;
    s16 m_left_in_framebuffer;
    s16 m_top_in_framebuffer;
    u16 m_width_in_framebuffer;
    u16 m_height_in_framebuffer;
    r32 m_view_width;
    r32 m_view_height;
    ckCol m_clear_col;
    r32 m_focus_dist;
    r32 m_near_clip_dist;
    r32 m_far_clip_dist;
    ckMat m_view;
    ckDraw m_root_draw;
    ckTex* m_scr_tex;
    ckID m_guest_id[GUEST_SCREEN_NUM];
    r32 m_view_to_clip[16];
    r32 m_world_to_view[16];
    r32 m_world_to_clip[16];
};
