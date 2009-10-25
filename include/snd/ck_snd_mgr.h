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


class pgSndDaemon;


/*!
    @ingroup pgSnd
    TODO
*/
class PG_API pgSndMgr
{
    friend class pgSndDaemon;

public:
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionInvalidCall);
    pgDefineException(ExceptionInvalidType);
    pgDefineException(ExceptionNotFound);
    pgDefineException(ExceptionNotInitialized);
    pgDefineException(ExceptionWaveInitializerFailed);

    static const u32 TRACK_NUM = 4;

    enum ChannelNum
    {
        CHANNEL_NUM_MONO = 1, //
        CHANNEL_NUM_STEREO = 2
    };

    enum SampleRate
    {
        SAMPLE_RATE_11KHZ = 11025, //
        SAMPLE_RATE_22KHZ = 22050, //
        SAMPLE_RATE_44KHZ = 44100
    };

    static bool isCreated();
    static void createAfterRes();
    static void destroyBeforeRes();

    static bool isSoundDeviceOpen();
    static bool openSoundDevice(ChannelNum channel_num, SampleRate sample_rate, u16 snd_mix_buf_msec);
    static void closeSoundDevice();

    static ChannelNum getSoundDeviceChannelNum();
    static SampleRate getSoundDeviceSampleRate();
    static u16 getSoundMixBufferMsec();

    static ChannelNum getChannelNum(pgID snd_id);
    static SampleRate getSampleRate(pgID snd_id);
    static u32 getSampleNum(pgID snd_id);

    static u8 getTrackVolume(u8 trk_no);
    static void setTrackVolume(u8 trk_no, u8 trk_vol);
    static void fadeTrackVolume(u8 trk_no, u8 to_trk_vol, r32 delta_per_frame);

    static bool isPlaying(u8 trk_no);
    static void play(u8 trk_no, pgID snd_id, u8 snd_vol, bool is_play_loop, u32 play_pos = 0);
    static void stop(u8 trk_no);

    static pgID getPlayingSoundID(u8 trk_no);
    static u8 getPlayingSoundVolume(u8 trk_no);
    static bool isPlayingLoop(u8 trk_no);
    static u32 getPlayingPosition(u8 trk_no);

private:
    enum TrackState
    {
        STATE_STOP, //
        STATE_PLAY_ONESHOT, //
        STATE_PLAY_LOOP
    };

    struct SoundInfo
    {
        pgType<u8, ChannelNum> channel_num;
        pgType<u16, SampleRate> sample_rate;
        const void* data;
        u32 data_size;
        u32 sample_num;
    };

    struct TrackInfo
    {
        pgType<u8, TrackState> trk_state;
        r32 trk_vol;
        pgID snd_id;
        u8 snd_vol;
        const SoundInfo* snd_info;
        u32 play_pos;
        u8 fade_to_trk_vol;
        u32 fade_frame_count;
        r32 fade_delta_per_frame;
    };

    pgSndMgr();
    ~pgSndMgr();
    void operator=(const pgSndMgr&);

    static pgSndMgr* instance();

    static bool soundMixFunction(void* snd_mix_buf);

    static void waveInitializer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void** exinfo);
    static void waveFinalizer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void* exinfo);

    TrackInfo m_trk_info[TRACK_NUM];
    pgSndDaemon* m_snd_daemon;

    static pgSndMgr* m_instance;
};
