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
    @ingroup ckDraw
    TODO
*/
class CK_API ckScr
{
    friend class ckDraw;
    friend class ckDrawMgr;

public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);

    static const u32 DEFAULT_FOCUS_DIST = 1000;
    static const u32 DEFAULT_NEAR_CLIP_DIST = 10;
    static const u32 DEFAULT_FAR_CLIP_DIST = 100000;
    static const u32 GUEST_SCREEN_NUM = 4;

    ckScr* getPrevN() const;
    ckScr* getNextN() const;

    void moveFirst();
    void moveLast();
    void moveBefore(ckID id);
    void moveAfter(ckID id);

    ckDraw* getFirstDrawN();
    ckDraw* getLastDrawN();

    ckID getID() const;

    s16 getLeftInFramebuffer() const;
    s16 getTopInFramebuffer() const;
    s16 getWidthInFramebuffer() const;
    s16 getHeightInFramebuffer() const;
    void setAreaInFramebuffer(s16 left, s16 top, u16 width, u16 height);

    r32 getViewWidth() const;
    r32 getViewHeight() const;
    void setViewSize(r32 width, r32 height);

    bool isActive() const;
    void setActive(bool is_active);

    bool isClearColor() const;
    bool isClearDepth() const;
    void setClearMode(bool is_clear_color, bool is_clear_depth);

    ckCol getClearColor() const;
    void setClearColor(ckCol col);

    bool isPerspective() const;
    void setPerspective(bool is_perspective);

    r32 getFocusDist() const;
    void setFocusDist(r32 focus_dist);

    r32 getNearClipDist() const;
    r32 getFarClipDist() const;
    void setClipDist(r32 near, r32 far);

    ckMat& view();

    bool hasScreenTexture() const;
    ckID getScreenTextureID() const;
    void attachScreenTexture(ckTex::TexFormat format);
    void detachScreenTexture();
    void updateScreenTexture(bool is_frame_skip_reset);

    ckID getGuestScreenID(u8 index) const;
    void setGuestScreenID(u8 index, ckID scr_id);

    r32 screenXToFramebufferX(r32 x_in_screen) const;
    r32 screenYToFramebufferY(r32 y_in_screen) const;
    r32 framebufferXToScreenX(r32 x_in_framebuffer) const;
    r32 framebufferYToScreenY(r32 y_in_framebuffer) const;

    ckVec worldToScreen(const ckVec& pos_in_world);
    ckVec worldToFramebuffer(const ckVec& pos_in_world);
    ckVec screenToPlane(r32 x_in_screen, r32 y_in_screen, const ckMat& xy_plane) const;
    ckVec framebufferToPlane(r32 x_in_framebuffer, r32 y_in_framebuffer, const ckMat& xy_plane) const;

    bool isInScreen(const ckVec& pos_in_world);
    bool canBoundClip(const ckMat& world, const ckVec& bound_max, const ckVec& bound_min);

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
