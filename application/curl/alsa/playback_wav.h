#ifndef _ALSA_WAV_H
#define _ALSA_WAV_H

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>
#include <stdio.h>
int set_pcm_play(FILE *fp);
int playback_wav(char *path);


#endif
