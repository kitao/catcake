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


pgMot::pgMot()
{
    m_interp_info = NULL;
}


pgMot::~pgMot()
{
    uninit();
}


void pgMot::init(pgMdl* mdl, pgID mot_data_id)
{
    uninit();

    if (mot_data_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgRes res;

    pgTry
    {
        res = pgResMgr::getResource(mot_data_id);
    }
    pgCatch(pgResMgr::ExceptionNotFound)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_mot_data.initAsReader(res.getData<void>(), res.getDataSize());

    pgNewArray(m_interp_info, InterpInfo, m_mot_data.getNodeNum());

    m_play_mode = PLAY_NORMAL;
    m_mot_index = 0;
    m_interp_frame = 0;
    m_play_speed = 1.0f;
    m_playing_frame = -1.0f;
    m_interp_speed = 0.0f;
    m_interp_ratio = 0.0f;
}


u16 pgMot::getMotionIndex() const
{
    if (!m_interp_info)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_mot_index;
}


pgMot::PlayMode pgMot::getPlayMode() const
{
    if (!m_interp_info)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_play_mode.getType();
}


r32 pgMot::getPlaySpeed() const
{
    if (!m_interp_info)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_play_speed;
}


u16 pgMot::getInterpFrame() const
{
    if (!m_interp_info)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_interp_frame;
}


bool pgMot::isPlaying() const
{
    if (!m_interp_info)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return (m_playing_frame >= 0.0f);
}


void pgMot::play(pgMdl* mdl, u16 mot_index, PlayMode play_mode, r32 play_speed, u16 interp_frame)
{
    if (!m_interp_info)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (!mdl || mot_index >= m_mot_data.getMotionNum())
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_play_mode = play_mode;
    m_play_speed = play_speed;
    m_mot_index = mot_index;
    m_interp_frame = interp_frame;
    m_playing_frame = 0.0f;

    if (interp_frame > 0)
    {
        m_interp_speed = 1.0f / interp_frame;
        m_interp_ratio = 1.0f;

        const pgMdlData* mdl_data = mdl->getModelData();
        u16 node_num = pgMath::min(m_mot_data.getNodeNum(), mdl_data->getNodeNum());

        for (s32 i = 0; i < node_num; i++)
        {
            InterpInfo* interp_info = &m_interp_info[i];
            pgMat local = mdl->getNodeDraw(i)->local().toLocalOf(mdl_data->getNodeLocal(i));

            interp_info->quat = pgQuat::fromMat(local);
            interp_info->trans = local.trans;
        }
    }
    else
    {
        m_interp_speed = 0.0f;
        m_interp_ratio = 0.0f;
    }
}


void pgMot::stop()
{
    if (!m_interp_info)
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_playing_frame = -1.0f;
}


r32 pgMot::getNextUpdateFrame() const
{
    if (!m_interp_info)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return (m_playing_frame < 0.0f) ? 0.0f : m_playing_frame;
}


void pgMot::update(pgMdl* mdl)
{
    if (!m_interp_info)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (!mdl)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (!isPlaying())
    {
        return;
    }

    /*
        find current key_frame_index
    */
    u32 key_frame_num = m_mot_data.getMotionKeyFrameNum(m_mot_index);
    u32 first_key_frame_index = m_mot_data.getMotionKeyFrameIndex(m_mot_index);
    u32 last_key_frame_index = first_key_frame_index + key_frame_num - 1;

    u32 key_frame_index;

    for (key_frame_index = last_key_frame_index; key_frame_index > first_key_frame_index; key_frame_index--)
    {
        if (m_playing_frame >= m_mot_data.getKeyFramePlayFrame(key_frame_index))
        {
            break;
        }
    }

    /*
        calculate blend ratio
    */
    u32 cur_key_frame_play_frame = m_mot_data.getKeyFramePlayFrame(key_frame_index);

    r32 blend_ratio = (key_frame_index >= last_key_frame_index) ? 0.0f : //
        (m_playing_frame - cur_key_frame_play_frame) / (m_mot_data.getKeyFramePlayFrame(key_frame_index + 1) - cur_key_frame_play_frame);

    bool is_blend = (blend_ratio > 0.0f);

    /*
        update interpolate ratio
    */
    if (m_interp_ratio > 0.0f)
    {
        m_interp_ratio -= m_interp_speed * m_play_speed;

        if (m_interp_ratio < 0.0f)
        {
            m_interp_ratio = 0.0f;
        }
    }

    bool is_interp = (m_interp_ratio > 0.0f);

    /*
        update model's local matrix
    */
    const pgMdlData* mdl_data = mdl->getModelData();
    u16 node_num = pgMath::min(m_mot_data.getNodeNum(), mdl_data->getNodeNum());

    for (s32 i = 0; i < node_num; i++)
    {
        pgQuat quat = m_mot_data.getNodeRotate(i, key_frame_index);
        pgVec trans = m_mot_data.getNodeTrans(i, key_frame_index);

        if (is_blend)
        {
            pgQuat quat2 = m_mot_data.getNodeRotate(i, key_frame_index + 1); // to avoid runtime error on iPhone
            pgVec trans2 = m_mot_data.getNodeTrans(i, key_frame_index + 1); // to avoid runtime error on iPhone

            quat = quat.slerp(quat2, blend_ratio);
            trans = trans.interp(trans2, blend_ratio);
        }

        if (is_interp)
        {
            InterpInfo* interp_info = &m_interp_info[i];

            quat = quat.slerp(interp_info->quat, m_interp_ratio);
            trans = trans.interp(interp_info->trans, m_interp_ratio);
        }

        mdl->getNodeDraw(i)->local() = quat.toMat(trans).toGlobalFrom(mdl_data->getNodeLocal(i));
    }

    /*
        update current frame
    */
    r32 frame_num = static_cast<r32>(m_mot_data.getMotionFrameNum(m_mot_index));
    bool is_frame_remaining = (m_playing_frame < frame_num);

    m_playing_frame += m_play_speed;

    if (m_playing_frame > frame_num)
    {
        if (m_play_mode == PLAY_LOOP)
        {
            m_playing_frame -= frame_num;
        }
        else if (m_play_mode == PLAY_KEEP || is_frame_remaining)
        {
            m_playing_frame = frame_num;
        }
        else
        {
            m_playing_frame = -1.0f;
        }
    }
}


PG_DEFINE_COPY_CONSTRUCTOR(pgMot)


PG_DEFINE_OPERATOR_EQUAL(pgMot)


void pgMot::uninit()
{
    if (m_interp_info)
    {
        pgDeleteArray(m_interp_info, InterpInfo);

        m_interp_info = NULL;
    }
}
