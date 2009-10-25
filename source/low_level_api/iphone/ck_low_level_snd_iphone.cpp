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


#ifdef PG_IPHONE


#include "pg_low_level_api.h"

#include "pg_mem_all.h"


static bool s_is_snd_dev_open = false;
// static bool s_is_playing;
static u8 s_channel_num;
static u16 s_sample_rate;
static u16 s_snd_mix_buf_msec;
static u32 s_snd_mix_buf_size;
static u32 s_snd_mix_buf_sample_num;


bool pgLowLevelAPI::isSoundDeviceOpen()
{
    // TODO

    return s_is_snd_dev_open;
}


bool pgLowLevelAPI::openSoundDevice(u8 channel_num, u16 sample_rate, u16 snd_mix_buf_msec, SoundMixFunction snd_mix_func)
{
    if (isSoundDeviceOpen())
    {
        closeSoundDevice();
    }

    s_channel_num = channel_num;
    s_sample_rate = sample_rate;
    s_snd_mix_buf_msec = snd_mix_buf_msec;
    s_snd_mix_buf_size = channel_num * sample_rate * 2 * snd_mix_buf_msec / 1000;
    s_snd_mix_buf_sample_num = sample_rate * snd_mix_buf_msec / 1000;

    // TODO
    // s_is_snd_dev_open = true;

    return false; // TODO
}


void pgLowLevelAPI::closeSoundDevice()
{
    // TODO

    s_is_snd_dev_open = false;
}


u8 pgLowLevelAPI::getSoundDeviceChannelNum()
{
    return s_channel_num;
}


u16 pgLowLevelAPI::getSoundDeviceSampleRate()
{
    return s_sample_rate;
}


u16 pgLowLevelAPI::getSoundMixBufferMsec()
{
    return s_snd_mix_buf_msec;
}


u32 pgLowLevelAPI::getSoundMixBufferSize()
{
    return s_snd_mix_buf_size;
}


u32 pgLowLevelAPI::getSoundMixBufferSampleNum()
{
    return s_snd_mix_buf_sample_num;
}


void pgLowLevelAPI::lockSoundMixMutex()
{
    // TODO
}


void pgLowLevelAPI::unlockSoundMixMutex()
{
    // TODO
}


#endif // PG_IPHONE
