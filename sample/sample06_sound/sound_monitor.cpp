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


#include "catcake.h"


class SoundMonitor : public ckTask
{
public:
    SoundMonitor();
    virtual ~SoundMonitor();

private:
    virtual void onUpdate();
    virtual void onMessage(ckID msg_id, ckMsg<4>& msg);
};


void newSoundMonitor()
{
    ckNewTask(SoundMonitor);
}


SoundMonitor::SoundMonitor() : ckTask(ORDER_ZERO)
{
    ckSndMgr::openSoundDevice(ckSndMgr::CHANNEL_NUM_MONO, ckSndMgr::SAMPLE_RATE_11KHZ, 50);
}


SoundMonitor::~SoundMonitor()
{
    ckSndMgr::closeSoundDevice();
}


void SoundMonitor::onUpdate()
{
    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_F))
    {
        ckSysMgr::toggleFullScreen(640, 480);
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_Q))
    {
        ckEndCatcake();
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_1))
    {
        ckSndMgr::openSoundDevice(ckSndMgr::CHANNEL_NUM_MONO, ckSndMgr::SAMPLE_RATE_11KHZ, 50);
    }
    else if (ckKeyMgr::isPressed(ckKeyMgr::KEY_2))
    {
        ckSndMgr::openSoundDevice(ckSndMgr::CHANNEL_NUM_MONO, ckSndMgr::SAMPLE_RATE_22KHZ, 50);
    }
    else if (ckKeyMgr::isPressed(ckKeyMgr::KEY_3))
    {
        ckSndMgr::openSoundDevice(ckSndMgr::CHANNEL_NUM_MONO, ckSndMgr::SAMPLE_RATE_44KHZ, 50);
    }
    else if (ckKeyMgr::isPressed(ckKeyMgr::KEY_4))
    {
        ckSndMgr::openSoundDevice(ckSndMgr::CHANNEL_NUM_STEREO, ckSndMgr::SAMPLE_RATE_11KHZ, 50);
    }
    else if (ckKeyMgr::isPressed(ckKeyMgr::KEY_5))
    {
        ckSndMgr::openSoundDevice(ckSndMgr::CHANNEL_NUM_STEREO, ckSndMgr::SAMPLE_RATE_22KHZ, 50);
    }
    else if (ckKeyMgr::isPressed(ckKeyMgr::KEY_6))
    {
        ckSndMgr::openSoundDevice(ckSndMgr::CHANNEL_NUM_STEREO, ckSndMgr::SAMPLE_RATE_44KHZ, 50);
    }

    if (!ckSndMgr::isSoundDeviceOpen())
    {
        ckDbgMgr::drawString(-138, 0, ckCol::FULL, 2, "Can't Open Sound Device");
    }

    ckDbgMgr::drawString(-280.0f, -166.0f, ckCol(255, 128, 128, 255), 1, //
        "When the sound device's sample rate is higher than sound data's sample rate, noise may occur.");

    ckDbgMgr::drawString(-280.0f, -200.0f, ckCol::FULL, 1, "SOUND DEVICE CHANNEL NUM: %d", ckSndMgr::getSoundDeviceChannelNum());
    ckDbgMgr::drawString(-280.0f, -210.0f, ckCol::FULL, 1, "SOUND DEVICE SAMPLE RATE: %dHz", ckSndMgr::getSoundDeviceSampleRate());

    for (u32 i = 0; i < ckSndMgr::TRACK_NUM; i++)
    {
        if (ckSndMgr::isPlaying(i))
        {
            ckDbgMgr::drawString(-242.0f + 150.0f * i, -144.0f, ckCol(255, 255, 255, 96), 1, "%06d", ckSndMgr::getPlayingPosition(i));
        }
    }
}


void SoundMonitor::onMessage(ckID msg_id, ckMsg<4>& msg)
{
    const u32 SOUND_NUM = 6;

    static const ckID s_snd_id[SOUND_NUM] =
    {
        ckID_("mono_11khz.wav"), ckID_("stereo_11khz.wav"), //
        ckID_("mono_22khz.wav"), ckID_("stereo_22khz.wav"), //
        ckID_("mono_44khz.wav"), ckID_("stereo_44khz.wav")
    };

    static const bool s_is_play_loop[SOUND_NUM] =
    {
        false, true, true, false, false, false
    };

    if (msg_id == ckID_("PLAY"))
    {
        u8 snd_no = msg.getParam<u8>(1);

        if (snd_no < SOUND_NUM)
        {
            ckSndMgr::play(msg.getParam<u8>(0), s_snd_id[snd_no], 128, s_is_play_loop[snd_no]);
        }
    }
    else if (msg_id == ckID_("FADE"))
    {
        ckSndMgr::fadeTrackVolume(msg.getParam<u8>(0), msg.getParam<u8>(1), msg.getParam<r32>(2));
    }
}
