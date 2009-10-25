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
#include "pg_util_all.h"
#include "pg_private_macro.h"


pgMdlData::pgMdlData()
{
    m_data = NULL;
}


pgMdlData::~pgMdlData()
{
    if (m_data && m_mode == MODE_WRITE)
    {
        pgFree(m_data);
    }
}


void pgMdlData::initAsReader(const void* data, u32 data_size)
{
    if (!data || data_size == 0)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (m_data && m_mode == MODE_WRITE)
    {
        pgFree(m_data);
    }

    m_mode = MODE_READ;
    m_data = reinterpret_cast<u8*>(const_cast<void*>(data));
    m_data_size = data_size;
    m_mdl_data_header = reinterpret_cast<ModelDataHeader*>(m_data);
    m_node_info = reinterpret_cast<NodeInfo*>(m_mdl_data_header + 1);
    m_prim_data = reinterpret_cast<pgPrim::PrimData*>(m_data + m_mdl_data_header->prim_data_offset);
    m_normal_data = reinterpret_cast<pgVec*>(m_data + m_mdl_data_header->normal_data_offset);

    if (m_mdl_data_header->format_id != pgID_("POGOLYN_MODEL_DATA") || m_mdl_data_header->format_version > MODEL_DATA_VERSION)
    {
        pgThrow(ExceptionInvalidData);
    }

    u32 valid_data_size = sizeof(ModelDataHeader) + sizeof(NodeInfo) * getNodeNum() + (sizeof(pgPrim::PrimData) + (hasNormal() ? sizeof(pgVec) : 0)) * getVertNum();

    if (m_data_size != valid_data_size)
    {
        pgThrow(ExceptionInvalidData);
    }
}


void pgMdlData::initAsWriter(u16 node_num, u16 vert_num, pgID tex_id, bool has_normal)
{
    if (node_num == 0 || vert_num == 0)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (m_data && m_mode == MODE_WRITE)
    {
        pgFree(m_data);
    }

    m_mode = MODE_WRITE;

    u32 node_info_size = sizeof(NodeInfo) * node_num;
    u32 prim_data_size = sizeof(pgPrim::PrimData) * vert_num;
    u32 normal_data_size = has_normal ? sizeof(pgVec) * vert_num : 0;

    m_data_size = sizeof(ModelDataHeader) + node_info_size + prim_data_size + normal_data_size;
    m_data = static_cast<u8*>(pgMalloc(m_data_size));

    pgMemMgr::memset(m_data, 0, m_data_size);

    m_mdl_data_header = reinterpret_cast<ModelDataHeader*>(m_data);

    m_mdl_data_header->format_id = pgID_("POGOLYN_MODEL_DATA");
    m_mdl_data_header->format_version = MODEL_DATA_VERSION;
    m_mdl_data_header->node_num = node_num;
    m_mdl_data_header->vert_num = vert_num;
    m_mdl_data_header->flag.set(FLAG_HAS_NORMAL, has_normal);
    m_mdl_data_header->tex_id = tex_id;
    m_mdl_data_header->prim_data_offset = sizeof(ModelDataHeader) + node_info_size;
    m_mdl_data_header->normal_data_offset = m_mdl_data_header->prim_data_offset + prim_data_size;

    m_node_info = reinterpret_cast<NodeInfo*>(m_mdl_data_header + 1);
    m_prim_data = reinterpret_cast<pgPrim::PrimData*>(m_data + m_mdl_data_header->prim_data_offset);
    m_normal_data = reinterpret_cast<pgVec*>(m_data + m_mdl_data_header->normal_data_offset);

    for (s32 i = 0; i < node_num; i++)
    {
        m_node_info[i].local = pgMat::UNIT;
    }

    for (s32 i = 0; i < vert_num; i++)
    {
        m_prim_data[i].col = pgCol::FULL;

        if (has_normal)
        {
            m_normal_data[i] = pgVec::Z_UNIT;
        }
    }
}


pgMdlData::ModelDataMode pgMdlData::getMode() const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_mode.getType();
}


u16 pgMdlData::getNodeNum() const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_mdl_data_header->node_num;
}


u16 pgMdlData::getVertNum() const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_mdl_data_header->vert_num;
}


bool pgMdlData::hasNormal() const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_mdl_data_header->flag.isOn(FLAG_HAS_NORMAL);
}


pgID pgMdlData::getTextureID() const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_mdl_data_header->tex_id;
}


u16 pgMdlData::getNodeParentIndex(u16 node_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].parent_index;
}


void pgMdlData::setNodeParentIndex(u16 node_index, u16 parent_index)
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num || parent_index >= m_mdl_data_header->node_num || node_index == parent_index)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_node_info[node_index].parent_index = parent_index;
}


void pgMdlData::setNodeParentIndex_noParent(u16 node_index)
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_node_info[node_index].parent_index = node_index;
}


const pgMat& pgMdlData::getNodeLocal(u16 node_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].local;
}


void pgMdlData::setNodeLocal(u16 node_index, const pgMat& local)
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_node_info[node_index].local = local;
}


pgPrim::PrimMode pgMdlData::getNodePrimMode(u16 node_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].prim_mode.getType();
}


