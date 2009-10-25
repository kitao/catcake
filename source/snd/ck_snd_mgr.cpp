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
#include "pg_task_all.h"
#include "pg_res_all.h"
#include "pg_low_level_api.h"
#include "pg_private_macro.h"


pgSndMgr* pgSndMgr::m_instance = NULL;


PG_DEFINE_MANAGER_IS_CREATED(pgSndMgr)


PG_DEFINE_MANAGER_CREATE(pgSndMgr, AfterRes, BeforeRes)


PG_DEFINE_MANAGER_DESTROY(pgSndMgr, BeforeRes)


bool pgSndMgr::isSoundDeviceOpen()
{
    instance();

    return pgLowLevelAPI::isSoundDeviceOpen();
}


bool pgSndMgr::openSoundDevice(ChannelNum channel_num, SampleRate sample_rate, u16 snd_mix_buf_msec)
{
    instance();

    if (snd_mix_buf_msec == 0)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (pgLowLevelAPI::openSoundDevice(channel_num, sample_rate, snd_mix_buf_msec, soundMixFunction))
    {
        return true;
    }
    else
    {
        pgLowLevelAPI::printf("*** can't open sound device ***");

        return false;
    }
}


void pgSndMgr::closeSoundDevice()
{
    instance();

    if (pgLowLevelAPI::isSoundDeviceOpen())
    {
        pgLowLevelAPI::closeSoundDevice();
    }
}


pgSndMgr::ChannelNum pgSndMgr::getSoundDeviceChannelNum()
{
    instance();

    if (!pgLowLevelAPI::isSoundDeviceOpen())
    {
        return CHANNEL_NUM_MONO;
    }

    return static_cast<ChannelNum>(pgLowLevelAPI::getSoundDeviceChannelNum());
}


pgSndMgr::SampleRate pgSndMgr::getSoundDeviceSampleRate()
{
    instance();

    if (!pgLowLevelAPI::isSoundDeviceOpen())
    {
        return SAMPLE_RATE_11KHZ;
    }

    return static_cast<SampleRate>(pgLowLevelAPI::getSoundDeviceSampleRate());
}


u16 pgSndMgr::getSoundMixBufferMsec()
{
    instance();

    if (!pgLowLevelAPI::isSoundDeviceOpen())
    {
        return 0;
    }

    return pgLowLevelAPI::getSoundMixBufferMsec();
}


