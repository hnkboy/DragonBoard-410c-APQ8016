/*
 * list3.c
 * read from input(STDIN) and writes to the default PCM device
 * for 5 seconds of data

 ./list3 < /dev/urandom
 */

/* use the newer ALSA_API */

#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>

/*
 * init the alsa/device
 * times : us
*/
      

snd_pcm_t *alsa_init(snd_pcm_hw_params_t *params, 
						snd_pcm_uframes_t frames,
						unsigned int *period)
{
    int rc;
    snd_pcm_t *handle;
    unsigned int val;
    int dir;  
   

   
    int fd;
    rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        fprintf(stderr, "unable to open PCM device: %s\n",
                snd_strerror(rc));
        exit(1);

    }

    /* alloc hardware params object */
    snd_pcm_hw_params_alloca(&params);

    /* fill it with default values */
    snd_pcm_hw_params_any(handle, params);

    /* interleaved mode */
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

    /* signed 16 bit little ending format */
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

    /* two channels (stereo) */
    snd_pcm_hw_params_set_channels(handle, params, 2);

    /* 44100 bits/second sampling rate (CD quality) */
    val = 44100;
    snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

    /* set period size t 32 frames */
    frames = 32;    
    snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

    /* write params to the driver */
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        fprintf(stderr, "unable to set hw params: %s\n",
                snd_strerror(rc));
        exit(1);
    }
	/* use buffer large enough to hold one period */
    snd_pcm_hw_params_get_period_size(params, &frames, &dir);

    /* we want to loop for 5 seconds */
    snd_pcm_hw_params_get_period_time(params, &val, &dir);
	*period = val;
	return handle;                                       
}
int exce_alsa(snd_pcm_t *handle,
				snd_pcm_uframes_t frames,
			    unsigned int period,	
				int timeus){
	

	int size;
	long loops;
	int rc;
	char *buffer;
    //frames = 32;
	int fd;
	fd = open("./01.pcm",O_RDONLY);
	if(fd < 0){
		fprintf(stderr, "open error\n");
        return -1;
	}
	size = frames * 4; //2 bytes/sample, 2 channels
    buffer = (char *)malloc(size);

    /* 5 second in micro seconds divided by period time */
	/*  is period time*/
    loops = timeus / period;
    while (loops > 0) {
        loops--;
        rc = read(fd, buffer, size);
        if (rc == 0) {
            fprintf(stderr, "end of file on input\n");
            break;
        } else if (rc != size) {
            fprintf(stderr, "short read: read %d bytes\n", rc);
        }

        rc = snd_pcm_writei(handle, buffer, frames);
        if (rc == -EPIPE) {
            /* -EPIPE means underrun */
            fprintf(stderr, "underrun occured\n");
            snd_pcm_prepare(handle);
        } else if (rc < 0) {
            fprintf(stderr, "error from writei: %s\n", snd_strerror(rc));
        } else if (rc != (int)frames) {
            fprintf(stderr, "short write, write %d frames\n", rc);
        }
    }  
	if (close(fd) != 0){
		fprintf(stderr, "close error\n");
        return -1;
	}
	free(buffer);
	return 0;
}
int close_alse(snd_pcm_t *handle){
    /* allow any pending sound samples to be transferred */
    snd_pcm_drain(handle);
    snd_pcm_close(handle);

    return 0;
}

//int main(int argc, char* argv[]){
int testvioce(void){	
	snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
	snd_pcm_uframes_t frames;
	unsigned int period;
	
	handle = alsa_init(params, frames,&period);

	exce_alsa(handle, 32, period, 5000000);

	close_alse(handle);
	
	return 0;
	
}