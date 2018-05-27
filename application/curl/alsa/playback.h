/*
 * list3.c
 * read from input(STDIN) and writes to the default PCM device
 * for 5 seconds of data

 ./list3 < /dev/urandom
 */

/* use the newer ALSA_API */
#ifndef _ALSA_JIALI_H
#define _ALSA_JIALI_H

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>

snd_pcm_t *alsa_init(snd_pcm_hw_params_t *params, 
						snd_pcm_uframes_t frames,
						unsigned int *period);

int exce_alsa(snd_pcm_t *handle,
				snd_pcm_uframes_t frames,
			    unsigned int period,	
				int timeus);

int close_alse(snd_pcm_t *handle);
 
int testvioce(void);

#endif