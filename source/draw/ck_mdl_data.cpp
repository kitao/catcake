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

#include "ck_res_all.h"
#include "ck_util_all.h"
#include "ck_private_macro.h"


ckMdlData::ckMdlData()
{
    m_data = NULL;
}


ckMdlData::~ckMdlData()
{
    if (m_data && m_mode == MODE_WRITE)
    {
        ckFree(m_data);
    }
}


void ckMdlData::initAsReader(const void* data, u32 data_size)
{
    if (!data || data_size == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (m_data && m_mode == MODE_WRITE)
    {
        ckFree(m_data);
    }

    m_mode = MODE_READ;
    m_data = reinterpret_cast<u8*>(const_cast<void*>(data));
    m_data_size = data_size;
    m_mdl_data_header = reinterpret_cast<ModelDataHeader*>(m_data);
    m_node_info = reinterpret_cast<NodeInfo*>(m_mdl_data_header + 1);
    m_prim_data = reinterpret_cast<ckPrim::PrimData*>(m_data + m_mdl_data_header->prim_data_offset);
    m_normal_data = reinterpret_cast<ckVec*>(m_data + m_mdl_data_header->normal_data_offset);

    if (m_mdl_data_header->format_id != ckID_("CATCAKE_MODEL_DATA") || m_mdl_data_header->format_version > MODEL_DATA_VERSION)
    {
        ckThrow(ExceptionInvalidData);
    }

    u32 valid_data_size = sizeof(ModelDataHeader) + sizeof(NodeInfo) * getNodeNum() + (sizeof(ckPrim::PrimData) + (hasNormal() ? sizeof(ckVec) : 0)) * getVertNum();

    if (m_data_size != valid_data_size)
    {
        ckThrow(ExceptionInvalidData);
    }
}


void ckMdlData::initAsWriter(u16 node_num, u16 vert_num, ckID tex_id, bool has_normal)
{
    if (node_num == 0 || vert_num == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (m_data && m_mode == MODE_WRITE)
    {
        ckFree(m_data);
    }

    m_mode = MODE_WRITE;

    u32 node_info_size = sizeof(NodeInfo) * node_num;
    u32 prim_data_size = sizeof(ckPrim::PrimData) * vert_num;
    u32 normal_data_size = has_normal ? sizeof(ckVec) * vert_num : 0;

    m_data_size = sizeof(ModelDataHeader) + node_info_size + prim_data_size + normal_data_size;
    m_data = static_cast<u8*>(ckMalloc(m_data_size));

    ckMemMgr::memset(m_data, 0, m_data_size);

    m_mdl_data_header = reinterpret_cast<ModelDataHeader*>(m_data);

    m_mdl_data_header->format_id = ckID_("CATCAKE_MODEL_DATA");
    m_mdl_data_header->format_version = MODEL_DATA_VERSION;
    m_mdl_data_header->node_num = node_num;
    m_mdl_data_header->vert_num = vert_num;
    m_mdl_data_header->flag.set(FLAG_HAS_NORMAL, has_normal);
    m_mdl_data_header->tex_id = tex_id;
    m_mdl_data_header->prim_data_offset = sizeof(ModelDataHeader) + node_info_size;
    m_mdl_data_header->normal_data_offset = m_mdl_data_header->prim_data_offset + prim_data_size;

    m_node_info = reinterpret_cast<NodeInfo*>(m_mdl_data_header + 1);
    m_prim_data = reinterpret_cast<ckPrim::PrimData*>(m_data + m_mdl_data_header->prim_data_offset);
    m_normal_data = reinterpret_cast<ckVec*>(m_data + m_mdl_data_header->normal_data_offset);

    for (s32 i = 0; i < node_num; i++)
    {
        m_node_info[i].local = ckMat::UNIT;
    }

    for (s32 i = 0; i < vert_num; i++)
    {
        m_prim_data[i].col = ckCol::FULL;

        if (has_normal)
        {
            m_normal_data[i] = ckVec::Z_UNIT;
        }
    }
}


ckMdlData::ModelDataMode ckMdlData::getMode() const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_mode.getType();
}


u16 ckMdlData::getNodeNum() const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_mdl_data_header->node_num;
}


u16 ckMdlData::getVertNum() const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_mdl_data_header->vert_num;
}


bool ckMdlData::hasNormal() const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_mdl_data_header->flag.isOn(FLAG_HAS_NORMAL);
}


ckID ckMdlData::getTextureID() const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_mdl_data_header->tex_id;
}


u16 ckMdlData::getNodeParentIndex(u16 node_index) const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].parent_index;
}


void ckMdlData::setNodeParentIndex(u16 node_index, u16 parent_index)
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num || parent_index >= m_mdl_data_header->node_num || node_index == parent_index)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_node_info[node_index].parent_index = parent_index;
}