pgSndMgr::ChannelNum pgSndMgr::getChannelNum(pgID snd_id)
{
    instance();

    if (snd_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgRes res;

    pgTry
    {
        res = pgResMgr::getResource(snd_id);
    }
    pgCatch(pgResMgr::ExceptionNotFound)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (res.getExtension() != "WAV")
    {
        pgThrow(ExceptionInvalidType);
    }

    return res.getExInfo<SoundInfo>()->channel_num.getType();
}


pgSndMgr::SampleRate pgSndMgr::getSampleRate(pgID snd_id)
{
    instance();

    if (snd_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgRes res;

    pgTry
    {
        res = pgResMgr::getResource(snd_id);
    }
    pgCatch(pgResMgr::ExceptionNotFound)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (res.getExtension() != "WAV")
    {
        pgThrow(ExceptionInvalidType);
    }

    return res.getExInfo<SoundInfo>()->sample_rate.getType();
}


u32 pgSndMgr::getSampleNum(pgID snd_id)
{
    instance();

    if (snd_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgRes res;

    pgTry
    {
        res = pgResMgr::getResource(snd_id);
    }
    pgCatch(pgResMgr::ExceptionNotFound)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (res.getExtension() != "WAV")
    {
        pgThrow(ExceptionInvalidType);
    }

    return res.getExInfo<SoundInfo>()->sample_num;
}


u8 pgSndMgr::getTrackVolume(u8 trk_no)
{
    pgSndMgr* ins = instance();

    if (!pgLowLevelAPI::isSoundDeviceOpen())
    {
        return 0;
    }

    if (trk_no >= TRACK_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    u8 trk_vol;

    pgLowLevelAPI::lockSoundMixMutex();
    {
        trk_vol = static_cast<u8>(ins->m_trk_info[trk_no].trk_vol);
    }
    pgLowLevelAPI::unlockSoundMixMutex();

    return trk_vol;
}


void pgSndMgr::setTrackVolume(u8 trk_no, u8 trk_vol)
{
    pgSndMgr* ins = instance();

    if (!pgLowLevelAPI::isSoundDeviceOpen())
    {
        return;
    }

    if (trk_no >= TRACK_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgLowLevelAPI::lockSoundMixMutex();
    {
        ins->m_trk_info[trk_no].trk_vol = static_cast<u8>(trk_vol);
        ins->m_trk_info[trk_no].fade_frame_count = 0;
    }
    pgLowLevelAPI::unlockSoundMixMutex();
}


void pgSndMgr::fadeTrackVolume(u8 trk_no, u8 to_trk_vol, r32 delta_per_frame)
{
    pgSndMgr* ins = instance();

    if (!pgLowLevelAPI::isSoundDeviceOpen())
    {
        return;
    }

    if (trk_no >= TRACK_NUM || delta_per_frame < pgMath::EPSILON)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgLowLevelAPI::lockSoundMixMutex();
    {
        TrackInfo* trk_info = &ins->m_trk_info[trk_no];
        u32 fade_frame_count = static_cast<u32>(pgMath::abs<r32>(to_trk_vol - trk_info->trk_vol) / delta_per_frame);

        if (fade_frame_count > 0)
        {
            trk_info->fade_to_trk_vol = to_trk_vol;
            trk_info->fade_frame_count = fade_frame_count;
            trk_info->fade_delta_per_frame = (to_trk_vol > trk_info->trk_vol) ? delta_per_frame : -delta_per_frame;
        }
        else
        {
            trk_info->trk_vol = to_trk_vol;
        }
    }
    pgLowLevelAPI::unlockSoundMixMutex();
}


bool pgSndMgr::isPlaying(u8 trk_no)
{
    pgSndMgr* ins = instance();

    if (!pgLowLevelAPI::isSoundDeviceOpen())
    {
        return false;
    }

    if (trk_no >= TRACK_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    bool is_playing;

    pgLowLevelAPI::lockSoundMixMutex();
    {
        is_playing = (ins->m_trk_info[trk_no].trk_state != STATE_STOP);
    }
    pgLowLevelAPI::unlockSoundMixMutex();

    return is_playing;
}


void pgSndMgr::play(u8 trk_no, pgID snd_id, u8 snd_vol, bool is_play_loop, u32 play_pos)
{
    pgSndMgr* ins = instance();

    if (!pgLowLevelAPI::isSoundDeviceOpen())
    {
        return;
    }

    if (trk_no >= TRACK_NUM || snd_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgRes res;

    pgTry
    {
        res = pgResMgr::getResource(snd_id);
    }
    pgCatch(pgResMgr::ExceptionNotFound)
    {
        pgThrow(ExceptionNotFound);
    }

    if (res.getExtension() != "WAV")
    {
        pgThrow(ExceptionInvalidType);
    }

    SoundInfo* snd_info = res.getExInfo<SoundInfo>();

    if (play_pos >= snd_info->sample_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    TrackInfo* trk_info = &ins->m_trk_info[trk_no];

    pgLowLevelAPI::lockSoundMixMutex();
    {
        trk_info->trk_state = is_play_loop ? STATE_PLAY_LOOP : STATE_PLAY_ONESHOT;
        trk_info->snd_id = snd_id;
        trk_info->snd_vol = snd_vol;
        trk_info->snd_info = snd_info;
        trk_info->play_pos = play_pos;
    }
    pgLowLevelAPI::unlockSoundMixMutex();
}


void pgSndMgr::stop(u8 trk_no)
{
    pgSndMgr* ins = instance();

    if (!pgLowLevelAPI::isSoundDeviceOpen())
    {
        return;
    }

    if (trk_no >= TRACK_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgLowLevelAPI::lockSoundMixMutex();
    {
        ins->m_trk_info[trk_no].trk_state = STATE_STOP;
    }
    pgLowLevelAPI::unlockSoundMixMutex();
}


pgID pgSndMgr::getPlayingSoundID(u8 trk_no)
{
    pgSndMgr* ins = instance();

    if (!pgLowLevelAPI::isSoundDeviceOpen())
    {
        return pgID::ZERO;
    }

    if (trk_no >= TRACK_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (!isPlaying(trk_no))
    {
        pgThrow(ExceptionInvalidCall);
    }

    pgID snd_id;

    pgLowLevelAPI::lockSoundMixMutex();
    {
        snd_id = ins->m_trk_info[trk_no].snd_id;
    }
    pgLowLevelAPI::unlockSoundMixMutex();

    return snd_id;
}


u8 pgSndMgr::getPlayingSoundVolume(u8 trk_no)
{
    pgSndMgr* ins = instance();

    if (!pgLowLevelAPI::isSoundDeviceOpen())
    {
        return 0;
    }

    if (trk_no >= TRACK_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (!isPlaying(trk_no))
    {
        pgThrow(ExceptionInvalidCall);
    }

    u8 snd_vol;

    pgLowLevelAPI::lockSoundMixMutex();
    {
        snd_vol = ins->m_trk_info[trk_no].snd_vol;
    }
    pgLowLevelAPI::unlockSoundMixMutex();

    return snd_vol;
}


bool pgSndMgr::isPlayingLoop(u8 trk_no)
{
    pgSndMgr* ins = instance();

    if (!pgLowLevelAPI::isSoundDeviceOpen())
    {
        return false;
    }

    if (trk_no >= TRACK_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (!isPlaying(trk_no))
    {
        pgThrow(ExceptionInvalidCall);
    }

    bool is_playing_loop;

    pgLowLevelAPI::lockSoundMixMutex();
    {
        is_playing_loop = (ins->m_trk_info[trk_no].trk_state == STATE_PLAY_LOOP);
    }
    pgLowLevelAPI::unlockSoundMixMutex();

    return is_playing_loop;
}


u32 pgSndMgr::getPlayingPosition(u8 trk_no)
{
    pgSndMgr* ins = instance();

    if (!pgLowLevelAPI::isSoundDeviceOpen())
    {
        return 0;
    }

    if (trk_no >= TRACK_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (!isPlaying(trk_no))
    {
        pgThrow(ExceptionInvalidCall);
    }

    u32 play_pos;

    pgLowLevelAPI::lockSoundMixMutex();
    {
        play_pos = ins->m_trk_info[trk_no].play_pos;
    }
    pgLowLevelAPI::unlockSoundMixMutex();

    return play_pos;
}


class pgSndDaemon : public pgTask
{
public:
    pgSndDaemon() : pgTask(ORDER_PLUS_8_FOR_SYSTEM) {}

    virtual void onUpdate()
    {
        pgSndMgr* ins = pgSndMgr::instance();

        for (u32 i = 0; i < pgSndMgr::TRACK_NUM; i++)
        {
            pgSndMgr::TrackInfo* trk_info = &ins->m_trk_info[i];

            if (trk_info->fade_frame_count > 0)
            {
                trk_info->trk_vol = pgMath::clamp(trk_info->trk_vol + trk_info->fade_delta_per_frame, 0.0f, 255.0f);
                trk_info->fade_frame_count--;

                if (trk_info->fade_frame_count == 0)
                {
                    trk_info->trk_vol = trk_info->fade_to_trk_vol;
                }
            }
        }
    }
};


pgSndMgr::pgSndMgr()
{
    for (u32 i = 0; i < TRACK_NUM; i++)
    {
        TrackInfo* trk_info = &m_trk_info[i];

        trk_info->trk_state = STATE_STOP;
        trk_info->trk_vol = 192.0f;
        trk_info->fade_frame_count = 0;
    }

    m_snd_daemon = pgNewTask(pgSndDaemon);

    pgResMgr::addType("WAV", waveInitializer, waveFinalizer);
}


pgSndMgr::~pgSndMgr()
{
    if (pgTaskMgr::isCreated())
    {
        pgDeleteTask(m_snd_daemon);
    }

    pgLowLevelAPI::closeSoundDevice();

    for (const pgRes* res = pgResMgr::getFirstResourceN(); res; res = pgResMgr::getNextResourceN(res->getID()))
    {
        if (res->getExtension() == "WAV")
        {
            waveFinalizer(res->getID(), res->getExtension(), res->getData<void>(), res->getDataSize(), res->getExInfo<void>());
        }
    }

    pgResMgr::removeType("WAV");
}


PG_DEFINE_OPERATOR_EQUAL(pgSndMgr)


PG_DEFINE_MANAGER_INSTANCE(pgSndMgr)


void pgSndMgr::waveInitializer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    const u8* buf = static_cast<const u8*>(data);

    if (buf[0] != 'R' || buf[1] != 'I' || buf[2] != 'F' || buf[3] != 'F' || //
        buf[8] != 'W' || buf[9] != 'A' || buf[10] != 'V' || buf[11] != 'E')
    {
        pgThrow(ExceptionWaveInitializerFailed);
    }

    buf += 12;

    bool is_fmt_chunk = false;
    bool is_data_chunk = false;
    const u8* end = buf + data_size - 1;
    SoundInfo* snd_info = pgNew(SoundInfo);
    u16 bits_per_sample = 0;

    while (!is_fmt_chunk || !is_data_chunk)
    {
        u32 chunk_size = pgReadLittleEndian(buf + 4, u32);

        if (buf + 8 + chunk_size > end)
        {
            pgDelete(snd_info, SoundInfo);
            pgThrow(ExceptionWaveInitializerFailed);
        }

        if (buf[0] == 'f' && buf[1] == 'm' && buf[2] == 't' && buf[3] == ' ')
        {
            if (pgReadLittleEndian(buf + 8, u16) != 1)
            {
                pgDelete(snd_info, SoundInfo);
                pgThrow(ExceptionWaveInitializerFailed);
            }

            is_fmt_chunk = true;

            snd_info->channel_num = static_cast<ChannelNum>(pgReadLittleEndian(buf + 10, u16));
            snd_info->sample_rate = static_cast<SampleRate>(pgReadLittleEndian(buf + 12, u32));
            bits_per_sample = pgReadLittleEndian(buf + 22, u16);
        }
        else if (buf[0] == 'd' && buf[1] == 'a' && buf[2] == 't' && buf[3] == 'a')
        {
            is_data_chunk = true;

            snd_info->data = buf + 8;
            snd_info->data_size = chunk_size;
        }

        buf += 8 + chunk_size;
    }

    if ((snd_info->channel_num != CHANNEL_NUM_MONO && snd_info->channel_num != CHANNEL_NUM_STEREO) || //
        (snd_info->sample_rate != SAMPLE_RATE_11KHZ && snd_info->sample_rate != SAMPLE_RATE_22KHZ && snd_info->sample_rate != SAMPLE_RATE_44KHZ) || //
        bits_per_sample != 16 || snd_info->data_size == 0)
    {
        pgThrow(ExceptionWaveInitializerFailed);
    }

    snd_info->sample_num = snd_info->data_size / (snd_info->channel_num.getValue() * 2);

    *exinfo = snd_info;
}


void pgSndMgr::waveFinalizer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    pgDelete(exinfo, SoundInfo);
}
