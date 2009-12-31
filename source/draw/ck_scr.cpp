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


#include "ck_draw_all.h"

#include "ck_sys_all.h"
#include "ck_task_all.h"
#include "ck_low_level_api.h"
#include "ck_private_macro.h"


ckScr* ckScr::getPrevN() const
{
    ckDrawMgr* ins = ckDrawMgr::instance();
    const ckID* id = ins->m_scr_map.getPrevKeyN(m_id);

    return id ? *ins->m_scr_map.get(*id) : NULL;
}


ckScr* ckScr::getNextN() const
{
    ckDrawMgr* ins = ckDrawMgr::instance();
    const ckID* id = ins->m_scr_map.getNextKeyN(m_id);

    return id ? *ins->m_scr_map.get(*id) : NULL;
}


void ckScr::moveFirst()
{
    ckDrawMgr::instance()->m_scr_map.moveFirst(m_id);
}


void ckScr::moveLast()
{
    ckDrawMgr::instance()->m_scr_map.moveLast(m_id);
}


void ckScr::moveBefore(ckID id)
{
    ckDrawMgr* ins = ckDrawMgr::instance();

    if (id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckTry
    {
        ins->m_scr_map.get(id);
    }
    ckCatch(ckMap<ckID, ckScr*>::ExceptionNotFound)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ins->m_scr_map.moveBefore(m_id, id);
}


void ckScr::moveAfter(ckID id)
{
    ckDrawMgr* ins = ckDrawMgr::instance();

    if (id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckTry
    {
        ins->m_scr_map.get(id);
    }
    ckCatch(ckMap<ckID, ckScr*>::ExceptionNotFound)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ins->m_scr_map.moveAfter(m_id, id);
}


ckDraw* ckScr::getFirstDrawN()
{
    return m_root_draw.getFirstChildN();
}


ckDraw* ckScr::getLastDrawN()
{
    ckDraw* desc = m_root_draw.getLastDescendant();

    return (desc != &m_root_draw) ? desc : NULL;
}


ckID ckScr::getID() const
{
    return m_id;
}


s16 ckScr::getLeftInFramebuffer() const
{
    return m_left_in_framebuffer;
}


s16 ckScr::getTopInFramebuffer() const
{
    return m_top_in_framebuffer;
}


s16 ckScr::getWidthInFramebuffer() const
{
    return m_width_in_framebuffer;
}


s16 ckScr::getHeightInFramebuffer() const
{
    return m_height_in_framebuffer;
}


void ckScr::setAreaInFramebuffer(s16 left, s16 top, u16 width, u16 height)
{
    if (width == 0 || height == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_left_in_framebuffer = left;
    m_top_in_framebuffer = top;
    m_width_in_framebuffer = width;
    m_height_in_framebuffer = height;
}


r32 ckScr::getViewWidth() const
{
    return m_view_width;
}


r32 ckScr::getViewHeight() const
{
    return m_view_height;
}


void ckScr::setViewSize(r32 width, r32 height)
{
    m_view_width = width;
    m_view_height = height;
}


bool ckScr::isActive() const
{
    return m_flag.isOn(FLAG_ACTIVE);
}


void ckScr::setActive(bool is_active)
{
    m_flag.set(FLAG_ACTIVE, is_active);
}


bool ckScr::isClearColor() const
{
    return m_flag.isOn(FLAG_CLEAR_COLOR);
}


bool ckScr::isClearDepth() const
{
    return m_flag.isOn(FLAG_CLEAR_DEPTH);
}


void ckScr::setClearMode(bool is_clear_color, bool is_clear_depth)
{
    m_flag.set(FLAG_CLEAR_COLOR, is_clear_color);
    m_flag.set(FLAG_CLEAR_DEPTH, is_clear_depth);
}


ckCol ckScr::getClearColor() const
{
    return m_clear_col;
}


void ckScr::setClearColor(ckCol col)
{
    m_clear_col = col;
}


bool ckScr::isPerspective() const
{
    return m_flag.isOn(FLAG_PERSPECTIVE);
}


void ckScr::setPerspective(bool is_perspective)
{
    m_flag.set(FLAG_PERSPECTIVE, is_perspective);
}


r32 ckScr::getFocusDist() const
{
    return m_focus_dist;
}


void ckScr::setFocusDist(r32 focus_dist)
{
    if (focus_dist < ckMath::EPSILON)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_focus_dist = focus_dist;
}


r32 ckScr::getNearClipDist() const
{
    return m_near_clip_dist;
}


r32 ckScr::getFarClipDist() const
{
    return m_far_clip_dist;
}


void ckScr::setClipDist(r32 near, r32 far)
{
    if (near < ckMath::EPSILON || far <= near)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_near_clip_dist = near;
    m_far_clip_dist = far;
}


ckMat& ckScr::view()
{
    return m_view;
}


bool ckScr::hasScreenTexture() const
{
    return m_scr_tex ? true : false;
}


ckID ckScr::getScreenTextureID() const
{
    if (!hasScreenTexture())
    {
        ckThrow(ExceptionInvalidCall);
    }

    return m_scr_tex->getID();
}


void ckScr::attachScreenTexture(ckTex::TexFormat format)
{
    if (hasScreenTexture())
    {
        detachScreenTexture();
    }

    if (m_width_in_framebuffer > ckDrawMgr::getMaxTextureLength() || m_height_in_framebuffer > ckDrawMgr::getMaxTextureLength())
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (format != ckTex::FORMAT_RGB && format != ckTex::FORMAT_RGBA)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_scr_tex = ckNew(ckTex)(ckID::genID(), m_width_in_framebuffer, m_height_in_framebuffer, format, ckTex::MODE_FRAMEBUFFER, NULL, 0);

    m_flag.setOn(FLAG_COPY_SCREEN);
}


void ckScr::detachScreenTexture()
{
    if (hasScreenTexture())
    {
        ckDelete(m_scr_tex, ckTex);
        m_scr_tex = NULL;

        m_flag.setOff(FLAG_COPY_SCREEN);
    }
}


void ckScr::updateScreenTexture(bool is_frame_skip_reset)
{
    if (!hasScreenTexture())
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (is_frame_skip_reset)
    {
        ckTaskMgr::resetFrameSkip();
    }

    m_flag.setOn(FLAG_COPY_SCREEN);
}


ckID ckScr::getGuestScreenID(u8 index) const
{
    if (index >= GUEST_SCREEN_NUM)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_guest_id[index];
}


void ckScr::setGuestScreenID(u8 index, ckID scr_id)
{
    if (index >= GUEST_SCREEN_NUM)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_guest_id[index] = scr_id;
}


r32 ckScr::screenXToFramebufferX(r32 x_in_screen) const
{
    return (x_in_screen / m_view_width + 0.5f) * m_width_in_framebuffer + m_left_in_framebuffer;
}


r32 ckScr::screenYToFramebufferY(r32 y_in_screen) const
{
    return (0.5f - y_in_screen / m_view_height) * m_height_in_framebuffer + m_top_in_framebuffer;
}


r32 ckScr::framebufferXToScreenX(r32 x_in_framebuffer) const
{
    return ((x_in_framebuffer - m_left_in_framebuffer) / m_width_in_framebuffer - 0.5f) * m_view_width;
}


r32 ckScr::framebufferYToScreenY(r32 y_in_framebuffer) const
{
    return (0.5f - (y_in_framebuffer - m_top_in_framebuffer) / m_height_in_framebuffer) * m_view_height;
}


ckVec ckScr::worldToScreen(const ckVec& pos_in_world)
{
    calcProjection();

    ckVec vec = worldToClip_noCalcProjection(pos_in_world);

    return ckVec(vec.x * m_view_width / 2.0f, vec.y * m_view_height / 2.0f, vec.z);
}


ckVec ckScr::worldToFramebuffer(const ckVec& pos_in_world)
{
    calcProjection();

    ckVec vec = worldToClip_noCalcProjection(pos_in_world);

    return ckVec( //
        (vec.x + 1.0f) / 2.0f * m_width_in_framebuffer + m_left_in_framebuffer, //
        (1.0f - vec.y) / 2.0f * m_height_in_framebuffer + m_top_in_framebuffer, //
        vec.z);
}


ckVec ckScr::screenToPlane(r32 x_in_screen, r32 y_in_screen, const ckMat& xy_plane) const
{
    ckVec from = m_view.trans.toLocalOf(xy_plane);
    ckVec ray = ckVec(x_in_screen, y_in_screen, -m_focus_dist).toGlobalFrom_noTrans(m_view).toLocalOf_noTrans(xy_plane);
    ckVec vec = from - ray / ray.z * from.z;

    return ckVec(vec.x, vec.y, 0.0f);
}


ckVec ckScr::framebufferToPlane(r32 x_in_framebuffer, r32 y_in_framebuffer, const ckMat& xy_plane) const
{
    return screenToPlane(framebufferXToScreenX(x_in_framebuffer), framebufferYToScreenY(y_in_framebuffer), xy_plane);
}


bool ckScr::isInScreen(const ckVec& pos_in_world)
{
    calcProjection();

    ckVec vec = worldToClip_noCalcProjection(pos_in_world);

    return (ckMath::abs(vec.x) <= 1.0f && ckMath::abs(vec.y) <= 1.0f && ckMath::abs(vec.z) <= 1.0f);
}


bool ckScr::canBoundClip(const ckMat& world, const ckVec& bound_max, const ckVec& bound_min)
{
    calcProjection();

    return canBoundClip_noCalcProjection(world, bound_max, bound_min);
}


ckVec ckScr::calcVisibleVector(const ckVec& pos1, const ckVec& pos2)
{
    calcProjection();

    ckVec vec = worldToClip_noCalcProjection(pos2) - worldToClip_noCalcProjection(pos1);
    vec.set(-vec.y * m_view_height, vec.x * m_view_width, 0.0f);

    r32 sq_length = vec.sqLength();

    return (sq_length > ckMath::EPSILON) ? vec.toGlobalFrom_noTrans(m_view) / ckMath::sqrt(sq_length) : m_view.x_axis;
}


ckScr::ckScr(ckID scr_id)
{
    m_id = scr_id;
    m_view = ckMat::UNIT.translate(0.0f, 0.0f, static_cast<r32>(DEFAULT_FOCUS_DIST));
    m_root_draw.m_private_flag.setOn(ckDraw::FLAG_INITIALIZED);
    m_root_draw.m_scr_id = m_id;
    m_scr_tex = NULL;

    m_flag.clear();

    for (u32 i = 0; i < GUEST_SCREEN_NUM; i++)
    {
        m_guest_id[i] = ckID::ZERO;
    }

    setActive(true);
    setAreaInFramebuffer(0, 0, ckSysMgr::getFramebufferWidth(), ckSysMgr::getFramebufferHeight());
    setViewSize(ckSysMgr::getFramebufferWidth(), ckSysMgr::getFramebufferHeight());
    setClearMode(true, true);
    setClearColor(ckCol(255, 0, 0, 0));
    setPerspective(true);
    setFocusDist(static_cast<r32>(DEFAULT_FOCUS_DIST));
    setClipDist(static_cast<r32>(DEFAULT_NEAR_CLIP_DIST), static_cast<r32>(DEFAULT_FAR_CLIP_DIST));

    ckDrawMgr::instance()->m_scr_map.add(m_id, this);
}


ckScr::~ckScr()
{
    ckDrawMgr::instance()->m_scr_map.remove(m_id);

    detachScreenTexture();
}


CK_DEFINE_OPERATOR_EQUAL(ckScr)


void ckScr::calcProjection()
{
    r32* m = m_view_to_clip;

    if (isPerspective())
    {
        float inv_sub = 1.0f / (m_far_clip_dist - m_near_clip_dist);

        m[0] = m_focus_dist * 2.0f / m_view_width;
        m[4] = 0.0f;
        m[8] = 0.0f;
        m[12] = 0.0f;

        m[1] = 0.0f;
        m[5] = m_focus_dist * 2.0f / m_view_height;
        m[9] = 0.0f;
        m[13] = 0.0f;

        m[2] = 0.0f;
        m[6] = 0.0f;
        m[10] = (m_far_clip_dist + m_near_clip_dist) * inv_sub;
        m[14] = 2.0f * m_far_clip_dist * m_near_clip_dist * inv_sub;

        m[3] = 0.0f;
        m[7] = 0.0f;
        m[11] = -1.0f;
        m[15] = 0.0f;
    }
    else
    {
        m[0] = 2.0f / m_view_width;
        m[4] = 0.0f;
        m[8] = 0.0f;
        m[12] = 0.0f;

        m[1] = 0.0f;
        m[5] = 2.0f / m_view_height;
        m[9] = 0.0f;
        m[13] = 0.0f;

        m[2] = 0.0f;
        m[6] = 0.0f;
        m[10] = 2.0f / (m_far_clip_dist - m_near_clip_dist);
        m[14] = (m_far_clip_dist + m_near_clip_dist) / (m_far_clip_dist - m_near_clip_dist);

        m[3] = 0.0f;
        m[7] = 0.0f;
        m[11] = 0.0f;
        m[15] = 1.0f;
    }

    ckMat world_to_view = ckMat::UNIT.toLocalOf(m_view);
    world_to_view.toR32x16(m_world_to_view);

    ckMat::mulR32x16(m_world_to_clip, m_view_to_clip, m_world_to_view);
}


void ckScr::setupProjection()
{
    calcProjection();
    ckLowLevelAPI::setProjection(m_world_to_clip);
}


ckVec ckScr::worldToClip_noCalcProjection(const ckVec& pos_in_world) const
{
    r32 v[4];

    pos_in_world.toR32x4(v, 1.0f);
    ckVec::mulR32x4(v, m_world_to_clip, v);

    return ckVec::fromR32x4(v) / v[3];
}


bool ckScr::canBoundClip_noCalcProjection(const ckMat& world, const ckVec& bound_max, const ckVec& bound_min) const
{
    ckVec pos[8];

    pos[0].set(bound_min.x, bound_min.y, bound_min.z);
    pos[1].set(bound_max.x, bound_min.y, bound_min.z);
    pos[2].set(bound_min.x, bound_max.y, bound_min.z);
    pos[3].set(bound_max.x, bound_max.y, bound_min.z);
    pos[4].set(bound_min.x, bound_min.y, bound_max.z);
    pos[5].set(bound_max.x, bound_min.y, bound_max.z);
    pos[6].set(bound_min.x, bound_max.y, bound_max.z);
    pos[7].set(bound_max.x, bound_max.y, bound_max.z);

    for (s32 i = 0; i < 8; i++)
    {
        pos[i] = pos[i].toGlobalFrom(world);
        pos[i] = worldToClip_noCalcProjection(pos[i]);
    }

    ckVec min = pos[0];
    ckVec max = pos[0];

    for (s32 i = 1; i < 8; i++)
    {
        if (min.x > pos[i].x)
        {
            min.x = pos[i].x;
        }

        if (min.y > pos[i].y)
        {
            min.y = pos[i].y;
        }

        if (min.z > pos[i].z)
        {
            min.z = pos[i].z;
        }

        if (max.x < pos[i].x)
        {
            max.x = pos[i].x;
        }

        if (max.y < pos[i].y)
        {
            max.y = pos[i].y;
        }

        if (max.z < pos[i].z)
        {
            max.z = pos[i].z;
        }
    }

    return (min.x > 1.0f || min.y > 1.0f || min.z > 1.0f || max.x < -1.0f || max.y < -1.0f || max.z < -1.0f);
}


void ckScr::copyScreenTexture()
{
    if (hasScreenTexture() && m_flag.isOn(ckScr::FLAG_COPY_SCREEN))
    {
        if (m_scr_tex->m_tex_obj == 0)
        {
            m_scr_tex->m_tex_obj = ckLowLevelAPI::registerTexture( //
                ckDrawMgr::getValidTextureLength(m_scr_tex->m_width), ckDrawMgr::getValidTextureLength(m_scr_tex->m_height), //
                static_cast<ckLowLevelAPI::TextureFormat>(m_scr_tex->m_format.getType()), NULL);
        }

        ckLowLevelAPI::copyImageFromFramebufferToTexture( //
            m_scr_tex->m_tex_obj, //
            ckDrawMgr::getValidTextureLength(m_scr_tex->m_width), ckDrawMgr::getValidTextureLength(m_scr_tex->m_height), //
            static_cast<ckLowLevelAPI::TextureFormat>(m_scr_tex->m_format.getType()), //
            m_left_in_framebuffer, m_top_in_framebuffer);

        m_flag.setOff(ckScr::FLAG_COPY_SCREEN);
    }
}