void ckMdlData::setNodeParentIndex_noParent(u16 node_index)
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_node_info[node_index].parent_index = node_index;
}


const ckMat& ckMdlData::getNodeLocal(u16 node_index) const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].local;
}


void ckMdlData::setNodeLocal(u16 node_index, const ckMat& local)
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_node_info[node_index].local = local;
}


ckPrim::PrimMode ckMdlData::getNodePrimMode(u16 node_index) const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].prim_mode.getType();
}


void ckMdlData::setNodePrimMode(u16 node_index, ckPrim::PrimMode prim_mode)
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_node_info[node_index].prim_mode = prim_mode;
}


ckPrim::BlendMode ckMdlData::getNodeBlendMode(u16 node_index) const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].blend_mode.getType();
}


void ckMdlData::setNodeBlendMode(u16 node_index, ckPrim::BlendMode blend_mode)
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_node_info[node_index].blend_mode = blend_mode;
}


u16 ckMdlData::getNodeVertIndex(u16 node_index) const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].vert_index;
}


u16 ckMdlData::getNodeVertNum(u16 node_index) const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].vert_num;
}


void ckMdlData::setNodeVertInfo(u16 node_index, u16 vert_index, u16 vert_num)
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num || vert_index >= m_mdl_data_header->vert_num || vert_index + vert_num > m_mdl_data_header->vert_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    NodeInfo* node_info = &m_node_info[node_index];

    node_info->vert_index = vert_index;
    node_info->vert_num = vert_num;
}


const ckPrim::PrimData* ckMdlData::getNodePrimData(u16 node_index) const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return &m_prim_data[m_node_info[node_index].vert_index];
}


const ckVec* ckMdlData::getNodeNormalData(u16 node_index) const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_mdl_data_header->flag.isOff(FLAG_HAS_NORMAL))
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return &m_normal_data[m_node_info[node_index].vert_index];
}


const ckVec& ckMdlData::getNodeClipBoundMinForReader(u16 node_index) const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_WRITE)
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].bound_min;
}


const ckVec& ckMdlData::getNodeClipBoundMaxForReader(u16 node_index) const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_WRITE)
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].bound_max;
}


const ckVec& ckMdlData::getVertPos(u16 vert_index) const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[vert_index].pos;
}


void ckMdlData::setVertPos(u16 vert_index, const ckVec& pos)
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_prim_data[vert_index].pos = pos;
}


ckCol ckMdlData::getVertCol(u16 vert_index) const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[vert_index].col;
}


void ckMdlData::setVertCol(u16 vert_index, ckCol col)
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_prim_data[vert_index].col = col;
}


r32 ckMdlData::getVertU(u16 vert_index) const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[vert_index].u;
}


r32 ckMdlData::getVertV(u16 vert_index) const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[vert_index].v;
}


void ckMdlData::setVertUV(u16 vert_index, r32 u, r32 v)
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckPrim::PrimData* prim_data = &m_prim_data[vert_index];

    prim_data->u = u;
    prim_data->v = v;
}


const ckVec& ckMdlData::getVertN(u16 vert_index) const
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_mdl_data_header->flag.isOff(FLAG_HAS_NORMAL))
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_normal_data[vert_index];
}


void ckMdlData::setVertN(u16 vert_index, const ckVec& n)
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ || m_mdl_data_header->flag.isOff(FLAG_HAS_NORMAL))
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_normal_data[vert_index] = n;
}


void ckMdlData::calcNormalAsTriangles(bool is_smoothing)
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ || m_mdl_data_header->flag.isOff(FLAG_HAS_NORMAL))
    {
        ckThrow(ExceptionInvalidCall);
    }

    for (s32 i = 0; i < m_mdl_data_header->node_num; i++)
    {
        NodeInfo* node_info = &m_node_info[i];
        u16 vert_num = node_info->vert_num;

        if (vert_num > 0)
        {
            u16 vert_index = node_info->vert_index;

            ckUtil::calcNormalAsTriangles(&m_normal_data[vert_index], &m_prim_data[vert_index], vert_num, is_smoothing);
        }
    }
}


void ckMdlData::registerAsResource(ckID res_id)
{
    if (!m_data)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (res_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    for (s32 i = 0; i < m_mdl_data_header->node_num; i++)
    {
        NodeInfo* node_info = &m_node_info[i];

        ckPrim::PrimData* prim_data = &m_prim_data[node_info->vert_index];

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

    ckResMgr::addResource(res_id, "", m_data, m_data_size, true);

    m_mode = MODE_READ;
}


CK_DEFINE_COPY_CONSTRUCTOR(ckMdlData)


CK_DEFINE_OPERATOR_EQUAL(ckMdlData)
