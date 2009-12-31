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


#include "ck_snd_all.h"

#include "ck_math_all.h"
#include "ck_sys_all.h"
#include "ck_low_level_api.h"


const s32 MIN_AMPLITUDE = -32765;
const s32 MAX_AMPLITUDE = 32765;


#define SOUND_MIX_CODE(src_sample_size, dest_sample_size) \
    while (cur_dest_sample < dest_sample_num) \
    { \
        if (cur_src_sample >= src_sample_num) \
        { \
            if (trk_info->trk_state == STATE_PLAY_LOOP) \
            { \
                cur_src_sample = 0; \
                src = static_cast<const u8*>(trk_info->snd_info->data); \
            } \
            else \
            { \
                break; \
            } \
        } \
    \
        INJECTION_CODE1 \
    \
        for (u32 i = 0; i < dest_sample_step && cur_dest_sample < dest_sample_num; i++) \
        { \
            INJECTION_CODE2 \
    \
            cur_dest_sample++; \
            dest += dest_sample_size; \
        } \
    \
        cur_src_sample += src_sample_step; \
        src += src_sample_step * src_sample_size; \
    }


bool ckSndMgr::soundMixFunction(void* snd_mix_buf)
{
    ckSndMgr* ins = instance();
    bool is_playing = false;

    for (u32 i = 0; i < TRACK_NUM; i++)
    {
        if (ins->m_trk_info[i].trk_state != STATE_STOP)
        {
            is_playing = true;
            break;
        }
    }

    if (!is_playing)
    {
        return false;
    }

    TrackInfo temp_trk_info[TRACK_NUM];

    ckLowLevelAPI::lockSoundMixMutex();
    {
        for (u32 i = 0; i < TRACK_NUM; i++)
        {
            TrackInfo* trk_info = &ins->m_trk_info[i];

            temp_trk_info[i] = *trk_info;

            if (trk_info->trk_state == STATE_STOP)
            {
                continue;
            }

            trk_info->play_pos += ckLowLevelAPI::getSoundMixBufferSampleNum() * //
                trk_info->snd_info->sample_rate.getValue() / ckLowLevelAPI::getSoundDeviceSampleRate();

            if (trk_info->play_pos >= trk_info->snd_info->sample_num)
            {
                if (trk_info->trk_state == STATE_PLAY_ONESHOT)
                {
                    trk_info->trk_state = STATE_STOP;
                    trk_info->play_pos = trk_info->snd_info->sample_num;
                }
                else // STATE_PLAY_LOOP
                {
                    trk_info->play_pos = trk_info->play_pos % trk_info->snd_info->sample_num;
                }
            }
        }
    }
    ckLowLevelAPI::unlockSoundMixMutex();

    ckMemMgr::memset(snd_mix_buf, 0, ckLowLevelAPI::getSoundMixBufferSize());

    for (u32 i = 0; i < TRACK_NUM; i++)
    {
        TrackInfo* trk_info = &temp_trk_info[i];

        if (trk_info->trk_state == STATE_STOP)
        {
            continue;
        }

        const u8* src = static_cast<const u8*>(trk_info->snd_info->data) + trk_info->snd_info->channel_num.getValue() * 2 * trk_info->play_pos;
        u8* dest = static_cast<u8*>(snd_mix_buf);

        u32 src_sample_rate = trk_info->snd_info->sample_rate.getValue();
        u32 dest_sample_rate = ckLowLevelAPI::getSoundDeviceSampleRate();

        u32 src_sample_step = ckMath::max(src_sample_rate / dest_sample_rate, static_cast<u32>(1));
        u32 dest_sample_step = ckMath::max(dest_sample_rate / src_sample_rate, static_cast<u32>(1));

        u32 src_sample_num = trk_info->snd_info->sample_num;
        u32 dest_sample_num = ckLowLevelAPI::getSoundMixBufferSampleNum();

        u32 cur_src_sample = trk_info->play_pos;
        u32 cur_dest_sample = 0;

        u8 mix_vol = static_cast<u8>((trk_info->trk_vol * trk_info->snd_vol) / 255.0f);

        if (mix_vol == 0)
        {
            continue;
        }

        if (trk_info->snd_info->channel_num == CHANNEL_NUM_MONO)
        {
            if (ckLowLevelAPI::getSoundDeviceChannelNum() == 1)
            {
#undef INJECTION_CODE1
#define INJECTION_CODE1 \
    s32 src_amp = (ckReadLittleEndian(src, s16) * mix_vol) >> 8; \
    s32 dest_amp = ckMath::clamp(ckReadLittleEndian(dest, s16) + src_amp, MIN_AMPLITUDE, MAX_AMPLITUDE);

#undef INJECTION_CODE2
#define INJECTION_CODE2 ckWriteLittleEndian(dest, dest_amp, s16);

                SOUND_MIX_CODE(2, 2)
            }
            else
            {
#undef INJECTION_CODE1
#define INJECTION_CODE1 \
    s32 src_amp = (ckReadLittleEndian(src, s16) * mix_vol) >> 8; \
    s32 dest_amp = ckMath::clamp(ckReadLittleEndian(dest, s16) + src_amp, MIN_AMPLITUDE, MAX_AMPLITUDE);

#undef INJECTION_CODE2
#define INJECTION_CODE2 \
    ckWriteLittleEndian(dest + 0, dest_amp, s16); \
    ckWriteLittleEndian(dest + 2, dest_amp, s16);

                SOUND_MIX_CODE(2, 4)
            }
        }
        else
        {
            if (ckLowLevelAPI::getSoundDeviceChannelNum() == 1)
            {
#undef INJECTION_CODE1
#define INJECTION_CODE1 \
    s32 src_amp_l = (ckReadLittleEndian(src + 0, s16) * mix_vol) >> 8; \
    s32 src_amp_r = (ckReadLittleEndian(src + 2, s16) * mix_vol) >> 8; \
    s32 src_amp = (src_amp_l + src_amp_r) / 2; \
    s32 dest_amp = ckMath::clamp(ckReadLittleEndian(dest, s16) + src_amp, MIN_AMPLITUDE, MAX_AMPLITUDE);

#undef INJECTION_CODE2
#define INJECTION_CODE2 ckWriteLittleEndian(dest, dest_amp, s16);

                SOUND_MIX_CODE(4, 2)
            }
            else
            {
#undef INJECTION_CODE1
#define INJECTION_CODE1 \
    s32 src_amp_l = (ckReadLittleEndian(src + 0, s16) * mix_vol) >> 8; \
    s32 src_amp_r = (ckReadLittleEndian(src + 2, s16) * mix_vol) >> 8; \
    s32 dest_amp_l = ckMath::clamp(ckReadLittleEndian(dest + 0, s16) + src_amp_l, MIN_AMPLITUDE, MAX_AMPLITUDE); \
    s32 dest_amp_r = ckMath::clamp(ckReadLittleEndian(dest + 2, s16) + src_amp_r, MIN_AMPLITUDE, MAX_AMPLITUDE);

#undef INJECTION_CODE2
#define INJECTION_CODE2 \
    ckWriteLittleEndian(dest + 0, dest_amp_l, s16); \
    ckWriteLittleEndian(dest + 2, dest_amp_r, s16);

                SOUND_MIX_CODE(4, 4)
            }
        }
    }

    return true;
}
