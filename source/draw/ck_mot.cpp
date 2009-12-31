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
#include "ck_private_macro.h"


ckMot::ckMot()
{
    m_interp_info = NULL;
}


ckMot::~ckMot()
{
    uninit();
}


void ckMot::init(ckMdl* mdl, ckID mot_data_id)
{
    uninit();

    if (mot_data_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckRes res;

    ckTry
    {
        res = ckResMgr::getResource(mot_data_id);
    }
    ckCatch(ckResMgr::ExceptionNotFound)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_mot_data.initAsReader(res.getData<void>(), res.getDataSize());

    ckNewArray(m_interp_info, InterpInfo, m_mot_data.getNodeNum());

    m_play_mode = PLAY_NORMAL;
    m_mot_index = 0;
    m_interp_frame = 0;
    m_play_speed = 1.0f;
    m_playing_frame = -1.0f;
    m_interp_speed = 0.0f;
    m_interp_ratio = 0.0f;
}


u16 ckMot::getMotionIndex() const
{
    if (!m_interp_info)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_mot_index;
}


ckMot::PlayMode ckMot::getPlayMode() const
{
    if (!m_interp_info)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_play_mode.getType();
}


r32 ckMot::getPlaySpeed() const
{
    if (!m_interp_info)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_play_speed;
}


u16 ckMot::getInterpFrame() const
{
    if (!m_interp_info)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_interp_frame;
}


bool ckMot::isPlaying() const
{
    if (!m_interp_info)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return (m_playing_frame >= 0.0f);
}


void ckMot::play(ckMdl* mdl, u16 mot_index, PlayMode play_mode, r32 play_speed, u16 interp_frame)
{
    if (!m_interp_info)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (!mdl || mot_index >= m_mot_data.getMotionNum())
    {
        ckThrow(ExceptionInvalidArgument);
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

        const ckMdlData* mdl_data = mdl->getModelData();
        u16 node_num = ckMath::min(m_mot_data.getNodeNum(), mdl_data->getNodeNum());

        for (s32 i = 0; i < node_num; i++)
        {
            InterpInfo* interp_info = &m_interp_info[i];
            ckMat local = mdl->getNodeDraw(i)->local().toLocalOf(mdl_data->getNodeLocal(i));

            interp_info->quat = ckQuat::fromMat(local);
            interp_info->trans = local.trans;
        }
    }
    else
    {
        m_interp_speed = 0.0f;
        m_interp_ratio = 0.0f;
    }
}


void ckMot::stop()
{
    if (!m_interp_info)
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_playing_frame = -1.0f;
}


r32 ckMot::getNextUpdateFrame() const
{
    if (!m_interp_info)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return (m_playing_frame < 0.0f) ? 0.0f : m_playing_frame;
}


void ckMot::update(ckMdl* mdl)
{
    if (!m_interp_info)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (!mdl)
    {
        ckThrow(ExceptionInvalidArgument);
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
    const ckMdlData* mdl_data = mdl->getModelData();
    u16 node_num = ckMath::min(m_mot_data.getNodeNum(), mdl_data->getNodeNum());

    for (s32 i = 0; i < node_num; i++)
    {
        ckQuat quat = m_mot_data.getNodeRotate(i, key_frame_index);
        ckVec trans = m_mot_data.getNodeTrans(i, key_frame_index);

        if (is_blend)
        {
            ckQuat quat2 = m_mot_data.getNodeRotate(i, key_frame_index + 1); // to avoid runtime error on iPhone
            ckVec trans2 = m_mot_data.getNodeTrans(i, key_frame_index + 1); // to avoid runtime error on iPhone

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


CK_DEFINE_COPY_CONSTRUCTOR(ckMot)


CK_DEFINE_OPERATOR_EQUAL(ckMot)


void ckMot::uninit()
{
    if (m_interp_info)
    {
        ckDeleteArray(m_interp_info, InterpInfo);

        m_interp_info = NULL;
    }
}