void pgMdlData::setNodePrimMode(u16 node_index, pgPrim::PrimMode prim_mode)
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_node_info[node_index].prim_mode = prim_mode;
}


pgPrim::BlendMode pgMdlData::getNodeBlendMode(u16 node_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].blend_mode.getType();
}


void pgMdlData::setNodeBlendMode(u16 node_index, pgPrim::BlendMode blend_mode)
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_node_info[node_index].blend_mode = blend_mode;
}


u16 pgMdlData::getNodeVertIndex(u16 node_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].vert_index;
}


u16 pgMdlData::getNodeVertNum(u16 node_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].vert_num;
}


void pgMdlData::setNodeVertInfo(u16 node_index, u16 vert_index, u16 vert_num)
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num || vert_index >= m_mdl_data_header->vert_num || vert_index + vert_num > m_mdl_data_header->vert_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    NodeInfo* node_info = &m_node_info[node_index];

    node_info->vert_index = vert_index;
    node_info->vert_num = vert_num;
}


const pgPrim::PrimData* pgMdlData::getNodePrimData(u16 node_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return &m_prim_data[m_node_info[node_index].vert_index];
}


const pgVec* pgMdlData::getNodeNormalData(u16 node_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mdl_data_header->flag.isOff(FLAG_HAS_NORMAL))
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return &m_normal_data[m_node_info[node_index].vert_index];
}


const pgVec& pgMdlData::getNodeClipBoundMinForReader(u16 node_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_WRITE)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].bound_min;
}


const pgVec& pgMdlData::getNodeClipBoundMaxForReader(u16 node_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_WRITE)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].bound_max;
}


const pgVec& pgMdlData::getVertPos(u16 vert_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[vert_index].pos;
}


void pgMdlData::setVertPos(u16 vert_index, const pgVec& pos)
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_prim_data[vert_index].pos = pos;
}


pgCol pgMdlData::getVertCol(u16 vert_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[vert_index].col;
}


void pgMdlData::setVertCol(u16 vert_index, pgCol col)
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_prim_data[vert_index].col = col;
}


r32 pgMdlData::getVertU(u16 vert_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[vert_index].u;
}


r32 pgMdlData::getVertV(u16 vert_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[vert_index].v;
}


void pgMdlData::setVertUV(u16 vert_index, r32 u, r32 v)
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgPrim::PrimData* prim_data = &m_prim_data[vert_index];

    prim_data->u = u;
    prim_data->v = v;
}


const pgVec& pgMdlData::getVertN(u16 vert_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mdl_data_header->flag.isOff(FLAG_HAS_NORMAL))
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_normal_data[vert_index];
}


void pgMdlData::setVertN(u16 vert_index, const pgVec& n)
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ || m_mdl_data_header->flag.isOff(FLAG_HAS_NORMAL))
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_normal_data[vert_index] = n;
}


void pgMdlData::calcNormalAsTriangles(bool is_smoothing)
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ || m_mdl_data_header->flag.isOff(FLAG_HAS_NORMAL))
    {
        pgThrow(ExceptionInvalidCall);
    }

    for (s32 i = 0; i < m_mdl_data_header->node_num; i++)
    {
        NodeInfo* node_info = &m_node_info[i];
        u16 vert_num = node_info->vert_num;

        if (vert_num > 0)
        {
            u16 vert_index = node_info->vert_index;

            pgUtil::calcNormalAsTriangles(&m_normal_data[vert_index], &m_prim_data[vert_index], vert_num, is_smoothing);
        }
    }
}


void pgMdlData::registerAsResource(pgID res_id)
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (res_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    for (s32 i = 0; i < m_mdl_data_header->node_num; i++)
    {
        NodeInfo* node_info = &m_node_info[i];

        pgPrim::PrimData* prim_data = &m_prim_data[node_info->vert_index];

        node_info->bound_min = node_info->bound_max = prim_data->pos;

        prim_data++;

        for (s32 j = 1; j < node_info->vert_num; j++)
        {
            if (prim_data->pos.x < node_info->bound_min.x)
            {
                node_info->bound_min.x = prim_data->pos.x;
            }

            if (prim_data->pos.y < node_info->bound_min.y)
            {
                node_info->bound_min.y = prim_data->pos.y;
            }

            if (prim_data->pos.z < node_info->bound_min.z)
            {
                node_info->bound_min.z = prim_data->pos.z;
            }

            if (prim_data->pos.x > node_info->bound_max.x)
            {
                node_info->bound_max.x = prim_data->pos.x;
            }

            if (prim_data->pos.y > node_info->bound_max.y)
            {
                node_info->bound_max.y = prim_data->pos.y;
            }

            if (prim_data->pos.z > node_info->bound_max.z)
            {
                node_info->bound_max.z = prim_data->pos.z;
            }

            prim_data++;
        }
    }

    pgResMgr::addResource(res_id, "", m_data, m_data_size, true);

    m_mode = MODE_READ;
}


PG_DEFINE_COPY_CONSTRUCTOR(pgMdlData)


PG_DEFINE_OPERATOR_EQUAL(pgMdlData)
