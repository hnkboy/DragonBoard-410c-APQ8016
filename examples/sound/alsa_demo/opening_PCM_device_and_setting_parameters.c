/*
 * list2.c
 * http://www.linuxjournal.com/article/6735?page=0,1
 * 20150911
 * opening PCM device and setting parameters
 */
/* use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API
/* All of the ALSA library API is defined in this header */
#include "alsa/asoundlib.h"

int main()
{
    int rc;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    unsigned int val, val2;
    int dir;
    snd_pcm_uframes_t frames;

    rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
        exit(1);
    }

    /* Alloc a hardware parameters object */
    snd_pcm_hw_params_alloca(&params);

    /* Fill it with default values */
    snd_pcm_hw_params_any(handle, params);

    /* set the disired hardware parameters */
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

    /* signed 16 bit little-endian format */
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

    /* two channels (stereo) */
    snd_pcm_hw_params_set_channels(handle, params, 2);

    val = 44100;
    /* 44100 bits/second sampling rate (CD quality) */
    snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

    /* write params to the driver */
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        fprintf(stderr, "unable to set hw params: %s\n", snd_strerror(rc));
        exit(1);
    }

    /* display information about PCM interface */
    printf("PCM handle name=%s\n", snd_pcm_name(handle));
    printf("PCM state=%s\n", snd_pcm_state_name(snd_pcm_state(handle)));

    snd_pcm_hw_params_get_access(params, (snd_pcm_access_t *)&val);
    printf("access type=%s\n", snd_pcm_access_name((snd_pcm_access_t)val));

    snd_pcm_hw_params_get_format(params, (snd_pcm_format_t *)&val);
    printf("format=%s (%s)\n", snd_pcm_format_name((snd_pcm_format_t)val),
                snd_pcm_format_description((snd_pcm_format_t)val));

    snd_pcm_hw_params_get_subformat(params, (snd_pcm_subformat_t *)&val);
    printf("subformat=%s (%s)\n", snd_pcm_subformat_name((snd_pcm_subformat_t)val),
                snd_pcm_subformat_description((snd_pcm_subformat_t)val));

    snd_pcm_hw_params_get_channels(params, &val);
    printf("channels=%d\n", val);

    snd_pcm_hw_params_get_rate(params, &val, &dir);
    printf("rate=%d bps\n", val);

    snd_pcm_hw_params_get_period_time(params, &val, &dir);
    printf("period time=%d us\n", val);

    snd_pcm_hw_params_get_period_size(params, (snd_pcm_uframes_t *)&frames, &dir);
    printf("period size=%d frames\n", (int)frames);//add (int) to avoid compile warning

    snd_pcm_hw_params_get_buffer_time(params, &val, &dir);
    printf("buffer time=%d us\n", val);

    snd_pcm_hw_params_get_buffer_size(params, (snd_pcm_uframes_t *)&frames);
    printf("buffer size=%d frames\n", (int)frames);//add (int) to avoid compile warning
    snd_pcm_hw_params_get_periods(params, &val, &dir);
    printf("periods per buffer=%d frames\n", val);

    snd_pcm_hw_params_get_rate_numden(params, &val, &val2);
    printf("exact rate=%d/%d bps\n", val, val2);

    val = snd_pcm_hw_params_get_sbits(params);
    printf("significant bits=%d\n", val);

    snd_pcm_hw_params_get_tick_time(params, &val, &dir);
    printf("tick time=%d us\n", val);

    val = snd_pcm_hw_params_is_batch(params);
    printf("is batch=%d\n", val);

    val = snd_pcm_hw_params_is_block_transfer(params);
    printf("is block transfer=%d\n", val);

    val = snd_pcm_hw_params_is_double(params);
    printf("is double=%d\n", val);

    val = snd_pcm_hw_params_is_half_duplex(params);
    printf("is half duplex=%d\n", val);

    val = snd_pcm_hw_params_is_joint_duplex(params);
    printf("is jonit duplex=%d\n", val);

    val = snd_pcm_hw_params_can_overrange(params);
    printf("can overrange=%d\n", val);

    val = snd_pcm_hw_params_can_mmap_sample_resolution(params);
    printf("can mmap=%d\n", val);

    val = snd_pcm_hw_params_can_pause(params);
    printf("can pause=%d\n", val);

    val = snd_pcm_hw_params_can_resume(params);
    printf("can resume=%d\n", val);

    val = snd_pcm_hw_params_can_sync_start(params);
    printf("can sync start=%d\n", val);

    snd_pcm_close(handle);

    return 0;
}