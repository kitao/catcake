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


#ifdef CK_LINUX


#include "ck_low_level_api.h"

#include "ck_mem_all.h"

#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <pthread.h>

#define USE_ALSA

#ifdef USE_ALSA
#include <alsa/asoundlib.h>
#else // USE_ALSA
#include <linux/soundcard.h>
#include <sys/ioctl.h>
#endif // USE_ALSA

namespace
{

#ifdef USE_ALSA
snd_pcm_t* s_handle = NULL;
#else // USE_ALSA
int s_fd_dev_dsp = -1;
#endif // USE_ALSA

bool s_is_snd_dev_open = false;
volatile bool s_is_playing = false;
u8 s_channel_num;
u16 s_sample_rate;
u16 s_bits_per_sample;
u16 s_snd_mix_buf_msec;
u32 s_snd_mix_buf_size;
u32 s_snd_mix_buf_sample_num;

ckLowLevelAPI::SoundMixFunction s_snd_mix_func;
void* s_snd_mix_buf[2] = { NULL, NULL };

pthread_mutex_t s_snd_mix_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
pthread_t s_snd_play_thread;


#ifdef USE_ALSA
void* soundPlayThread(void*)
{
    int status;
    int sample_len;
    signed short* sample_buf;


    while (s_is_playing)
    {
        for (s32 i = 0; i < 2; i++)
        {
            if ((*s_snd_mix_func)(s_snd_mix_buf[i]))
            {
                sample_buf = (short*)s_snd_mix_buf[i];
                sample_len = s_snd_mix_buf_sample_num;

                status = snd_pcm_writei(s_handle, sample_buf, sample_len);
                if (status < 0)
                {
                    if (status == -EAGAIN)
                    {
                        usleep(1);
                        continue;
                    }
                    if (status == -ESTRPIPE)
                    {
                        do
                        {
                            usleep(1);
                            status = snd_pcm_resume(s_handle);
                        }
                        while (status == -EAGAIN);
                    }
                    if (status < 0)
                    {
                        status = snd_pcm_prepare(s_handle);
                    }
                    if (status < 0)
                    {
                        pthread_exit(NULL);
                    }
                    continue;
                }
                sample_buf += status * s_channel_num;
                sample_len -= status;
            }
        }
        pthread_yield();
    }
    pthread_exit(NULL);
}

#if 0
void checkPCMHWCaps(snd_pcm_t** handle)
{
    unsigned int chmin, chmax;
    status = snd_pcm_hw_params_get_channels_min(hwparams, &chmin);
    status = snd_pcm_hw_params_get_channels_max(hwparams, &chmax);
    printf("channels : min: %d max: %d\n", chmin, chmax);
    
    unsigned int ratemin, ratemax;
    int dirmin, dirmax;
    status = snd_pcm_hw_params_get_rate_min(hwparams, &ratemin, &dirmin);
    status = snd_pcm_hw_params_get_rate_max(hwparams, &ratemax, &dirmax);
    printf("rate : min: %d max: %d\n", ratemin, ratemax);
    
    snd_pcm_format_t formats[] = {
        SND_PCM_FORMAT_S8,
        SND_PCM_FORMAT_U8,
        SND_PCM_FORMAT_S16_LE,
        SND_PCM_FORMAT_S16_BE,
        SND_PCM_FORMAT_U16_LE,
        SND_PCM_FORMAT_U16_BE,
        SND_PCM_FORMAT_S24_LE,
        SND_PCM_FORMAT_S24_BE,
        SND_PCM_FORMAT_U24_LE,
        SND_PCM_FORMAT_U24_BE,
        SND_PCM_FORMAT_S32_LE,
        SND_PCM_FORMAT_S32_BE,
        SND_PCM_FORMAT_U32_LE,
        SND_PCM_FORMAT_U32_BE,
        SND_PCM_FORMAT_FLOAT_LE,
        SND_PCM_FORMAT_FLOAT_BE,
        SND_PCM_FORMAT_FLOAT64_LE,
        SND_PCM_FORMAT_FLOAT64_BE,
        SND_PCM_FORMAT_IEC958_SUBFRAME_LE,
        SND_PCM_FORMAT_IEC958_SUBFRAME_BE,
        SND_PCM_FORMAT_MU_LAW,
        SND_PCM_FORMAT_A_LAW,
        SND_PCM_FORMAT_IMA_ADPCM,
        SND_PCM_FORMAT_MPEG,
        SND_PCM_FORMAT_GSM,
        SND_PCM_FORMAT_SPECIAL,
        SND_PCM_FORMAT_S24_3LE,
        SND_PCM_FORMAT_S24_3BE,
        SND_PCM_FORMAT_U24_3LE,
        SND_PCM_FORMAT_U24_3BE,
        SND_PCM_FORMAT_S20_3LE,
        SND_PCM_FORMAT_S20_3BE,
        SND_PCM_FORMAT_U20_3LE,
        SND_PCM_FORMAT_U20_3BE,
        SND_PCM_FORMAT_S18_3LE,
        SND_PCM_FORMAT_S18_3BE,
        SND_PCM_FORMAT_U18_3LE,
        SND_PCM_FORMAT_U18_3BE,
        SND_PCM_FORMAT_LAST,
    
    #if __BYTE_ORDER == __LITTLE_ENDIAN
        SND_PCM_FORMAT_S16,
        SND_PCM_FORMAT_U16,
        SND_PCM_FORMAT_S24,
        SND_PCM_FORMAT_U24,
        SND_PCM_FORMAT_S32,
        SND_PCM_FORMAT_U32,
        SND_PCM_FORMAT_FLOAT,
        SND_PCM_FORMAT_FLOAT64,
        SND_PCM_FORMAT_IEC958_SUBFRAME
    #elif __BYTE_ORDER == __BIG_ENDIAN
        SND_PCM_FORMAT_S16,
        SND_PCM_FORMAT_U16,
        SND_PCM_FORMAT_S24,
        SND_PCM_FORMAT_U24,
        SND_PCM_FORMAT_S32,
        SND_PCM_FORMAT_U32,
        SND_PCM_FORMAT_FLOAT,
        SND_PCM_FORMAT_FLOAT64,
        SND_PCM_FORMAT_IEC958_SUBFRAME
    #else
    #error "Unknown endian"
    #endif
    };
    for (size_t i = 0; i < sizeof(formats) / sizeof(formats[0]); i++) {
        status = snd_pcm_hw_params_test_format(handle, hwparams, formats[i]);
        if (status == 0)
          printf("format: %d\n", formats[i]);
    }
}
#endif

int openPCM(snd_pcm_t** handle, u32* snd_mix_buf_sample_num, unsigned int channels, unsigned int rate, u16 bits_per_sample)
{
    int status = -1;
    snd_pcm_format_t format;
    snd_pcm_hw_params_t *hwparams;
    snd_pcm_sw_params_t *swparams;
    snd_pcm_uframes_t frames;
    unsigned int periods = 2;

    int m, n;
    const int max_hw = 20;
    int num_hw = 0;
    char devices[max_hw][13]; // "hw:0,0"..."hw:99,99", "plughw:0,0", "plughw:99,99"
    char tmp[13];
    int card;
    int dev;
    char buf[256];
    FILE* fp;
    int device_index = 0;
    
    snd_pcm_hw_params_alloca(&hwparams);
    snd_pcm_sw_params_alloca(&swparams);
    
    // get device names
    fp = fopen("/proc/asound/pcm", "r");
    if (fp == NULL)
        return -1;
    while (fgets(buf, 256, fp) != NULL)
    {
        if (sscanf(buf, "%d-%d:", &card, &dev) != 0)
        {
            if (card >= 100 || dev >= 100 || card < 0 || dev < 0) // prevent buffer overflow
                continue;

            sprintf(devices[num_hw], "plughw:%d,%d", card, dev);
            num_hw++;
            if (num_hw >= max_hw)
                break;

            sprintf(devices[num_hw], "hw:%d,%d", card, dev);
            num_hw++;
            if (num_hw >= max_hw)
                break;
        }
    }
    fclose(fp);

    // sort device names
    for (m = 0; m < num_hw - 1; m++)
    {
        for (n = m + 1; n < num_hw; n++)
        {
            if ((strcmp(devices[n], devices[m])) < 0)
            {
                strcpy(tmp, devices[m]);
                strcpy(devices[m], devices[n]);
                strcpy(devices[n], tmp);
            }
        }
    }

    if (bits_per_sample == 8)
        format = SND_PCM_FORMAT_U8;
    else if (bits_per_sample == 16)
        format = SND_PCM_FORMAT_S16_LE; // only support little endian because of no API to specify the endian type.
    else
        return -1;

    // at 1st, try to open the default device
    status = snd_pcm_open(handle, "default", SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);
    if (status < 0)
        goto RETRY;
    else
        goto NEXT;

RETRY:
    if (device_index >= num_hw)
        return -1;

    //printf("%s\n", snd_strerror(status));

    status = snd_pcm_open(handle, devices[device_index], SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);
    device_index++;

    if (status < 0)
        goto RETRY;
NEXT:


    status = snd_pcm_hw_params_any(*handle, hwparams);
    if (status < 0)
    {
        snd_pcm_close(*handle);
        goto RETRY;
    }

    status = snd_pcm_hw_params_set_access(*handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (status < 0)
    {
        snd_pcm_close(*handle);
        goto RETRY;
    }

    status = snd_pcm_hw_params_set_format(*handle, hwparams, format);
    if (status < 0)
    {
        snd_pcm_close(*handle);
        goto RETRY;
    }
    
    status = snd_pcm_hw_params_set_channels(*handle, hwparams, channels);
    if (status < 0)
    {
        snd_pcm_close(*handle);
        goto RETRY;
    }

    status = snd_pcm_hw_params_set_rate_near(*handle, hwparams, &rate, NULL);
    if (status < 0)
    {
        snd_pcm_close(*handle);
        goto RETRY;
    }

    frames = *snd_mix_buf_sample_num;
    status = snd_pcm_hw_params_set_period_size_near(*handle, hwparams, &frames, NULL);
    if (status < 0)
    {
        snd_pcm_close(*handle);
        goto RETRY;
    }
    *snd_mix_buf_sample_num = frames;

    status = snd_pcm_hw_params_set_periods_near(*handle, hwparams, &periods, NULL);
    if (status < 0)
    {
        snd_pcm_close(*handle);
        goto RETRY;
    }

    status = snd_pcm_hw_params(*handle, hwparams);
    if (status < 0)
    {
        snd_pcm_close(*handle);
        goto RETRY;
    }


    status = snd_pcm_sw_params_current(*handle, swparams);
    if (status < 0)
    {
        snd_pcm_close(*handle);
        goto RETRY;
    }

    status = snd_pcm_sw_params_set_start_threshold(*handle, swparams, 0);
    if (status < 0)
    {
        snd_pcm_close(*handle);
        goto RETRY;
    }

    status = snd_pcm_sw_params_set_avail_min(*handle, swparams, frames);
    if (status < 0)
    {
        snd_pcm_close(*handle);
        goto RETRY;
    }

    status = snd_pcm_sw_params(*handle, swparams);
    if (status < 0)
    {
        snd_pcm_close(*handle);
        goto RETRY;
    }

    snd_pcm_nonblock(*handle, 0);

    return 0;
}

#else // USE_ALSA

void* soundPlayThread(void*)
{
    while (s_is_playing)
    {
        for (s32 i = 0; i < 2; i++)
        {
            if ((*s_snd_mix_func)(s_snd_mix_buf[i]))
            {
                if (write(s_fd_dev_dsp, s_snd_mix_buf[i], s_snd_mix_buf_size) == -1)
                {
                    break;
                }
            }
        }
        pthread_yield();
    }
    pthread_exit(NULL);
}

int openDSP(int* fd, int channels, int rate, int bits_per_sample)
{
    int format;

    if (bits_per_sample == 8)
        format = AFMT_U8;
    else if (bits_per_sample == 16)
        format = AFMT_S16_LE;
    else
        return -1;

    if ((*fd = open("/dev/dsp", O_WRONLY)) == -1)
        return -1;

    if (ioctl(*fd, SNDCTL_DSP_SETFMT, &format) == -1)
    {
        close(*fd);
        return -1;
    }

    if (ioctl(*fd, SOUND_PCM_WRITE_RATE, &rate) == -1)
    {
        close(*fd);
        return -1;
    }

    if (ioctl(*fd, SOUND_PCM_WRITE_CHANNELS, &channels) == -1)
    {
        close(*fd);
        return -1;
    }

    return 0;
}
#endif // USE_ALSA

}

bool ckLowLevelAPI::isSoundDeviceOpen()
{
    return s_is_snd_dev_open;
}


bool ckLowLevelAPI::openSoundDevice(u8 channel_num, u16 sample_rate, u16 snd_mix_buf_msec, SoundMixFunction snd_mix_func)
{
    if (isSoundDeviceOpen())
    {
        closeSoundDevice();
    }

    pthread_mutex_init(&s_snd_mix_mutex, NULL);

    s_snd_mix_func =  snd_mix_func;

    s_channel_num = channel_num;
    s_sample_rate = sample_rate;
    s_snd_mix_buf_msec = snd_mix_buf_msec;
    s_snd_mix_buf_sample_num = sample_rate * snd_mix_buf_msec / 1000;
    s_bits_per_sample = 16;

#ifdef USE_ALSA

    if (openPCM(&s_handle, &s_snd_mix_buf_sample_num, s_channel_num, s_sample_rate, s_bits_per_sample) != 0)
        return false;

    s_snd_mix_buf_size = 2 * s_channel_num * s_snd_mix_buf_sample_num;
    for (s32 i = 0; i < 2; i++)
    {
        s_snd_mix_buf[i] = ckMalloc(s_snd_mix_buf_size);
        ckMemMgr::memset(s_snd_mix_buf[i], 0, s_snd_mix_buf_size);
    }
#else

    if (openDSP(&s_fd_dev_dsp, s_channel_num, s_sample_rate, s_bits_per_sample) != 0)
        return false;

    s_snd_mix_buf_size = 2 * channel_num * s_snd_mix_buf_sample_num;
    for (s32 i = 0; i < 2; i++)
    {
        s_snd_mix_buf[i] = ckMalloc(s_snd_mix_buf_size);
        ckMemMgr::memset(s_snd_mix_buf[i], 0, s_snd_mix_buf_size);
    }
#endif

    s_is_playing = true;

    if (pthread_create(&s_snd_play_thread, NULL, soundPlayThread, NULL) != 0)
    {
        s_is_playing = false;
        closeSoundDevice();
        return false;
    }

    s_is_snd_dev_open = true;

    return true;
}


void ckLowLevelAPI::closeSoundDevice()
{
    if (s_is_playing)
    {
        s_is_playing = false;
        pthread_join(s_snd_play_thread, NULL);
    }

    for (s32 i = 0; i < 2; i++)
    {
        if (s_snd_mix_buf[i])
        {
            ckFree(s_snd_mix_buf[i]);
            s_snd_mix_buf[i] = NULL;
        }
    }

#ifdef USE_ALSA

    if (s_handle != NULL)
    {
        snd_pcm_close(s_handle);
        s_handle = NULL;
    }
#else

    if (s_fd_dev_dsp != -1)
    {
        close(s_fd_dev_dsp);
        s_fd_dev_dsp = -1;
    }
#endif

    pthread_mutex_destroy(&s_snd_mix_mutex);

    s_is_snd_dev_open = false;
}


u8 ckLowLevelAPI::getSoundDeviceChannelNum()
{
    return s_channel_num;
}


u16 ckLowLevelAPI::getSoundDeviceSampleRate()
{
    return s_sample_rate;
}


u16 ckLowLevelAPI::getSoundMixBufferMsec()
{
    return s_snd_mix_buf_msec;
}


u32 ckLowLevelAPI::getSoundMixBufferSize()
{
    return s_snd_mix_buf_size;
}


u32 ckLowLevelAPI::getSoundMixBufferSampleNum()
{
    return s_snd_mix_buf_sample_num;
}


void ckLowLevelAPI::lockSoundMixMutex()
{
    pthread_mutex_lock(&s_snd_mix_mutex);
}


void ckLowLevelAPI::unlockSoundMixMutex()
{
    pthread_mutex_unlock(&s_snd_mix_mutex);
}


#endif // CK_LINUX
