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


#include "test.h"


void ckSndMgrTest()
{
    /*
        static const u32 TRACK_NUM
    */
    {
        ckAssert(ckSndMgr::TRACK_NUM == 4);
    }

    /*
        static bool isCreated()
        static void createAfterRes()
        static void destroyBeforeRes()
    */
    {
        ckAssert(!ckSndMgr::isCreated());

        ckSndMgr::createAfterRes();
        ckSndMgr::createAfterRes();

        ckAssert(ckSndMgr::isCreated());

        ckSndMgr::destroyBeforeRes();
        ckSndMgr::destroyBeforeRes();

        ckAssert(!ckSndMgr::isCreated());
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
        ckAssertThrow(ckSndMgr::isSoundDeviceOpen(), ckSndMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSndMgr::openSoundDevice(ckSndMgr::CHANNEL_NUM_MONO, ckSndMgr::SAMPLE_RATE_11KHZ, 0), ckSndMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSndMgr::closeSoundDevice(), ckSndMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSndMgr::getSoundDeviceChannelNum(), ckSndMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSndMgr::getSoundDeviceSampleRate(), ckSndMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSndMgr::getSoundMixBufferMsec(), ckSndMgr::ExceptionNotInitialized);

        ckSndMgr::createAfterRes();

        ckAssert(!ckSndMgr::isSoundDeviceOpen());
        ckAssert(ckSndMgr::getSoundDeviceChannelNum() == ckSndMgr::CHANNEL_NUM_MONO);
        ckAssert(ckSndMgr::getSoundDeviceSampleRate() == ckSndMgr::SAMPLE_RATE_11KHZ);
        ckAssert(ckSndMgr::getSoundMixBufferMsec() == 0);

        ckSndMgr::closeSoundDevice();

        if (ckSndMgr::openSoundDevice(ckSndMgr::CHANNEL_NUM_MONO, ckSndMgr::SAMPLE_RATE_11KHZ, 1))
        {
            ckAssert(ckSndMgr::openSoundDevice(ckSndMgr::CHANNEL_NUM_STEREO, ckSndMgr::SAMPLE_RATE_44KHZ, 123));
            ckAssert(ckSndMgr::isSoundDeviceOpen());
            ckAssert(ckSndMgr::getSoundDeviceChannelNum() == ckSndMgr::CHANNEL_NUM_STEREO);
            ckAssert(ckSndMgr::getSoundDeviceSampleRate() == ckSndMgr::SAMPLE_RATE_44KHZ);
            ckAssert(ckSndMgr::getSoundMixBufferMsec() == 123);

            ckSndMgr::closeSoundDevice();
        }

        ckSndMgr::closeSoundDevice();

        ckAssert(!ckSndMgr::isSoundDeviceOpen());

        ckAssertThrow(ckSndMgr::openSoundDevice(ckSndMgr::CHANNEL_NUM_MONO, ckSndMgr::SAMPLE_RATE_11KHZ, 0), ckSndMgr::ExceptionInvalidArgument);

        ckSndMgr::destroyBeforeRes();
    }

    /*
        static ChannelNum getChannelNum(ckID snd_id)
        static SampleRate getSampleRate(ckID snd_id)
        static u32 getSampleNum(ckID snd_id)
    */
    {
        ckAssertThrow(ckSndMgr::getChannelNum(ckID::ZERO), ckSndMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSndMgr::getSampleRate(ckID::ZERO), ckSndMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSndMgr::getSampleNum(ckID::ZERO), ckSndMgr::ExceptionNotInitialized);

        ckSndMgr::createAfterRes();

        ckResMgr::loadResource(TEST_DATA_DIR "mono_11khz.wav", true);
        ckResMgr::loadResource(TEST_DATA_DIR "mono_22khz.wav", true);
        ckResMgr::loadResource(TEST_DATA_DIR "mono_44khz.wav", true);
        ckResMgr::loadResource(TEST_DATA_DIR "stereo_11khz.wav", true);
        ckResMgr::loadResource(TEST_DATA_DIR "stereo_22khz.wav", true);
        ckResMgr::loadResource(TEST_DATA_DIR "stereo_44khz.wav", true);
        ckResMgr::loadResource(TEST_DATA_DIR "rgb.png", true);

        ckAssert(ckSndMgr::getChannelNum(ckID_("mono_11khz.wav")) == ckSndMgr::CHANNEL_NUM_MONO);
        ckAssert(ckSndMgr::getSampleRate(ckID_("mono_11khz.wav")) == ckSndMgr::SAMPLE_RATE_11KHZ);
        ckAssert(ckSndMgr::getSampleNum(ckID_("mono_11khz.wav")) == 40792);

        ckAssert(ckSndMgr::getChannelNum(ckID_("mono_22khz.wav")) == ckSndMgr::CHANNEL_NUM_MONO);
        ckAssert(ckSndMgr::getSampleRate(ckID_("mono_22khz.wav")) == ckSndMgr::SAMPLE_RATE_22KHZ);
        ckAssert(ckSndMgr::getSampleNum(ckID_("mono_22khz.wav")) == 101269);

        ckAssert(ckSndMgr::getChannelNum(ckID_("mono_44khz.wav")) == ckSndMgr::CHANNEL_NUM_MONO);
        ckAssert(ckSndMgr::getSampleRate(ckID_("mono_44khz.wav")) == ckSndMgr::SAMPLE_RATE_44KHZ);
        ckAssert(ckSndMgr::getSampleNum(ckID_("mono_44khz.wav")) == 159157);

        ckAssert(ckSndMgr::getChannelNum(ckID_("stereo_11khz.wav")) == ckSndMgr::CHANNEL_NUM_STEREO);
        ckAssert(ckSndMgr::getSampleRate(ckID_("stereo_11khz.wav")) == ckSndMgr::SAMPLE_RATE_11KHZ);
        ckAssert(ckSndMgr::getSampleNum(ckID_("stereo_11khz.wav")) == 61535);

        ckAssert(ckSndMgr::getChannelNum(ckID_("stereo_22khz.wav")) == ckSndMgr::CHANNEL_NUM_STEREO);
        ckAssert(ckSndMgr::getSampleRate(ckID_("stereo_22khz.wav")) == ckSndMgr::SAMPLE_RATE_22KHZ);
        ckAssert(ckSndMgr::getSampleNum(ckID_("stereo_22khz.wav")) == 116553);

        ckAssert(ckSndMgr::getChannelNum(ckID_("stereo_44khz.wav")) == ckSndMgr::CHANNEL_NUM_STEREO);
        ckAssert(ckSndMgr::getSampleRate(ckID_("stereo_44khz.wav")) == ckSndMgr::SAMPLE_RATE_44KHZ);
        ckAssert(ckSndMgr::getSampleNum(ckID_("stereo_44khz.wav")) == 35944);

        ckAssertThrow(ckSndMgr::getChannelNum(ckID::ZERO), ckSndMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSndMgr::getChannelNum(ckID_("dummy")), ckSndMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSndMgr::getChannelNum(ckID_("rgb.png")), ckSndMgr::ExceptionInvalidType);
        ckAssertThrow(ckSndMgr::getSampleRate(ckID::ZERO), ckSndMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSndMgr::getSampleRate(ckID_("dummy")), ckSndMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSndMgr::getSampleRate(ckID_("rgb.png")), ckSndMgr::ExceptionInvalidType);
        ckAssertThrow(ckSndMgr::getSampleNum(ckID::ZERO), ckSndMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSndMgr::getSampleNum(ckID_("dummy")), ckSndMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckSndMgr::getSampleNum(ckID_("rgb.png")), ckSndMgr::ExceptionInvalidType);

        ckResMgr::removeResource(ckID_("mono_11khz.wav"));
        ckResMgr::removeResource(ckID_("mono_22khz.wav"));
        ckResMgr::removeResource(ckID_("mono_44khz.wav"));
        ckResMgr::removeResource(ckID_("stereo_11khz.wav"));
        ckResMgr::removeResource(ckID_("stereo_22khz.wav"));
        ckResMgr::removeResource(ckID_("stereo_44khz.wav"));
        ckResMgr::removeResource(ckID_("rgb.png"));

        ckSndMgr::destroyBeforeRes();
    }

    /*
        static u8 getTrackVolume(u8 trk_no)
        static void setTrackVolume(u8 trk_no, u8 trk_vol)
        static void fadeTrackVolume(u8 trk_no, u8 to_trk_vol, r32 delta_per_frame)
    */
    {
        ckAssertThrow(ckSndMgr::getTrackVolume(123), ckSndMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSndMgr::setTrackVolume(123, 123), ckSndMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSndMgr::fadeTrackVolume(123, 123, 0.0f), ckSndMgr::ExceptionNotInitialized);

        ckSndMgr::createAfterRes();

        ckAssert(ckSndMgr::getTrackVolume(123) == 0);

        ckSndMgr::setTrackVolume(123, 123);
        ckSndMgr::fadeTrackVolume(123, 123, 0.0f);

        if (ckSndMgr::openSoundDevice(ckSndMgr::CHANNEL_NUM_MONO, ckSndMgr::SAMPLE_RATE_22KHZ, 100))
        {
            for (u32 i = 0; i < ckSndMgr::TRACK_NUM; i++)
            {
                ckAssert(ckSndMgr::getTrackVolume(i) == 192);

                ckSndMgr::setTrackVolume(i, 100);
                ckAssert(ckSndMgr::getTrackVolume(i) == 100);
            }

            ckSndMgr::fadeTrackVolume(0, 98, 0.5f);
            ckSndMgr::fadeTrackVolume(1, 130, 20.0f);
            ckSndMgr::fadeTrackVolume(2, 100, 200.0f);
            ckSndMgr::fadeTrackVolume(3, 0, 10.0f);

            ckTaskMgr::updateForSystem();
            ckTaskMgr::updateForSystem();

            ckAssert(ckSndMgr::getTrackVolume(0) == 99);
            ckAssert(ckSndMgr::getTrackVolume(1) == 130);
            ckAssert(ckSndMgr::getTrackVolume(2) == 100);
            ckAssert(ckSndMgr::getTrackVolume(3) == 80);

            ckAssertThrow(ckSndMgr::getTrackVolume(ckSndMgr::TRACK_NUM), ckSndMgr::ExceptionInvalidArgument);
            ckAssertThrow(ckSndMgr::setTrackVolume(ckSndMgr::TRACK_NUM, 123), ckSndMgr::ExceptionInvalidArgument);
            ckAssertThrow(ckSndMgr::fadeTrackVolume(ckSndMgr::TRACK_NUM, 123, 100), ckSndMgr::ExceptionInvalidArgument);
            ckAssertThrow(ckSndMgr::fadeTrackVolume(ckSndMgr::TRACK_NUM, 123, 0), ckSndMgr::ExceptionInvalidArgument);
            ckAssertThrow(ckSndMgr::fadeTrackVolume(0, 123, 0.0f), ckSndMgr::ExceptionInvalidArgument);
            ckAssertThrow(ckSndMgr::fadeTrackVolume(0, 123, -1.0f), ckSndMgr::ExceptionInvalidArgument);
        }

        ckSndMgr::destroyBeforeRes();
    }

    /*
        static bool isPlaying(u8 trk_no)
        static void play(u8 trk_no, ckID snd_id, u8 snd_vol, bool is_play_loop, u32 play_pos = 0)
        static void stop(u8 trk_no)
        static ckID getPlayingSoundID(u8 trk_no)
        static u8 getPlayingSoundVolume(u8 trk_no)
        static bool isPlayingLoop(u8 trk_no)
        static u32 getPlayingPosition(u8 trk_no)
    */
    {
        ckAssertThrow(ckSndMgr::isPlaying(123), ckSndMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSndMgr::play(123, ckID::ZERO, 0, false, 0), ckSndMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSndMgr::stop(123), ckSndMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSndMgr::getPlayingSoundID(123), ckSndMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSndMgr::getPlayingSoundVolume(123), ckSndMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSndMgr::isPlayingLoop(123), ckSndMgr::ExceptionNotInitialized);
        ckAssertThrow(ckSndMgr::getPlayingPosition(123), ckSndMgr::ExceptionNotInitialized);

        ckSndMgr::createAfterRes();

        ckAssert(!ckSndMgr::isPlaying(123));
        ckAssert(ckSndMgr::getPlayingSoundID(123) == ckID::ZERO);
        ckAssert(ckSndMgr::getPlayingSoundVolume(123) == 0);
        ckAssert(!ckSndMgr::isPlayingLoop(123));
        ckAssert(ckSndMgr::getPlayingPosition(123) == 0);

        ckSndMgr::play(123, ckID::ZERO, 0, false, 0);
        ckSndMgr::stop(123);

        ckResMgr::loadResource(TEST_DATA_DIR "mono_22khz.wav", true);
        ckResMgr::loadResource(TEST_DATA_DIR "rgb.png", true);

        if (ckSndMgr::openSoundDevice(ckSndMgr::CHANNEL_NUM_STEREO, ckSndMgr::SAMPLE_RATE_11KHZ, 100))
        {
            for (u32 i = 0; i < ckSndMgr::TRACK_NUM; i++)
            {
                ckAssert(!ckSndMgr::isPlaying(i));

                ckAssertThrow(ckSndMgr::getPlayingSoundID(i), ckSndMgr::ExceptionInvalidCall);
                ckAssertThrow(ckSndMgr::isPlayingLoop(i), ckSndMgr::ExceptionInvalidCall);
                ckAssertThrow(ckSndMgr::getPlayingPosition(i), ckSndMgr::ExceptionInvalidCall);
                ckAssertThrow(ckSndMgr::getPlayingSoundVolume(i), ckSndMgr::ExceptionInvalidCall);

                ckAssertThrow(ckSndMgr::play(i, ckID_("mono_22khz.wav"), 0, false, ckSndMgr::getSampleNum(ckID_("mono_22khz.wav"))), ckSndMgr::ExceptionInvalidArgument);

                ckSndMgr::play(i, ckID_("mono_22khz.wav"), 12, false, 34);
                ckAssert(ckSndMgr::getPlayingSoundID(i) == ckID_("mono_22khz.wav"));
                ckAssert(ckSndMgr::getPlayingSoundVolume(i) == 12);
                ckAssert(!ckSndMgr::isPlayingLoop(i));
                ckAssert(ckSndMgr::getPlayingPosition(i) >= 34);

                ckSndMgr::play(i, ckID_("mono_22khz.wav"), 23, true, 45);
                ckAssert(ckSndMgr::getPlayingSoundID(i) == ckID_("mono_22khz.wav"));
                ckAssert(ckSndMgr::getPlayingSoundVolume(i) == 23);
                ckAssert(ckSndMgr::isPlayingLoop(i));
                ckAssert(ckSndMgr::getPlayingPosition(i) >= 45);

                ckSndMgr::stop(i);
                ckAssert(!ckSndMgr::isPlaying(i));
            }

            ckAssertThrow(ckSndMgr::isPlaying(ckSndMgr::TRACK_NUM), ckSndMgr::ExceptionInvalidArgument);
            ckAssertThrow(ckSndMgr::play(ckSndMgr::TRACK_NUM, ckID_("mono_22khz.wav"), 0, false, 0), ckSndMgr::ExceptionInvalidArgument);
            ckAssertThrow(ckSndMgr::play(0, ckID::ZERO, 0, false, 0), ckSndMgr::ExceptionInvalidArgument);
            ckAssertThrow(ckSndMgr::play(0, ckID_("dummy"), 0, false, 0), ckSndMgr::ExceptionNotFound);
            ckAssertThrow(ckSndMgr::play(0, ckID_("rgb.png"), 0, false, 0), ckSndMgr::ExceptionInvalidType);
            ckAssertThrow(ckSndMgr::stop(ckSndMgr::TRACK_NUM), ckSndMgr::ExceptionInvalidArgument);
            ckAssertThrow(ckSndMgr::getPlayingSoundID(ckSndMgr::TRACK_NUM), ckSndMgr::ExceptionInvalidArgument);
            ckAssertThrow(ckSndMgr::getPlayingSoundVolume(ckSndMgr::TRACK_NUM), ckSndMgr::ExceptionInvalidArgument);
            ckAssertThrow(ckSndMgr::isPlayingLoop(ckSndMgr::TRACK_NUM), ckSndMgr::ExceptionInvalidArgument);
            ckAssertThrow(ckSndMgr::getPlayingPosition(ckSndMgr::TRACK_NUM), ckSndMgr::ExceptionInvalidArgument);
        }

        ckSndMgr::destroyBeforeRes();

        ckResMgr::removeResource(ckID_("mono_22khz.wav"));
        ckResMgr::removeResource(ckID_("rgb.png"));
    }
}
