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


#include "pg_draw_all.h"

#include "pg_sys_all.h"
#include "pg_task_all.h"
#include "pg_low_level_api.h"
#include "pg_private_macro.h"


pgScr* pgScr::getPrevN() const
{
    pgDrawMgr* ins = pgDrawMgr::instance();
    const pgID* id = ins->m_scr_map.getPrevKeyN(m_id);

    return id ? *ins->m_scr_map.get(*id) : NULL;
}


pgScr* pgScr::getNextN() const
{
    pgDrawMgr* ins = pgDrawMgr::instance();
    const pgID* id = ins->m_scr_map.getNextKeyN(m_id);

    return id ? *ins->m_scr_map.get(*id) : NULL;
}


void pgScr::moveFirst()
{
    pgDrawMgr::instance()->m_scr_map.moveFirst(m_id);
}


void pgScr::moveLast()
{
    pgDrawMgr::instance()->m_scr_map.moveLast(m_id);
}


void pgScr::moveBefore(pgID id)
{
    pgDrawMgr* ins = pgDrawMgr::instance();

    if (id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgTry
    {
        ins->m_scr_map.get(id);
    }
    pgCatch(pgMap<pgID, pgScr*>::ExceptionNotFound)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    ins->m_scr_map.moveBefore(m_id, id);
}


void pgScr::moveAfter(pgID id)
{
    pgDrawMgr* ins = pgDrawMgr::instance();

    if (id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgTry
    {
        ins->m_scr_map.get(id);
    }
    pgCatch(pgMap<pgID, pgScr*>::ExceptionNotFound)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    ins->m_scr_map.moveAfter(m_id, id);
}


pgDraw* pgScr::getFirstDrawN()
{
    return m_root_draw.getFirstChildN();
}


pgDraw* pgScr::getLastDrawN()
{
    pgDraw* desc = m_root_draw.getLastDescendant();

    return (desc != &m_root_draw) ? desc : NULL;
}


pgID pgScr::getID() const
{
    return m_id;
}


s16 pgScr::getLeftInFramebuffer() const
{
    return m_left_in_framebuffer;
}


s16 pgScr::getTopInFramebuffer() const
{
    return m_top_in_framebuffer;
}


s16 pgScr::getWidthInFramebuffer() const
{
    return m_width_in_framebuffer;
}


s16 pgScr::getHeightInFramebuffer() const
{
    return m_height_in_framebuffer;
}


void pgScr::setAreaInFramebuffer(s16 left, s16 top, u16 width, u16 height)
{
    if (width == 0 || height == 0)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_left_in_framebuffer = left;
    m_top_in_framebuffer = top;
    m_width_in_framebuffer = width;
    m_height_in_framebuffer = height;
}


r32 pgScr::getViewWidth() const
{
    return m_view_width;
}


r32 pgScr::getViewHeight() const
{
    return m_view_height;
}


void pgScr::setViewSize(r32 width, r32 height)
{
    m_view_width = width;
    m_view_height = height;
}


bool pgScr::isActive() const
{
    return m_flag.isOn(FLAG_ACTIVE);
}


void pgScr::setActive(bool is_active)
{
    m_flag.set(FLAG_ACTIVE, is_active);
}


bool pgScr::isClearColor() const
{
    return m_flag.isOn(FLAG_CLEAR_COLOR);
}


bool pgScr::isClearDepth() const
{
    return m_flag.isOn(FLAG_CLEAR_DEPTH);
}


void pgScr::setClearMode(bool is_clear_color, bool is_clear_depth)
{
    m_flag.set(FLAG_CLEAR_COLOR, is_clear_color);
    m_flag.set(FLAG_CLEAR_DEPTH, is_clear_depth);
}


pgCol pgScr::getClearColor() const
{
    return m_clear_col;
}


void pgScr::setClearColor(pgCol col)
{
    m_clear_col = col;
}


bool pgScr::isPerspective() const
{
    return m_flag.isOn(FLAG_PERSPECTIVE);
}


void pgScr::setPerspective(bool is_perspective)
{
    m_flag.set(FLAG_PERSPECTIVE, is_perspective);
}


r32 pgScr::getFocusDist() const
{
    return m_focus_dist;
}


void pgScr::setFocusDist(r32 focus_dist)
{
    if (focus_dist < pgMath::EPSILON)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_focus_dist = focus_dist;
}


r32 pgScr::getNearClipDist() const
{
    return m_near_clip_dist;
}


r32 pgScr::getFarClipDist() const
{
    return m_far_clip_dist;
}


void pgScr::setClipDist(r32 near, r32 far)
{
    if (near < pgMath::EPSILON || far <= near)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_near_clip_dist = near;
    m_far_clip_dist = far;
}


pgMat& pgScr::view()
{
    return m_view;
}


bool pgScr::hasScreenTexture() const
{
    return m_scr_tex ? true : false;
}


pgID pgScr::getScreenTextureID() const
{
    if (!hasScreenTexture())
    {
        pgThrow(ExceptionInvalidCall);
    }

    return m_scr_tex->getID();
}


void pgScr::attachScreenTexture(pgTex::TexFormat format)
{
    if (hasScreenTexture())
    {
        detachScreenTexture();
    }

    if (m_width_in_framebuffer > pgDrawMgr::getMaxTextureLength() || m_height_in_framebuffer > pgDrawMgr::getMaxTextureLength())
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (format != pgTex::FORMAT_RGB && format != pgTex::FORMAT_RGBA)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_scr_tex = pgNew(pgTex)(pgID::genID(), m_width_in_framebuffer, m_height_in_framebuffer, format, pgTex::MODE_FRAMEBUFFER, NULL, 0);

    m_flag.setOn(FLAG_COPY_SCREEN);
}


void pgScr::detachScreenTexture()
{
    if (hasScreenTexture())
    {
        pgDelete(m_scr_tex, pgTex);
        m_scr_tex = NULL;

        m_flag.setOff(FLAG_COPY_SCREEN);
    }
}


void pgScr::updateScreenTexture(bool is_frame_skip_reset)
{
    if (!hasScreenTexture())
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (is_frame_skip_reset)
    {
        pgTaskMgr::resetFrameSkip();
    }

    m_flag.setOn(FLAG_COPY_SCREEN);
}


pgID pgScr::getGuestScreenID(u8 index) const
{
    if (index >= GUEST_SCREEN_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_guest_id[index];
}


void pgScr::setGuestScreenID(u8 index, pgID scr_id)
{
    if (index >= GUEST_SCREEN_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_guest_id[index] = scr_id;
}


r32 pgScr::screenXToFramebufferX(r32 x_in_screen) const
{
    return (x_in_screen / m_view_width + 0.5f) * m_width_in_framebuffer + m_left_in_framebuffer;
}


r32 pgScr::screenYToFramebufferY(r32 y_in_screen) const
{
    return (0.5f - y_in_screen / m_view_height) * m_height_in_framebuffer + m_top_in_framebuffer;
}


r32 pgScr::framebufferXToScreenX(r32 x_in_framebuffer) const
{
    return ((x_in_framebuffer - m_left_in_framebuffer) / m_width_in_framebuffer - 0.5f) * m_view_width;
}


r32 pgScr::framebufferYToScreenY(r32 y_in_framebuffer) const
{
    return (0.5f - (y_in_framebuffer - m_top_in_framebuffer) / m_height_in_framebuffer) * m_view_height;
}


pgVec pgScr::worldToScreen(const pgVec& pos_in_world)
{
    calcProjection();

    pgVec vec = worldToClip_noCalcProjection(pos_in_world);

    return pgVec(vec.x * m_view_width / 2.0f, vec.y * m_view_height / 2.0f, vec.z);
}


pgVec pgScr::worldToFramebuffer(const pgVec& pos_in_world)
{
    calcProjection();

    pgVec vec = worldToClip_noCalcProjection(pos_in_world);

    return pgVec( //
        (vec.x + 1.0f) / 2.0f * m_width_in_framebuffer + m_left_in_framebuffer, //
        (1.0f - vec.y) / 2.0f * m_height_in_framebuffer + m_top_in_framebuffer, //
        vec.z);
}


pgVec pgScr::screenToPlane(r32 x_in_screen, r32 y_in_screen, const pgMat& xy_plane) const
{
    pgVec from = m_view.trans.toLocalOf(xy_plane);
    pgVec ray = pgVec(x_in_screen, y_in_screen, -m_focus_dist).toGlobalFrom_noTrans(m_view).toLocalOf_noTrans(xy_plane);
    pgVec vec = from - ray / ray.z * from.z;

    return pgVec(vec.x, vec.y, 0.0f);
}


pgVec pgScr::framebufferToPlane(r32 x_in_framebuffer, r32 y_in_framebuffer, const pgMat& xy_plane) const
{
    return screenToPlane(framebufferXToScreenX(x_in_framebuffer), framebufferYToScreenY(y_in_framebuffer), xy_plane);
}


bool pgScr::isInScreen(const pgVec& pos_in_world)
{
    calcProjection();

    pgVec vec = worldToClip_noCalcProjection(pos_in_world);

    return (pgMath::abs(vec.x) <= 1.0f && pgMath::abs(vec.y) <= 1.0f && pgMath::abs(vec.z) <= 1.0f);
}


bool pgScr::canBoundClip(const pgMat& world, const pgVec& bound_max, const pgVec& bound_min)
{
    calcProjection();

    return canBoundClip_noCalcProjection(world, bound_max, bound_min);
}


pgVec pgScr::calcVisibleVector(const pgVec& pos1, const pgVec& pos2)
{
    calcProjection();

    pgVec vec = worldToClip_noCalcProjection(pos2) - worldToClip_noCalcProjection(pos1);
    vec.set(-vec.y * m_view_height, vec.x * m_view_width, 0.0f);

    r32 sq_length = vec.sqLength();

    return (sq_length > pgMath::EPSILON) ? vec.toGlobalFrom_noTrans(m_view) / pgMath::sqrt(sq_length) : m_view.x_axis;
}


pgScr::pgScr(pgID scr_id)
{
    m_id = scr_id;
    m_view = pgMat::UNIT.translate(0.0f, 0.0f, static_cast<r32>(DEFAULT_FOCUS_DIST));
    m_root_draw.m_private_flag.setOn(pgDraw::FLAG_INITIALIZED);
    m_root_draw.m_scr_id = m_id;
    m_scr_tex = NULL;

    m_flag.clear();

    for (u32 i = 0; i < GUEST_SCREEN_NUM; i++)
    {
        m_guest_id[i] = pgID::ZERO;
    }

    setActive(true);
    setAreaInFramebuffer(0, 0, pgSysMgr::getFramebufferWidth(), pgSysMgr::getFramebufferHeight());
    setViewSize(pgSysMgr::getFramebufferWidth(), pgSysMgr::getFramebufferHeight());
    setClearMode(true, true);
    setClearColor(pgCol(255, 0, 0, 0));
    setPerspective(true);
    setFocusDist(static_cast<r32>(DEFAULT_FOCUS_DIST));
    setClipDist(static_cast<r32>(DEFAULT_NEAR_CLIP_DIST), static_cast<r32>(DEFAULT_FAR_CLIP_DIST));

    pgDrawMgr::instance()->m_scr_map.add(m_id, this);
}


pgScr::~pgScr()
{
    pgDrawMgr::instance()->m_scr_map.remove(m_id);

    detachScreenTexture();
}


PG_DEFINE_OPERATOR_EQUAL(pgScr)


void pgScr::calcProjection()
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

    pgMat world_to_view = pgMat::UNIT.toLocalOf(m_view);
    world_to_view.toR32x16(m_world_to_view);

    pgMat::mulR32x16(m_world_to_clip, m_view_to_clip, m_world_to_view);
}


void pgScr::setupProjection()
{
    calcProjection();
    pgLowLevelAPI::setProjection(m_world_to_clip);
}


pgVec pgScr::worldToClip_noCalcProjection(const pgVec& pos_in_world) const
{
    r32 v[4];

    pos_in_world.toR32x4(v, 1.0f);
    pgVec::mulR32x4(v, m_world_to_clip, v);

    return pgVec::fromR32x4(v) / v[3];
}


bool pgScr::canBoundClip_noCalcProjection(const pgMat& world, const pgVec& bound_max, const pgVec& bound_min) const
{
    pgVec pos[8];

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

    pgVec min = pos[0];
    pgVec max = pos[0];

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


void pgScr::copyScreenTexture()
{
    if (hasScreenTexture() && m_flag.isOn(pgScr::FLAG_COPY_SCREEN))
    {
        if (m_scr_tex->m_tex_obj == 0)
        {
            m_scr_tex->m_tex_obj = pgLowLevelAPI::registerTexture( //
                pgDrawMgr::getValidTextureLength(m_scr_tex->m_width), pgDrawMgr::getValidTextureLength(m_scr_tex->m_height), //
                static_cast<pgLowLevelAPI::TextureFormat>(m_scr_tex->m_format.getType()), NULL);
        }

        pgLowLevelAPI::copyImageFromFramebufferToTexture( //
            m_scr_tex->m_tex_obj, //
            pgDrawMgr::getValidTextureLength(m_scr_tex->m_width), pgDrawMgr::getValidTextureLength(m_scr_tex->m_height), //
            static_cast<pgLowLevelAPI::TextureFormat>(m_scr_tex->m_format.getType()), //
            m_left_in_framebuffer, m_top_in_framebuffer);

        m_flag.setOff(pgScr::FLAG_COPY_SCREEN);
    }
}
