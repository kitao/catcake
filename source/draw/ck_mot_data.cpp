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


pgMotData::pgMotData()
{
    m_data = NULL;
}


pgMotData::~pgMotData()
{
    if (m_data && m_mode == MODE_WRITE)
    {
        pgFree(m_data);
    }
}


void pgMotData::initAsReader(const void* data, u32 data_size)
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
    m_mot_data_header = reinterpret_cast<MotionDataHeader*>(m_data);
    m_mot_info = reinterpret_cast<MotionInfo*>(m_mot_data_header + 1);
    m_key_frame_info = reinterpret_cast<KeyFrameInfo*>(m_data + m_mot_data_header->key_frame_info_offset);
    m_node_info = reinterpret_cast<NodeInfo*>(m_data + m_mot_data_header->node_info_offset);

    if (m_mot_data_header->format_id != pgID_("POGOLYN_MOTION_DATA") || m_mot_data_header->format_version > MOTION_DATA_VERSION)
    {
        pgThrow(ExceptionInvalidData);
    }

    u32 valid_data_size = sizeof(MotionDataHeader) + sizeof(MotionInfo) * getMotionNum() + //
        sizeof(KeyFrameInfo) * getKeyFrameNum() + sizeof(NodeInfo) * getNodeNum() * getKeyFrameNum();

    if (m_data_size != valid_data_size)
    {
        pgThrow(ExceptionInvalidData);
    }
}


void pgMotData::initAsWriter(u16 node_num, u16 mot_num, u32 key_frame_num)
{
    if (node_num == 0 || mot_num == 0 || key_frame_num == 0)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (m_data && m_mode == MODE_WRITE)
    {
        pgFree(m_data);
    }

    m_mode = MODE_WRITE;

    u32 node_info_num = node_num * key_frame_num;

    u32 mot_info_size = sizeof(MotionInfo) * mot_num;
    u32 key_frame_info_size = sizeof(KeyFrameInfo) * key_frame_num;
    u32 node_info_size = sizeof(NodeInfo) * node_info_num;

    m_data_size = sizeof(MotionDataHeader) + mot_info_size + key_frame_info_size + node_info_size;
    m_data = static_cast<u8*>(pgMalloc(m_data_size));

    pgMemMgr::memset(m_data, 0, m_data_size);

    m_mot_data_header = reinterpret_cast<MotionDataHeader*>(m_data);

    m_mot_data_header->format_id = pgID_("POGOLYN_MOTION_DATA");
    m_mot_data_header->format_version = MOTION_DATA_VERSION;
    m_mot_data_header->node_num = node_num;
    m_mot_data_header->mot_num = mot_num;
    m_mot_data_header->key_frame_num = key_frame_num;
    m_mot_data_header->key_frame_info_offset = sizeof(MotionDataHeader) + mot_info_size;
    m_mot_data_header->node_info_offset = m_mot_data_header->key_frame_info_offset + key_frame_info_size;

    m_mot_info = reinterpret_cast<MotionInfo*>(m_mot_data_header + 1);
    m_key_frame_info = reinterpret_cast<KeyFrameInfo*>(m_data + m_mot_data_header->key_frame_info_offset);
    m_node_info = reinterpret_cast<NodeInfo*>(m_data + m_mot_data_header->node_info_offset);

    for (s32 i = 0; i < mot_num; i++)
    {
        m_mot_info[i].frame_num = 1;
        m_mot_info[i].key_frame_num = 1;
    }

    for (u32 i = 0; i < node_info_num; i++)
    {
        m_node_info[i].m_quat.w = 1.0f;
    }
}


pgMotData::MotionDataMode pgMotData::getMode() const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_mode.getType();
}


u16 pgMotData::getNodeNum() const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_mot_data_header->node_num;
}


u16 pgMotData::getMotionNum() const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_mot_data_header->mot_num;
}


u32 pgMotData::getKeyFrameNum() const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_mot_data_header->key_frame_num;
}


u32 pgMotData::getMotionFrameNum(u16 mot_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (mot_index >= m_mot_data_header->mot_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_mot_info[mot_index].frame_num;
}


u32 pgMotData::getMotionKeyFrameIndex(u16 mot_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (mot_index >= m_mot_data_header->mot_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_mot_info[mot_index].key_frame_index;
}


u32 pgMotData::getMotionKeyFrameNum(u16 mot_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (mot_index >= m_mot_data_header->mot_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_mot_info[mot_index].key_frame_num;
}


void pgMotData::setMotionInfo(u16 mot_index, u32 frame_num, u32 key_frame_index, u32 key_frame_num)
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (mot_index >= m_mot_data_header->mot_num || frame_num == 0 || key_frame_num == 0 || //
        key_frame_index + key_frame_num > m_mot_data_header->key_frame_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    MotionInfo* mot_info = &m_mot_info[mot_index];

    mot_info->frame_num = frame_num;
    mot_info->key_frame_index = key_frame_index;
    mot_info->key_frame_num = key_frame_num;
}


u32 pgMotData::getKeyFramePlayFrame(u32 key_frame_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (key_frame_index >= m_mot_data_header->key_frame_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_key_frame_info[key_frame_index].play_frame;
}


void pgMotData::setKeyFramePlayFrame(u32 key_frame_index, u32 play_frame)
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (key_frame_index >= m_mot_data_header->key_frame_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_key_frame_info[key_frame_index].play_frame = play_frame;
}


const pgQuat& pgMotData::getNodeRotate(u16 node_index, u32 key_frame_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mot_data_header->node_num || key_frame_index >= m_mot_data_header->key_frame_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_node_info[m_mot_data_header->key_frame_num * node_index + key_frame_index].m_quat;
}


const pgVec& pgMotData::getNodeTrans(u16 node_index, u32 key_frame_index) const
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mot_data_header->node_num || key_frame_index >= m_mot_data_header->key_frame_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_node_info[m_mot_data_header->key_frame_num * node_index + key_frame_index].m_trans;
}


void pgMotData::setNodeLocal(u16 node_index, u32 key_frame_index, const pgMat& local)
{
    if (!m_data)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mot_data_header->node_num || key_frame_index >= m_mot_data_header->key_frame_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    NodeInfo* joint_info = &m_node_info[m_mot_data_header->key_frame_num * node_index + key_frame_index];

    joint_info->m_quat = pgQuat::fromMat(local);
    joint_info->m_trans = local.trans;
}


void pgMotData::registerAsResource(pgID res_id)
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

    pgResMgr::addResource(res_id, "", m_data, m_data_size, true);

    m_mode = MODE_READ;
}


PG_DEFINE_COPY_CONSTRUCTOR(pgMotData)


PG_DEFINE_OPERATOR_EQUAL(pgMotData)
