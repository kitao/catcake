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

#include "pg_low_level_api.h"
#include "pg_private_macro.h"


pgDraw::~pgDraw()
{
    if (pgDrawMgr::isCreated() && m_private_flag.isOn(FLAG_INITIALIZED) && m_scr_id != pgDrawMgr::INVISIBLE_SCREEN_ID)
    {
        while (hasChild())
        {
            getFirstChildN()->setScreenID(pgDrawMgr::INVISIBLE_SCREEN_ID);
        }
    }
}


bool pgDraw::hasScreen() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return (m_tree.hasParent() && !m_tree.getParentN()->hasParent());
}


pgID pgDraw::getScreenID() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (!hasScreen())
    {
        pgThrow(ExceptionInvalidCall);
    }

    return m_tree.getParentN()->getSelf()->m_scr_id;
}


void pgDraw::setScreenID(pgID scr_id)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (scr_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgDrawMgr::getScreen(scr_id)->m_root_draw.m_tree.addLast(&m_tree);
}


bool pgDraw::hasParent() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return (m_tree.hasParent() && m_tree.getParentN()->hasParent());
}


pgDraw* pgDraw::getParentN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return hasParent() ? m_tree.getParentN()->getSelf() : NULL;
}


void pgDraw::setParent(pgDraw* parent)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (!parent)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    parent->m_tree.addLast(&m_tree);
}


pgDraw* pgDraw::getPrevAllN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    pgTree<pgDraw>* prev = m_tree.getPrevAllN();

    return (prev && prev->hasParent()) ? prev->getSelf() : NULL;
}


pgDraw* pgDraw::getNextAllN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    pgTree<pgDraw>* next = m_tree.getNextAllN();

    return next ? next->getSelf() : NULL;
}


pgDraw* pgDraw::getPrevSiblingN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    pgTree<pgDraw>* sibling = m_tree.getPrevSiblingN();

    return sibling ? sibling->getSelf() : NULL;
}


pgDraw* pgDraw::getNextSiblingN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    pgTree<pgDraw>* sibling = m_tree.getNextSiblingN();

    return sibling ? sibling->getSelf() : NULL;
}


pgDraw* pgDraw::getLastDescendant() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_tree.getLastDescendant()->getSelf();
}


bool pgDraw::hasChild() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_tree.hasChild();
}


pgDraw* pgDraw::getFirstChildN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    pgTree<pgDraw>* child = m_tree.getFirstChildN();

    return child ? child->getSelf() : NULL;
}


pgDraw* pgDraw::getLastChildN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    pgTree<pgDraw>* child = m_tree.getLastChildN();

    return child ? child->getSelf() : NULL;
}


void pgDraw::moveFirst()
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_tree.getParentN()->addFirst(&m_tree);
}


void pgDraw::moveLast()
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_tree.getParentN()->addLast(&m_tree);
}


void pgDraw::moveBefore(pgDraw* draw)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (!draw || !draw->getParentN())
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_tree.joinBefore(&draw->m_tree);
}


void pgDraw::moveAfter(pgDraw* draw)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (!draw || !draw->getParentN())
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_tree.joinAfter(&draw->m_tree);
}


pgDraw::DrawType pgDraw::getType() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_type.getType();
}


bool pgDraw::isVisible() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_private_flag.isOn(FLAG_VISIBLE);
}


void pgDraw::setVisible(bool is_visible)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_private_flag.set(FLAG_VISIBLE, is_visible);
}


pgCol pgDraw::getColor() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_local_col;
}


void pgDraw::setColor(pgCol col)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_local_col = col;
}


pgDraw::DepthTest pgDraw::getDepthTest() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_depth_test.getType();
}


void pgDraw::setDepthTest(DepthTest depth_test)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_depth_test = depth_test;
}


pgDraw::BlendMode pgDraw::getBlendMode() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_blend_mode.getType();
}


void pgDraw::setBlendMode(BlendMode blend_mode)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_blend_mode = blend_mode;
}


bool pgDraw::isDrawFlag(DrawFlag draw_flag) const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_draw_flag.isOn(draw_flag);
}


void pgDraw::setDrawFlag(DrawFlag draw_flag, bool is_on)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_draw_flag.set(draw_flag, is_on);
}


void pgDraw::clearDrawFlag()
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_draw_flag.clear();
}


void pgDraw::copyDrawFlag(const pgDraw* src)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (!src)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (src->m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_draw_flag = src->m_draw_flag;
}


void pgDraw::setPreset_defaultBlendOff()
{
    setDepthTest(DEPTH_TEST_GEQUAL);
    setBlendMode(BLEND_OFF);

    m_draw_flag.clear();
    m_draw_flag.setOn(FLAG_WRITE_RGB);
    m_draw_flag.setOn(FLAG_WRITE_DEPTH);
    m_draw_flag.setOn(FLAG_BILINEAR);
}


