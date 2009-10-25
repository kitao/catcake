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


#include "pogolyn.h"


class SoundMonitor : public pgTask
{
public:
    SoundMonitor();
    virtual ~SoundMonitor();

private:
    virtual void onUpdate();
    virtual void onMessage(pgID msg_id, pgMsg<4>& msg);
};


void newSoundMonitor()
{
    pgNewTask(SoundMonitor);
}


SoundMonitor::SoundMonitor() : pgTask(ORDER_ZERO)
{
    pgSndMgr::openSoundDevice(pgSndMgr::CHANNEL_NUM_MONO, pgSndMgr::SAMPLE_RATE_11KHZ, 50);
}


SoundMonitor::~SoundMonitor()
{
    pgSndMgr::closeSoundDevice();
}


void SoundMonitor::onUpdate()
{
    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_F))
    {
        pgSysMgr::toggleFullScreen(640, 480);
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_Q))
    {
        pgEndPogolyn();
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_1))
    {
        pgSndMgr::openSoundDevice(pgSndMgr::CHANNEL_NUM_MONO, pgSndMgr::SAMPLE_RATE_11KHZ, 50);
    }
    else if (pgKeyMgr::isPressed(pgKeyMgr::KEY_2))
    {
        pgSndMgr::openSoundDevice(pgSndMgr::CHANNEL_NUM_MONO, pgSndMgr::SAMPLE_RATE_22KHZ, 50);
    }
    else if (pgKeyMgr::isPressed(pgKeyMgr::KEY_3))
    {
        pgSndMgr::openSoundDevice(pgSndMgr::CHANNEL_NUM_MONO, pgSndMgr::SAMPLE_RATE_44KHZ, 50);
    }
    else if (pgKeyMgr::isPressed(pgKeyMgr::KEY_4))
    {
        pgSndMgr::openSoundDevice(pgSndMgr::CHANNEL_NUM_STEREO, pgSndMgr::SAMPLE_RATE_11KHZ, 50);
    }
    else if (pgKeyMgr::isPressed(pgKeyMgr::KEY_5))
    {
        pgSndMgr::openSoundDevice(pgSndMgr::CHANNEL_NUM_STEREO, pgSndMgr::SAMPLE_RATE_22KHZ, 50);
    }
    else if (pgKeyMgr::isPressed(pgKeyMgr::KEY_6))
    {
        pgSndMgr::openSoundDevice(pgSndMgr::CHANNEL_NUM_STEREO, pgSndMgr::SAMPLE_RATE_44KHZ, 50);
    }

    if (!pgSndMgr::isSoundDeviceOpen())
    {
        pgDbgMgr::drawString(-138, 0, pgCol::FULL, 2, "Can't Open Sound Device");
    }

    pgDbgMgr::drawString(-280.0f, -166.0f, pgCol(255, 128, 128, 255), 1, //
        "When the sound device's sample rate is higher than sound data's sample rate, noise may occur.");

    pgDbgMgr::drawString(-280.0f, -200.0f, pgCol::FULL, 1, "SOUND DEVICE CHANNEL NUM: %d", pgSndMgr::getSoundDeviceChannelNum());
    pgDbgMgr::drawString(-280.0f, -210.0f, pgCol::FULL, 1, "SOUND DEVICE SAMPLE RATE: %dHz", pgSndMgr::getSoundDeviceSampleRate());

    for (u32 i = 0; i < pgSndMgr::TRACK_NUM; i++)
    {
        if (pgSndMgr::isPlaying(i))
        {
            pgDbgMgr::drawString(-242.0f + 150.0f * i, -144.0f, pgCol(255, 255, 255, 96), 1, "%06d", pgSndMgr::getPlayingPosition(i));
        }
    }
}


void SoundMonitor::onMessage(pgID msg_id, pgMsg<4>& msg)
{
    const u32 SOUND_NUM = 6;

    static const pgID s_snd_id[SOUND_NUM] =
    {
        pgID_("mono_11khz.wav"), pgID_("stereo_11khz.wav"), //
        pgID_("mono_22khz.wav"), pgID_("stereo_22khz.wav"), //
        pgID_("mono_44khz.wav"), pgID_("stereo_44khz.wav")
    };

    static const bool s_is_play_loop[SOUND_NUM] =
    {
        false, true, true, false, false, false
    };

    if (msg_id == pgID_("PLAY"))
    {
        u8 snd_no = msg.getParam<u8>(1);

        if (snd_no < SOUND_NUM)
        {
            pgSndMgr::play(msg.getParam<u8>(0), s_snd_id[snd_no], 128, s_is_play_loop[snd_no]);
        }
    }
    else if (msg_id == pgID_("FADE"))
    {
        pgSndMgr::fadeTrackVolume(msg.getParam<u8>(0), msg.getParam<u8>(1), msg.getParam<r32>(2));
    }
}
