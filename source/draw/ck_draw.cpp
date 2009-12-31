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

#include "ck_low_level_api.h"
#include "ck_private_macro.h"


ckDraw::~ckDraw()
{
    if (ckDrawMgr::isCreated() && m_private_flag.isOn(FLAG_INITIALIZED) && m_scr_id != ckDrawMgr::INVISIBLE_SCREEN_ID)
    {
        while (hasChild())
        {
            getFirstChildN()->setScreenID(ckDrawMgr::INVISIBLE_SCREEN_ID);
        }
    }
}


bool ckDraw::hasScreen() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return (m_tree.hasParent() && !m_tree.getParentN()->hasParent());
}


ckID ckDraw::getScreenID() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (!hasScreen())
    {
        ckThrow(ExceptionInvalidCall);
    }

    return m_tree.getParentN()->getSelf()->m_scr_id;
}


void ckDraw::setScreenID(ckID scr_id)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (scr_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckDrawMgr::getScreen(scr_id)->m_root_draw.m_tree.addLast(&m_tree);
}


bool ckDraw::hasParent() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return (m_tree.hasParent() && m_tree.getParentN()->hasParent());
}


ckDraw* ckDraw::getParentN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return hasParent() ? m_tree.getParentN()->getSelf() : NULL;
}


void ckDraw::setParent(ckDraw* parent)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (!parent)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    parent->m_tree.addLast(&m_tree);
}


ckDraw* ckDraw::getPrevAllN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    ckTree<ckDraw>* prev = m_tree.getPrevAllN();

    return (prev && prev->hasParent()) ? prev->getSelf() : NULL;
}


ckDraw* ckDraw::getNextAllN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    ckTree<ckDraw>* next = m_tree.getNextAllN();

    return next ? next->getSelf() : NULL;
}


ckDraw* ckDraw::getPrevSiblingN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    ckTree<ckDraw>* sibling = m_tree.getPrevSiblingN();

    return sibling ? sibling->getSelf() : NULL;
}


ckDraw* ckDraw::getNextSiblingN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    ckTree<ckDraw>* sibling = m_tree.getNextSiblingN();

    return sibling ? sibling->getSelf() : NULL;
}


ckDraw* ckDraw::getLastDescendant() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_tree.getLastDescendant()->getSelf();
}


bool ckDraw::hasChild() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_tree.hasChild();
}


ckDraw* ckDraw::getFirstChildN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    ckTree<ckDraw>* child = m_tree.getFirstChildN();

    return child ? child->getSelf() : NULL;
}


ckDraw* ckDraw::getLastChildN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    ckTree<ckDraw>* child = m_tree.getLastChildN();

    return child ? child->getSelf() : NULL;
}


void ckDraw::moveFirst()
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_tree.getParentN()->addFirst(&m_tree);
}


void ckDraw::moveLast()
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_tree.getParentN()->addLast(&m_tree);
}


void ckDraw::moveBefore(ckDraw* draw)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (!draw || !draw->getParentN())
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_tree.joinBefore(&draw->m_tree);
}


void ckDraw::moveAfter(ckDraw* draw)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (!draw || !draw->getParentN())
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_tree.joinAfter(&draw->m_tree);
}


ckDraw::DrawType ckDraw::getType() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_type.getType();
}


bool ckDraw::isVisible() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_private_flag.isOn(FLAG_VISIBLE);
}


void ckDraw::setVisible(bool is_visible)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_private_flag.set(FLAG_VISIBLE, is_visible);
}


ckCol ckDraw::getColor() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_local_col;
}


void ckDraw::setColor(ckCol col)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_local_col = col;
}


ckDraw::DepthTest ckDraw::getDepthTest() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_depth_test.getType();
}


void ckDraw::setDepthTest(DepthTest depth_test)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_depth_test = depth_test;
}


ckDraw::BlendMode ckDraw::getBlendMode() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_blend_mode.getType();
}


void ckDraw::setBlendMode(BlendMode blend_mode, bool is_preset_setting)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_blend_mode = blend_mode;

    if (is_preset_setting)
    {
        setDepthTest(DEPTH_TEST_GEQUAL);
        m_draw_flag.clear();
        m_draw_flag.setOn(FLAG_WRITE_RGB);
        m_draw_flag.setOn(FLAG_BILINEAR);

        switch (m_blend_mode.getType())
        {
        case BLEND_OFF:
            m_draw_flag.setOn(FLAG_WRITE_DEPTH);
            break;

        case BLEND_HALF:
            m_draw_flag.setOn(FLAG_SORT);
            break;

        case BLEND_ADD:
            m_draw_flag.setOn(FLAG_SORT);
            break;

        case BLEND_DEST_ALPHA:
            m_draw_flag.setOn(FLAG_WRITE_DEPTH);
            break;

        default:
            break;
        }
    }
}


