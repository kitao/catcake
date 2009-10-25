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

#include "pg_res_all.h"
#include "pg_private_macro.h"


pgMdl::pgMdl()
{
    m_draw_ptr = NULL;
    m_rend_ptr = NULL;
}


pgMdl::~pgMdl()
{
    uninit();
}


void pgMdl::init(pgID mdl_data_id, pgID scr_id)
{
    init2(mdl_data_id, scr_id, NULL);
}


void pgMdl::init(pgID mdl_data_id, pgDraw* parent)
{
    init2(mdl_data_id, pgID::ZERO, parent);
}


const pgMdlData* pgMdl::getModelData() const
{
    if (!m_draw_ptr)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return &m_mdl_data;
}


pgID pgMdl::getTextureID() const
{
    if (!m_draw_ptr)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_root_node.getTextureID();
}


void pgMdl::setTextureID(pgID tex_id)
{
    if (!m_draw_ptr)
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_root_node.setTextureID(tex_id);

    for (s32 i = 0; i < m_node_num; i++)
    {
        m_draw_ptr[i]->setTextureID(tex_id);
    }
}


pgID pgMdl::getLightSetID() const
{
    if (!m_draw_ptr)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_lts_id;
}


void pgMdl::setLightSetID(pgID lts_id)
{
    if (!m_draw_ptr)
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_lts_id = lts_id;

    if (m_mdl_data.hasNormal())
    {
        if (m_lts_id == pgID::ZERO)
        {
            for (s32 i = 0; i < m_node_num; i++)
            {
                if (m_rend_ptr[i])
                {
                    m_rend_ptr[i]->setActive(false);
                    m_rend_ptr[i]->setLightSetID(pgDrawMgr::DEFAULT_LIGHT_SET_ID);
                }
            }
        }
        else
        {
            for (s32 i = 0; i < m_node_num; i++)
            {
                if (m_rend_ptr[i])
                {
                    m_rend_ptr[i]->setActive(true);
                    m_rend_ptr[i]->setLightSetID(lts_id);
                }
            }
        }
    }
}


pgDraw* pgMdl::getRootDraw()
{
    if (!m_draw_ptr)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return &m_root_node;
}


pgDraw* pgMdl::getNodeDraw(u16 node_index)
{
    if (!m_draw_ptr)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_node_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_draw_ptr[node_index];
}


void pgMdl::init2(pgID mdl_data_id, pgID scr_id, pgDraw* parent)
{
    uninit();

    if (mdl_data_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgRes res;

    pgTry
    {
        res = pgResMgr::getResource(mdl_data_id);
    }
    pgCatch(pgResMgr::ExceptionNotFound)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_mdl_data.initAsReader(res.getData<void>(), res.getDataSize());

    m_node_num = m_mdl_data.getNodeNum();
    m_lts_id = pgID::ZERO;

    if (parent)
    {
        m_root_node.init(parent);
    }
    else
    {
        m_root_node.init(scr_id);
    }

    pgNewArray(m_draw_ptr, pgDraw*, m_node_num);
    pgNewArray(m_rend_ptr, pgRend_Light*, m_node_num);

    for (s32 i = 0; i < m_node_num; i++)
    {
        if (m_mdl_data.getNodeVertNum(i) > 0)
        {
            m_draw_ptr[i] = pgNew(pgPrim);
            m_rend_ptr[i] = m_mdl_data.hasNormal() ? pgNew(pgRend_Light) : NULL;
        }
        else
        {
            m_draw_ptr[i] = pgNew(pgNode);
            m_rend_ptr[i] = NULL;
        }
    }

    for (s32 i = 0; i < m_node_num; i++)
    {
        u16 vert_num = m_mdl_data.getNodeVertNum(i);
        u16 parent_index = m_mdl_data.getNodeParentIndex(i);

        if (vert_num > 0)
        {
            pgPrim* prim = static_cast<pgPrim*>(m_draw_ptr[i]);
            pgPrim::PrimData* prim_data = const_cast<pgPrim::PrimData*>(m_mdl_data.getNodePrimData(i));

            prim->init(m_mdl_data.getNodePrimMode(i), prim_data, vert_num, (parent_index == i) ? &m_root_node : m_draw_ptr[parent_index]);

            prim->setBlendMode(m_mdl_data.getNodeBlendMode(i));

            if (prim->getBlendMode() != pgDraw::BLEND_OFF)
            {
                prim->setDrawFlag(pgDraw::FLAG_SORT, true);
            }

            prim->setDrawFlag(pgDraw::FLAG_BACKFACE_CULLING, true);

            pgVec bound_min = m_mdl_data.getNodeClipBoundMinForReader(i);
            pgVec bound_max = m_mdl_data.getNodeClipBoundMaxForReader(i);

            prim->setDrawFlag(pgDraw::FLAG_BOUND_CLIP, true);
            prim->setClipBound(bound_min, bound_max);
            prim->setSortCenter((bound_min + bound_max) / 2.0f);

            prim->local() = m_mdl_data.getNodeLocal(i);

            if (m_mdl_data.hasNormal())
            {
                pgRend_Light::RendData* rend_data = const_cast<pgRend_Light::RendData*>(reinterpret_cast<const pgRend_Light::RendData*>(m_mdl_data.getNodeNormalData(i)));

                m_rend_ptr[i]->init(prim, rend_data, pgDrawMgr::DEFAULT_LIGHT_SET_ID);
                m_rend_ptr[i]->setActive(false);
            }
        }
        else
        {
            pgNode* node = static_cast<pgNode*>(m_draw_ptr[i]);

            node->init((parent_index == i) ? &m_root_node : m_draw_ptr[parent_index]);

            node->local() = m_mdl_data.getNodeLocal(i);

            m_rend_ptr[i] = NULL;
        }
    }

    setTextureID(m_mdl_data.getTextureID());
}


PG_DEFINE_COPY_CONSTRUCTOR(pgMdl)


PG_DEFINE_OPERATOR_EQUAL(pgMdl)


void pgMdl::uninit()
{
    if (m_draw_ptr)
    {
        for (s32 i = 0; i < m_node_num; i++)
        {
            pgDelete(m_draw_ptr[i], pgDraw);

            if (m_rend_ptr[i])
            {
                pgDelete(m_rend_ptr[i], pgRend_Light);
            }
        }

        typedef pgDraw* pgDrawPtr;
        pgDeleteArray(m_draw_ptr, pgDrawPtr);

        typedef pgRend* pgRendPtr;
        pgDeleteArray(m_rend_ptr, pgRendPtr);

        m_draw_ptr = NULL;
        m_rend_ptr = NULL;
    }
}