void pgDraw::setPreset_defaultBlendHalf()
{
    setDepthTest(DEPTH_TEST_GEQUAL);
    setBlendMode(BLEND_HALF);

    m_draw_flag.clear();
    m_draw_flag.setOn(FLAG_SORT);
    m_draw_flag.setOn(FLAG_WRITE_RGB);
    m_draw_flag.setOn(FLAG_BILINEAR);
}


void pgDraw::setPreset_defaultBlendAdd()
{
    setDepthTest(DEPTH_TEST_GEQUAL);
    setBlendMode(BLEND_ADD);

    m_draw_flag.clear();
    m_draw_flag.setOn(FLAG_SORT);
    m_draw_flag.setOn(FLAG_WRITE_RGB);
    m_draw_flag.setOn(FLAG_BILINEAR);
}


const pgVec& pgDraw::getClipBoundMin() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_bound_min;
}


const pgVec& pgDraw::getClipBoundMax() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_bound_max;
}


void pgDraw::setClipBound(const pgVec& bound_min, const pgVec& bound_max)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (bound_min.x > bound_max.x || bound_min.y > bound_max.y || bound_min.z > bound_max.z)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_bound_min = bound_min;
    m_bound_max = bound_max;
}


const pgVec& pgDraw::getSortCenter() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_sort_center;
}


void pgDraw::setSortCenter(const pgVec& sort_center)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_sort_center = sort_center;
}


r32 pgDraw::getSortOffset() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_sort_offset;
}


void pgDraw::setSortOffset(r32 sort_offset)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_sort_offset = sort_offset;
}


pgID pgDraw::getTextureID() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_tex ? m_tex->getID() : pgID::ZERO;
}


void pgDraw::setTextureID(pgID tex_id)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_tex = (tex_id != pgID::ZERO) ? pgDrawMgr::getTexture(tex_id) : NULL;
}


pgMat& pgDraw::local()
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_local;
}


pgMat pgDraw::calcWorld() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    pgMat world = m_local;

    for (pgDraw* parent = getParentN(); parent; parent = parent->getParentN())
    {
        world = world.toGlobalFrom(parent->m_local);
    }

    return world;
}


pgCol pgDraw::calcFinalColor() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    pgCol final_col = m_local_col;

    for (pgDraw* parent = getParentN(); parent; parent = parent->getParentN())
    {
        final_col *= parent->m_local_col;
    }

    return final_col;
}


pgDraw::pgDraw()
{
    m_scr_id = pgID::ZERO;
    m_tex = NULL;
    m_local = pgMat::UNIT;
    m_world = pgMat::UNIT;
    m_local_col = pgCol::FULL;
    m_final_col = pgCol::FULL;
    m_bound_min = pgVec::ZERO;
    m_bound_max = pgVec::ZERO;
    m_sort_center = pgVec::ZERO;
    m_sort_offset = 0.0f;
    m_sort_value = 0.0f;
    m_next_sort = NULL;

    m_tree.init(this);

    m_private_flag.setOn(FLAG_VISIBLE);

    m_private_flag.setOn(FLAG_INITIALIZED); // to call SetPreset_DefaultBlendOff
    setPreset_defaultBlendOff();
    m_private_flag.setOff(FLAG_INITIALIZED); // restore initialized-flag
}


void pgDraw::setupWorld()
{
    m_world = m_local.toGlobalFrom(m_tree.getParentN()->getSelf()->m_world);
}


void pgDraw::setupFinalColor()
{
    m_final_col = m_tree.getParentN()->getSelf()->m_final_col * m_local_col;
}


void pgDraw::setupSortValue(const pgMat& view)
{
    pgVec sort_center = m_sort_center.toGlobalFrom(m_world);

    m_sort_value = (sort_center - view.trans).dot(view.z_axis) + m_sort_offset;
}


void pgDraw::setupDrawState()
{
    pgLowLevelAPI::setDepthTest(static_cast<pgLowLevelAPI::DepthTest>(m_depth_test.getType()));
    pgLowLevelAPI::setBlendMode(static_cast<pgLowLevelAPI::BlendMode>(m_blend_mode.getType()));
    pgLowLevelAPI::setWriteMode(m_draw_flag.isOn(FLAG_WRITE_RGB), m_draw_flag.isOn(FLAG_WRITE_ALPHA), m_draw_flag.isOn(FLAG_WRITE_DEPTH));
    pgLowLevelAPI::setBackfaceCulling(m_draw_flag.isOn(FLAG_BACKFACE_CULLING));

    r32 world[16];
    m_world.toR32x16(world);
    pgLowLevelAPI::setModelView(world);
}


void pgDraw::render(const pgMat& view)
{
    pgThrow(ExceptionInvalidCall);
}


PG_DEFINE_COPY_CONSTRUCTOR(pgDraw)


PG_DEFINE_OPERATOR_EQUAL(pgDraw)
