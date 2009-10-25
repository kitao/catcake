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


#include "pg_snd_all.h"

#include "pg_math_all.h"
#include "pg_sys_all.h"
#include "pg_low_level_api.h"


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


bool pgSndMgr::soundMixFunction(void* snd_mix_buf)
{
    pgSndMgr* ins = instance();
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

    pgLowLevelAPI::lockSoundMixMutex();
    {
        for (u32 i = 0; i < TRACK_NUM; i++)
        {
            TrackInfo* trk_info = &ins->m_trk_info[i];

            temp_trk_info[i] = *trk_info;

            if (trk_info->trk_state == STATE_STOP)
            {
                continue;
            }

            trk_info->play_pos += pgLowLevelAPI::getSoundMixBufferSampleNum() * //
                trk_info->snd_info->sample_rate.getValue() / pgLowLevelAPI::getSoundDeviceSampleRate();

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
    pgLowLevelAPI::unlockSoundMixMutex();

    pgMemMgr::memset(snd_mix_buf, 0, pgLowLevelAPI::getSoundMixBufferSize());

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
        u32 dest_sample_rate = pgLowLevelAPI::getSoundDeviceSampleRate();

        u32 src_sample_step = pgMath::max(src_sample_rate / dest_sample_rate, static_cast<u32>(1));
        u32 dest_sample_step = pgMath::max(dest_sample_rate / src_sample_rate, static_cast<u32>(1));

        u32 src_sample_num = trk_info->snd_info->sample_num;
        u32 dest_sample_num = pgLowLevelAPI::getSoundMixBufferSampleNum();

        u32 cur_src_sample = trk_info->play_pos;
        u32 cur_dest_sample = 0;

        u8 mix_vol = static_cast<u8>((trk_info->trk_vol * trk_info->snd_vol) / 255.0f);

        if (mix_vol == 0)
        {
            continue;
        }

        if (trk_info->snd_info->channel_num == CHANNEL_NUM_MONO)
        {
            if (pgLowLevelAPI::getSoundDeviceChannelNum() == 1)
            {
#undef INJECTION_CODE1
#define INJECTION_CODE1 \
    s32 src_amp = (pgReadLittleEndian(src, s16) * mix_vol) >> 8; \
    s32 dest_amp = pgMath::clamp(pgReadLittleEndian(dest, s16) + src_amp, MIN_AMPLITUDE, MAX_AMPLITUDE);

#undef INJECTION_CODE2
#define INJECTION_CODE2 pgWriteLittleEndian(dest, dest_amp, s16);

                SOUND_MIX_CODE(2, 2)
            }
            else
            {
#undef INJECTION_CODE1
#define INJECTION_CODE1 \
    s32 src_amp = (pgReadLittleEndian(src, s16) * mix_vol) >> 8; \
    s32 dest_amp = pgMath::clamp(pgReadLittleEndian(dest, s16) + src_amp, MIN_AMPLITUDE, MAX_AMPLITUDE);

#undef INJECTION_CODE2
#define INJECTION_CODE2 \
    pgWriteLittleEndian(dest + 0, dest_amp, s16); \
    pgWriteLittleEndian(dest + 2, dest_amp, s16);

                SOUND_MIX_CODE(2, 4)
            }
        }
        else
        {
            if (pgLowLevelAPI::getSoundDeviceChannelNum() == 1)
            {
#undef INJECTION_CODE1
#define INJECTION_CODE1 \
    s32 src_amp_l = (pgReadLittleEndian(src + 0, s16) * mix_vol) >> 8; \
    s32 src_amp_r = (pgReadLittleEndian(src + 2, s16) * mix_vol) >> 8; \
    s32 src_amp = (src_amp_l + src_amp_r) / 2; \
    s32 dest_amp = pgMath::clamp(pgReadLittleEndian(dest, s16) + src_amp, MIN_AMPLITUDE, MAX_AMPLITUDE);

#undef INJECTION_CODE2
#define INJECTION_CODE2 pgWriteLittleEndian(dest, dest_amp, s16);

                SOUND_MIX_CODE(4, 2)
            }
            else
            {
#undef INJECTION_CODE1
#define INJECTION_CODE1 \
    s32 src_amp_l = (pgReadLittleEndian(src + 0, s16) * mix_vol) >> 8; \
    s32 src_amp_r = (pgReadLittleEndian(src + 2, s16) * mix_vol) >> 8; \
    s32 dest_amp_l = pgMath::clamp(pgReadLittleEndian(dest + 0, s16) + src_amp_l, MIN_AMPLITUDE, MAX_AMPLITUDE); \
    s32 dest_amp_r = pgMath::clamp(pgReadLittleEndian(dest + 2, s16) + src_amp_r, MIN_AMPLITUDE, MAX_AMPLITUDE);

#undef INJECTION_CODE2
#define INJECTION_CODE2 \
    pgWriteLittleEndian(dest + 0, dest_amp_l, s16); \
    pgWriteLittleEndian(dest + 2, dest_amp_r, s16);

                SOUND_MIX_CODE(4, 4)
            }
        }
    }

    return true;
}