bool ckDraw::isDrawFlag(DrawFlag draw_flag) const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_draw_flag.isOn(draw_flag);
}


void ckDraw::setDrawFlag(DrawFlag draw_flag, bool is_on)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_draw_flag.set(draw_flag, is_on);
}


void ckDraw::clearDrawFlag()
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_draw_flag.clear();
}


void ckDraw::copyDrawFlag(const ckDraw* src)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (!src)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (src->m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_draw_flag = src->m_draw_flag;
}


const ckVec& ckDraw::getClipBoundMin() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_bound_min;
}


const ckVec& ckDraw::getClipBoundMax() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_bound_max;
}


void ckDraw::setClipBound(const ckVec& bound_min, const ckVec& bound_max)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (bound_min.x > bound_max.x || bound_min.y > bound_max.y || bound_min.z > bound_max.z)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_bound_min = bound_min;
    m_bound_max = bound_max;
}


const ckVec& ckDraw::getSortCenter() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_sort_center;
}


void ckDraw::setSortCenter(const ckVec& sort_center)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_sort_center = sort_center;
}


r32 ckDraw::getSortOffset() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_sort_offset;
}


void ckDraw::setSortOffset(r32 sort_offset)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_sort_offset = sort_offset;
}


ckID ckDraw::getTextureID() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_tex ? m_tex->getID() : ckID::ZERO;
}


void ckDraw::setTextureID(ckID tex_id)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_tex = (tex_id != ckID::ZERO) ? ckDrawMgr::getTexture(tex_id) : NULL;
}


ckMat& ckDraw::local()
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_local;
}


ckMat ckDraw::calcWorld() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    ckMat world = m_local;

    for (ckDraw* parent = getParentN(); parent; parent = parent->getParentN())
    {
        world = world.toGlobalFrom(parent->m_local);
    }

    return world;
}


ckCol ckDraw::calcFinalColor() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    ckCol final_col = m_local_col;

    for (ckDraw* parent = getParentN(); parent; parent = parent->getParentN())
    {
        final_col *= parent->m_local_col;
    }

    return final_col;
}


ckDraw::ckDraw()
{
    m_scr_id = ckID::ZERO;
    m_tex = NULL;
    m_local = ckMat::UNIT;
    m_world = ckMat::UNIT;
    m_local_col = ckCol::FULL;
    m_final_col = ckCol::FULL;
    m_bound_min = ckVec::ZERO;
    m_bound_max = ckVec::ZERO;
    m_sort_center = ckVec::ZERO;
    m_sort_offset = 0.0f;
    m_sort_value = 0.0f;
    m_next_sort = NULL;

    m_tree.init(this);

    m_private_flag.setOn(FLAG_VISIBLE);

    m_private_flag.setOn(FLAG_INITIALIZED); // to call SetPreset_DefaultBlendOff
    setBlendMode(BLEND_OFF, true);
    m_private_flag.setOff(FLAG_INITIALIZED); // restore initialized-flag
}


void ckDraw::setupWorld()
{
    m_world = m_local.toGlobalFrom(m_tree.getParentN()->getSelf()->m_world);
}


void ckDraw::setupFinalColor()
{
    m_final_col = m_tree.getParentN()->getSelf()->m_final_col * m_local_col;
}


void ckDraw::setupSortValue(const ckMat& view)
{
    ckVec sort_center = m_sort_center.toGlobalFrom(m_world);

    m_sort_value = (sort_center - view.trans).dot(view.z_axis) + m_sort_offset;
}


void ckDraw::setupDrawState()
{
    ckLowLevelAPI::setDepthTest(static_cast<ckLowLevelAPI::DepthTest>(m_depth_test.getType()));
    ckLowLevelAPI::setBlendMode(static_cast<ckLowLevelAPI::BlendMode>(m_blend_mode.getType()));
    ckLowLevelAPI::setWriteMode(m_draw_flag.isOn(FLAG_WRITE_RGB), m_draw_flag.isOn(FLAG_WRITE_ALPHA), m_draw_flag.isOn(FLAG_WRITE_DEPTH));
    ckLowLevelAPI::setBackfaceCulling(m_draw_flag.isOn(FLAG_BACKFACE_CULLING));

    r32 world[16];
    m_world.toR32x16(world);
    ckLowLevelAPI::setModelView(world);
}


void ckDraw::render(const ckMat& view)
{
    ckThrow(ExceptionInvalidCall);
}


CK_DEFINE_COPY_CONSTRUCTOR(ckDraw)


CK_DEFINE_OPERATOR_EQUAL(ckDraw)
