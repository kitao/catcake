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


#include "ck_draw_all.h"

#include "ck_res_all.h"
#include "ck_private_macro.h"


ckMdl::ckMdl()
{
    m_draw_ptr = NULL;
    m_rend_ptr = NULL;
}


ckMdl::~ckMdl()
{
    uninit();
}


void ckMdl::init(ckID mdl_data_id, ckID scr_id)
{
    init2(mdl_data_id, scr_id, NULL);
}


void ckMdl::init(ckID mdl_data_id, ckDraw* parent)
{
    init2(mdl_data_id, ckID::ZERO, parent);
}


const ckMdlData* ckMdl::getModelData() const
{
    if (!m_draw_ptr)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return &m_mdl_data;
}


ckID ckMdl::getTextureID() const
{
    if (!m_draw_ptr)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_root_node.getTextureID();
}


void ckMdl::setTextureID(ckID tex_id)
{
    if (!m_draw_ptr)
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_root_node.setTextureID(tex_id);

    for (s32 i = 0; i < m_node_num; i++)
    {
        m_draw_ptr[i]->setTextureID(tex_id);
    }
}


ckID ckMdl::getLightSetID() const
{
    if (!m_draw_ptr)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_lts_id;
}


void ckMdl::setLightSetID(ckID lts_id)
{
    if (!m_draw_ptr)
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_lts_id = lts_id;

    if (m_mdl_data.hasNormal())
    {
        if (m_lts_id == ckID::ZERO)
        {
            for (s32 i = 0; i < m_node_num; i++)
            {
                if (m_rend_ptr[i])
                {
                    m_rend_ptr[i]->setEnabled(false);
                    m_rend_ptr[i]->setLightSetID(ckDrawMgr::DEFAULT_LIGHT_SET_ID);
                }
            }
        }
        else
        {
            for (s32 i = 0; i < m_node_num; i++)
            {
                if (m_rend_ptr[i])
                {
                    m_rend_ptr[i]->setEnabled(true);
                    m_rend_ptr[i]->setLightSetID(lts_id);
                }
            }
        }
    }
}


ckDraw* ckMdl::getRootDraw()
{
    if (!m_draw_ptr)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return &m_root_node;
}


ckDraw* ckMdl::getNodeDraw(u16 node_index)
{
    if (!m_draw_ptr)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_node_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_draw_ptr[node_index];
}


void ckMdl::init2(ckID mdl_data_id, ckID scr_id, ckDraw* parent)
{
    uninit();

    if (mdl_data_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckRes res;

    ckTry
    {
        res = ckResMgr::getResource(mdl_data_id);
    }
    ckCatch(ckResMgr::ExceptionNotFound)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_mdl_data.initAsReader(res.getData<void>(), res.getDataSize());

    m_node_num = m_mdl_data.getNodeNum();
    m_lts_id = ckID::ZERO;

    if (parent)
    {
        m_root_node.init(parent);
    }
    else
    {
        m_root_node.init(scr_id);
    }

    ckNewArray(m_draw_ptr, ckDraw*, m_node_num);
    ckNewArray(m_rend_ptr, ckRend_Light*, m_node_num);

    for (s32 i = 0; i < m_node_num; i++)
    {
        if (m_mdl_data.getNodeVertNum(i) > 0)
        {
            m_draw_ptr[i] = ckNew(ckPrim);
            m_rend_ptr[i] = m_mdl_data.hasNormal() ? ckNew(ckRend_Light) : NULL;
        }
        else
        {
            m_draw_ptr[i] = ckNew(ckNode);
            m_rend_ptr[i] = NULL;
        }
    }

    for (s32 i = 0; i < m_node_num; i++)
    {
        u16 vert_num = m_mdl_data.getNodeVertNum(i);
        u16 parent_index = m_mdl_data.getNodeParentIndex(i);

        if (vert_num > 0)
        {
            ckPrim* prim = static_cast<ckPrim*>(m_draw_ptr[i]);
            ckPrim::PrimData* prim_data = const_cast<ckPrim::PrimData*>(m_mdl_data.getNodePrimData(i));

            prim->init(m_mdl_data.getNodePrimMode(i), prim_data, vert_num, (parent_index == i) ? &m_root_node : m_draw_ptr[parent_index]);

            prim->setBlendMode(m_mdl_data.getNodeBlendMode(i));

            if (prim->getBlendMode() != ckDraw::BLEND_OFF)
            {
                prim->setDrawFlag(ckDraw::FLAG_SORT, true);
            }

            prim->setDrawFlag(ckDraw::FLAG_BACKFACE_CULLING, true);

            ckVec bound_min = m_mdl_data.getNodeClipBoundMinForReader(i);
            ckVec bound_max = m_mdl_data.getNodeClipBoundMaxForReader(i);

            prim->setDrawFlag(ckDraw::FLAG_BOUND_CLIP, true);
            prim->setClipBound(bound_min, bound_max);
            prim->setSortCenter((bound_min + bound_max) / 2.0f);

            prim->local() = m_mdl_data.getNodeLocal(i);

            if (m_mdl_data.hasNormal())
            {
                ckRend_Light::RendData* rend_data = const_cast<ckRend_Light::RendData*>(reinterpret_cast<const ckRend_Light::RendData*>(m_mdl_data.getNodeNormalData(i)));

                m_rend_ptr[i]->init(prim, rend_data, ckDrawMgr::DEFAULT_LIGHT_SET_ID);
                m_rend_ptr[i]->setEnabled(false);
            }
        }
        else
        {
            ckNode* node = static_cast<ckNode*>(m_draw_ptr[i]);

            node->init((parent_index == i) ? &m_root_node : m_draw_ptr[parent_index]);

            node->local() = m_mdl_data.getNodeLocal(i);

            m_rend_ptr[i] = NULL;
        }
    }

    setTextureID(m_mdl_data.getTextureID());
}


CK_DEFINE_COPY_CONSTRUCTOR(ckMdl)


CK_DEFINE_OPERATOR_EQUAL(ckMdl)


void ckMdl::uninit()
{
    if (m_draw_ptr)
    {
        for (s32 i = 0; i < m_node_num; i++)
        {
            ckDelete(m_draw_ptr[i], ckDraw);

            if (m_rend_ptr[i])
            {
                ckDelete(m_rend_ptr[i], ckRend_Light);
            }
        }

        typedef ckDraw* ckDrawPtr;
        ckDeleteArray(m_draw_ptr, ckDrawPtr);

        typedef ckRend* ckRendPtr;
        ckDeleteArray(m_rend_ptr, ckRendPtr);

        m_draw_ptr = NULL;
        m_rend_ptr = NULL;
    }
}
