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


#include "test.h"


void pgSndMgrTest()
{
    /*
        static const u32 TRACK_NUM
    */
    {
        pgAssert(pgSndMgr::TRACK_NUM == 4);
    }

    /*
        static bool isCreated()
        static void createAfterRes()
        static void destroyBeforeRes()
    */
    {
        pgAssert(!pgSndMgr::isCreated());

        pgSndMgr::createAfterRes();
        pgSndMgr::createAfterRes();

        pgAssert(pgSndMgr::isCreated());

        pgSndMgr::destroyBeforeRes();
        pgSndMgr::destroyBeforeRes();

        pgAssert(!pgSndMgr::isCreated());
    }

    /*
        static bool isSoundDeviceOpen()
        static bool openSoundDevice(ChannelNum channel_num, SampleRate sample_rate, u16 snd_mix_buf_msec)
        static void closeSoundDevice()
        static ChannelNum getSoundDeviceChannelNum()
        static SampleRate getSoundDeviceSampleRate()
        static u16 getSoundMixBufferMsec()
    */
    {
        pgAssertThrow(pgSndMgr::isSoundDeviceOpen(), pgSndMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSndMgr::openSoundDevice(pgSndMgr::CHANNEL_NUM_MONO, pgSndMgr::SAMPLE_RATE_11KHZ, 0), pgSndMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSndMgr::closeSoundDevice(), pgSndMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSndMgr::getSoundDeviceChannelNum(), pgSndMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSndMgr::getSoundDeviceSampleRate(), pgSndMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSndMgr::getSoundMixBufferMsec(), pgSndMgr::ExceptionNotInitialized);

        pgSndMgr::createAfterRes();

        pgAssert(!pgSndMgr::isSoundDeviceOpen());
        pgAssert(pgSndMgr::getSoundDeviceChannelNum() == pgSndMgr::CHANNEL_NUM_MONO);
        pgAssert(pgSndMgr::getSoundDeviceSampleRate() == pgSndMgr::SAMPLE_RATE_11KHZ);
        pgAssert(pgSndMgr::getSoundMixBufferMsec() == 0);

        pgSndMgr::closeSoundDevice();

        if (pgSndMgr::openSoundDevice(pgSndMgr::CHANNEL_NUM_MONO, pgSndMgr::SAMPLE_RATE_11KHZ, 1))
        {
            pgAssert(pgSndMgr::openSoundDevice(pgSndMgr::CHANNEL_NUM_STEREO, pgSndMgr::SAMPLE_RATE_44KHZ, 123));
            pgAssert(pgSndMgr::isSoundDeviceOpen());
            pgAssert(pgSndMgr::getSoundDeviceChannelNum() == pgSndMgr::CHANNEL_NUM_STEREO);
            pgAssert(pgSndMgr::getSoundDeviceSampleRate() == pgSndMgr::SAMPLE_RATE_44KHZ);
            pgAssert(pgSndMgr::getSoundMixBufferMsec() == 123);

            pgSndMgr::closeSoundDevice();
        }

        pgSndMgr::closeSoundDevice();

        pgAssert(!pgSndMgr::isSoundDeviceOpen());

        pgAssertThrow(pgSndMgr::openSoundDevice(pgSndMgr::CHANNEL_NUM_MONO, pgSndMgr::SAMPLE_RATE_11KHZ, 0), pgSndMgr::ExceptionInvalidArgument);

        pgSndMgr::destroyBeforeRes();
    }

    /*
        static ChannelNum getChannelNum(pgID snd_id)
        static SampleRate getSampleRate(pgID snd_id)
        static u32 getSampleNum(pgID snd_id)
    */
    {
        pgAssertThrow(pgSndMgr::getChannelNum(pgID::ZERO), pgSndMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSndMgr::getSampleRate(pgID::ZERO), pgSndMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSndMgr::getSampleNum(pgID::ZERO), pgSndMgr::ExceptionNotInitialized);

        pgSndMgr::createAfterRes();

        pgResMgr::loadResource(TEST_DATA_DIR "mono_11khz.wav", true);
        pgResMgr::loadResource(TEST_DATA_DIR "mono_22khz.wav", true);
        pgResMgr::loadResource(TEST_DATA_DIR "mono_44khz.wav", true);
        pgResMgr::loadResource(TEST_DATA_DIR "stereo_11khz.wav", true);
        pgResMgr::loadResource(TEST_DATA_DIR "stereo_22khz.wav", true);
        pgResMgr::loadResource(TEST_DATA_DIR "stereo_44khz.wav", true);
        pgResMgr::loadResource(TEST_DATA_DIR "rgb.png", true);

        pgAssert(pgSndMgr::getChannelNum(pgID_("mono_11khz.wav")) == pgSndMgr::CHANNEL_NUM_MONO);
        pgAssert(pgSndMgr::getSampleRate(pgID_("mono_11khz.wav")) == pgSndMgr::SAMPLE_RATE_11KHZ);
        pgAssert(pgSndMgr::getSampleNum(pgID_("mono_11khz.wav")) == 40792);

        pgAssert(pgSndMgr::getChannelNum(pgID_("mono_22khz.wav")) == pgSndMgr::CHANNEL_NUM_MONO);
        pgAssert(pgSndMgr::getSampleRate(pgID_("mono_22khz.wav")) == pgSndMgr::SAMPLE_RATE_22KHZ);
        pgAssert(pgSndMgr::getSampleNum(pgID_("mono_22khz.wav")) == 101269);

        pgAssert(pgSndMgr::getChannelNum(pgID_("mono_44khz.wav")) == pgSndMgr::CHANNEL_NUM_MONO);
        pgAssert(pgSndMgr::getSampleRate(pgID_("mono_44khz.wav")) == pgSndMgr::SAMPLE_RATE_44KHZ);
        pgAssert(pgSndMgr::getSampleNum(pgID_("mono_44khz.wav")) == 159157);

        pgAssert(pgSndMgr::getChannelNum(pgID_("stereo_11khz.wav")) == pgSndMgr::CHANNEL_NUM_STEREO);
        pgAssert(pgSndMgr::getSampleRate(pgID_("stereo_11khz.wav")) == pgSndMgr::SAMPLE_RATE_11KHZ);
        pgAssert(pgSndMgr::getSampleNum(pgID_("stereo_11khz.wav")) == 61535);

        pgAssert(pgSndMgr::getChannelNum(pgID_("stereo_22khz.wav")) == pgSndMgr::CHANNEL_NUM_STEREO);
        pgAssert(pgSndMgr::getSampleRate(pgID_("stereo_22khz.wav")) == pgSndMgr::SAMPLE_RATE_22KHZ);
        pgAssert(pgSndMgr::getSampleNum(pgID_("stereo_22khz.wav")) == 116553);

        pgAssert(pgSndMgr::getChannelNum(pgID_("stereo_44khz.wav")) == pgSndMgr::CHANNEL_NUM_STEREO);
        pgAssert(pgSndMgr::getSampleRate(pgID_("stereo_44khz.wav")) == pgSndMgr::SAMPLE_RATE_44KHZ);
        pgAssert(pgSndMgr::getSampleNum(pgID_("stereo_44khz.wav")) == 35944);

        pgAssertThrow(pgSndMgr::getChannelNum(pgID::ZERO), pgSndMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSndMgr::getChannelNum(pgID_("dummy")), pgSndMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSndMgr::getChannelNum(pgID_("rgb.png")), pgSndMgr::ExceptionInvalidType);
        pgAssertThrow(pgSndMgr::getSampleRate(pgID::ZERO), pgSndMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSndMgr::getSampleRate(pgID_("dummy")), pgSndMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSndMgr::getSampleRate(pgID_("rgb.png")), pgSndMgr::ExceptionInvalidType);
        pgAssertThrow(pgSndMgr::getSampleNum(pgID::ZERO), pgSndMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSndMgr::getSampleNum(pgID_("dummy")), pgSndMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgSndMgr::getSampleNum(pgID_("rgb.png")), pgSndMgr::ExceptionInvalidType);

        pgResMgr::removeResource(pgID_("mono_11khz.wav"));
        pgResMgr::removeResource(pgID_("mono_22khz.wav"));
        pgResMgr::removeResource(pgID_("mono_44khz.wav"));
        pgResMgr::removeResource(pgID_("stereo_11khz.wav"));
        pgResMgr::removeResource(pgID_("stereo_22khz.wav"));
        pgResMgr::removeResource(pgID_("stereo_44khz.wav"));
        pgResMgr::removeResource(pgID_("rgb.png"));

        pgSndMgr::destroyBeforeRes();
    }

    /*
        static u8 getTrackVolume(u8 trk_no)
        static void setTrackVolume(u8 trk_no, u8 trk_vol)
        static void fadeTrackVolume(u8 trk_no, u8 to_trk_vol, r32 delta_per_frame)
    */
    {
        pgAssertThrow(pgSndMgr::getTrackVolume(123), pgSndMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSndMgr::setTrackVolume(123, 123), pgSndMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSndMgr::fadeTrackVolume(123, 123, 0.0f), pgSndMgr::ExceptionNotInitialized);

        pgSndMgr::createAfterRes();

        pgAssert(pgSndMgr::getTrackVolume(123) == 0);

        pgSndMgr::setTrackVolume(123, 123);
        pgSndMgr::fadeTrackVolume(123, 123, 0.0f);

        if (pgSndMgr::openSoundDevice(pgSndMgr::CHANNEL_NUM_MONO, pgSndMgr::SAMPLE_RATE_22KHZ, 100))
        {
            for (u32 i = 0; i < pgSndMgr::TRACK_NUM; i++)
            {
                pgAssert(pgSndMgr::getTrackVolume(i) == 192);

                pgSndMgr::setTrackVolume(i, 100);
                pgAssert(pgSndMgr::getTrackVolume(i) == 100);
            }

            pgSndMgr::fadeTrackVolume(0, 98, 0.5f);
            pgSndMgr::fadeTrackVolume(1, 130, 20.0f);
            pgSndMgr::fadeTrackVolume(2, 100, 200.0f);
            pgSndMgr::fadeTrackVolume(3, 0, 10.0f);

            pgTaskMgr::updateForSystem();
            pgTaskMgr::updateForSystem();

            pgAssert(pgSndMgr::getTrackVolume(0) == 99);
            pgAssert(pgSndMgr::getTrackVolume(1) == 130);
            pgAssert(pgSndMgr::getTrackVolume(2) == 100);
            pgAssert(pgSndMgr::getTrackVolume(3) == 80);

            pgAssertThrow(pgSndMgr::getTrackVolume(pgSndMgr::TRACK_NUM), pgSndMgr::ExceptionInvalidArgument);
            pgAssertThrow(pgSndMgr::setTrackVolume(pgSndMgr::TRACK_NUM, 123), pgSndMgr::ExceptionInvalidArgument);
            pgAssertThrow(pgSndMgr::fadeTrackVolume(pgSndMgr::TRACK_NUM, 123, 100), pgSndMgr::ExceptionInvalidArgument);
            pgAssertThrow(pgSndMgr::fadeTrackVolume(pgSndMgr::TRACK_NUM, 123, 0), pgSndMgr::ExceptionInvalidArgument);
            pgAssertThrow(pgSndMgr::fadeTrackVolume(0, 123, 0.0f), pgSndMgr::ExceptionInvalidArgument);
            pgAssertThrow(pgSndMgr::fadeTrackVolume(0, 123, -1.0f), pgSndMgr::ExceptionInvalidArgument);
        }

        pgSndMgr::destroyBeforeRes();
    }

    /*
        static bool isPlaying(u8 trk_no)
        static void play(u8 trk_no, pgID snd_id, u8 snd_vol, bool is_play_loop, u32 play_pos = 0)
        static void stop(u8 trk_no)
        static pgID getPlayingSoundID(u8 trk_no)
        static u8 getPlayingSoundVolume(u8 trk_no)
        static bool isPlayingLoop(u8 trk_no)
        static u32 getPlayingPosition(u8 trk_no)
    */
    {
        pgAssertThrow(pgSndMgr::isPlaying(123), pgSndMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSndMgr::play(123, pgID::ZERO, 0, false, 0), pgSndMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSndMgr::stop(123), pgSndMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSndMgr::getPlayingSoundID(123), pgSndMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSndMgr::getPlayingSoundVolume(123), pgSndMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSndMgr::isPlayingLoop(123), pgSndMgr::ExceptionNotInitialized);
        pgAssertThrow(pgSndMgr::getPlayingPosition(123), pgSndMgr::ExceptionNotInitialized);

        pgSndMgr::createAfterRes();

        pgAssert(!pgSndMgr::isPlaying(123));
        pgAssert(pgSndMgr::getPlayingSoundID(123) == pgID::ZERO);
        pgAssert(pgSndMgr::getPlayingSoundVolume(123) == 0);
        pgAssert(!pgSndMgr::isPlayingLoop(123));
        pgAssert(pgSndMgr::getPlayingPosition(123) == 0);

        pgSndMgr::play(123, pgID::ZERO, 0, false, 0);
        pgSndMgr::stop(123);

        pgResMgr::loadResource(TEST_DATA_DIR "mono_22khz.wav", true);
        pgResMgr::loadResource(TEST_DATA_DIR "rgb.png", true);

        if (pgSndMgr::openSoundDevice(pgSndMgr::CHANNEL_NUM_STEREO, pgSndMgr::SAMPLE_RATE_11KHZ, 100))
        {
            for (u32 i = 0; i < pgSndMgr::TRACK_NUM; i++)
            {
                pgAssert(!pgSndMgr::isPlaying(i));

                pgAssertThrow(pgSndMgr::getPlayingSoundID(i), pgSndMgr::ExceptionInvalidCall);
                pgAssertThrow(pgSndMgr::isPlayingLoop(i), pgSndMgr::ExceptionInvalidCall);
                pgAssertThrow(pgSndMgr::getPlayingPosition(i), pgSndMgr::ExceptionInvalidCall);
                pgAssertThrow(pgSndMgr::getPlayingSoundVolume(i), pgSndMgr::ExceptionInvalidCall);

                pgAssertThrow(pgSndMgr::play(i, pgID_("mono_22khz.wav"), 0, false, pgSndMgr::getSampleNum(pgID_("mono_22khz.wav"))), pgSndMgr::ExceptionInvalidArgument);

                pgSndMgr::play(i, pgID_("mono_22khz.wav"), 12, false, 34);
                pgAssert(pgSndMgr::getPlayingSoundID(i) == pgID_("mono_22khz.wav"));
                pgAssert(pgSndMgr::getPlayingSoundVolume(i) == 12);
                pgAssert(!pgSndMgr::isPlayingLoop(i));
                pgAssert(pgSndMgr::getPlayingPosition(i) >= 34);

                pgSndMgr::play(i, pgID_("mono_22khz.wav"), 23, true, 45);
                pgAssert(pgSndMgr::getPlayingSoundID(i) == pgID_("mono_22khz.wav"));
                pgAssert(pgSndMgr::getPlayingSoundVolume(i) == 23);
                pgAssert(pgSndMgr::isPlayingLoop(i));
                pgAssert(pgSndMgr::getPlayingPosition(i) >= 45);

                pgSndMgr::stop(i);
                pgAssert(!pgSndMgr::isPlaying(i));
            }

            pgAssertThrow(pgSndMgr::isPlaying(pgSndMgr::TRACK_NUM), pgSndMgr::ExceptionInvalidArgument);
            pgAssertThrow(pgSndMgr::play(pgSndMgr::TRACK_NUM, pgID_("mono_22khz.wav"), 0, false, 0), pgSndMgr::ExceptionInvalidArgument);
            pgAssertThrow(pgSndMgr::play(0, pgID::ZERO, 0, false, 0), pgSndMgr::ExceptionInvalidArgument);
            pgAssertThrow(pgSndMgr::play(0, pgID_("dummy"), 0, false, 0), pgSndMgr::ExceptionNotFound);
            pgAssertThrow(pgSndMgr::play(0, pgID_("rgb.png"), 0, false, 0), pgSndMgr::ExceptionInvalidType);
            pgAssertThrow(pgSndMgr::stop(pgSndMgr::TRACK_NUM), pgSndMgr::ExceptionInvalidArgument);
            pgAssertThrow(pgSndMgr::getPlayingSoundID(pgSndMgr::TRACK_NUM), pgSndMgr::ExceptionInvalidArgument);
            pgAssertThrow(pgSndMgr::getPlayingSoundVolume(pgSndMgr::TRACK_NUM), pgSndMgr::ExceptionInvalidArgument);
            pgAssertThrow(pgSndMgr::isPlayingLoop(pgSndMgr::TRACK_NUM), pgSndMgr::ExceptionInvalidArgument);
            pgAssertThrow(pgSndMgr::getPlayingPosition(pgSndMgr::TRACK_NUM), pgSndMgr::ExceptionInvalidArgument);
        }

        pgSndMgr::destroyBeforeRes();

        pgResMgr::removeResource(pgID_("mono_22khz.wav"));
        pgResMgr::removeResource(pgID_("rgb.png"));
    }
}
