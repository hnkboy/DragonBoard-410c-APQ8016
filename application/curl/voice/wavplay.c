#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <endian.h>

struct wav_info {
  int format_id;
  int rate;
  int channels;
  int bps;
  int bytes_per_sample;
  char *map;
  uint32_t map_len;
  char *fmt;
  uint32_t fmt_len;
  char *data;
  uint32_t data_len;
  char *end;
};

struct wav_info wi;

int16_t *mmap_wav(char *wavfile, struct wav_info *swi) {
  int fd=open(wavfile, O_RDONLY);
  uint32_t *tmpint;
  uint16_t *tmpsint;
  off_t fsize;
  char *map;


  if(fd<0) {
    fprintf(stderr, "Unable to open %s, quitting...\n", wavfile);
    exit(-1);
  }

  fsize=lseek(fd, 0, SEEK_END);
  if(fsize<1024) {
    fprintf(stderr, "File %s is too short (%d bytes).", wavfile, (int)fsize); 
    exit(-2);
  }
  lseek(fd, 0, SEEK_SET);

  map=mmap(NULL, fsize, PROT_READ, MAP_PRIVATE, fd, 0);
  if(!map) {
    fprintf(stderr, "Unable to mmap() %s, bogus...\n", wavfile);
    exit(-3);
  }

  swi->map=map;
  swi->map_len=fsize;

  if(memcmp(map, "RIFF", 4)||memcmp(map+8, "WAVEfmt ", 8)) {
    fprintf(stderr, "WAV file is not RIFF-based.\n");
    exit(-4);
  }

  // retrieve all info using tmpint for 32-bits and tmpsint for 16bits (and straight addressing for uint8)

  swi->fmt=map+20;
  tmpsint=(uint16_t *)swi->fmt;
  swi->format_id=tmpsint[0];
  if(swi->format_id!=1) {
    fprintf(stderr, "WAV file is not PCM, type = %04x\n", swi->format_id);
    exit(-5);
  }
  swi->channels=le16toh(tmpsint[1]);
  swi->bytes_per_sample=le16toh(tmpsint[6]);
  swi->bps=le16toh(tmpsint[7]);

  tmpint=(uint32_t *)map;
  swi->fmt_len=le32toh(tmpint[4]);
  swi->data=map+28+swi->fmt_len;
  tmpint=(uint32_t *)swi->data;
  swi->data_len=le32toh(tmpint[-1]);
  tmpint=(uint32_t *)swi->fmt+1;
  swi->rate=le16toh(*tmpint);

  return((uint16_t *)swi->data);
}


int main (int argc, char *argv[]) {
  int i, err, bps_format;
  snd_pcm_t *playback_handle;
  snd_pcm_hw_params_t *hw_params;
  int16_t *data;
  char *alsa_dev;
  char *file_to_play="tv.wav";		// I know you have a The Ventures' song laying around.


  if(argc>1) {
    if(!strcasecmp(argv[1], "--help")) {
      printf("%s <wavfile> <alsa-device>\n", argv[0]);
      exit(1);
    }
    else file_to_play=argv[1];
  }
  
  if(argc<3) alsa_dev="default";
  else alsa_dev=argv[2];

  if ((err = snd_pcm_open (&playback_handle, alsa_dev, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
    fprintf (stderr, "cannot open audio device %s (%s)\n", alsa_dev, snd_strerror (err));
    exit (1);
  }


  if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
    fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n", snd_strerror (err));
    exit (1);
  }

  data=mmap_wav(file_to_play, &wi);

  if ((err = snd_pcm_hw_params_any (playback_handle, hw_params)) < 0) {
    fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n", snd_strerror (err));
    exit (1);
  }

  if ((err = snd_pcm_hw_params_set_access (playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
    fprintf (stderr, "cannot set access type (%s)\n", snd_strerror (err));
    exit (1);
  }

  if(wi.bps==16) bps_format=SND_PCM_FORMAT_S16_LE;
  else if(wi.bps==8) bps_format=SND_PCM_FORMAT_S8;
  else if(wi.bps==24) bps_format=SND_PCM_FORMAT_S24_LE;
  else if(wi.bps==32) bps_format=SND_PCM_FORMAT_S32_LE;
  else {
    fprintf (stderr, "Unsupported bit depth (%d)\n", bps_format);
    exit (1);
  }

  if ((err = snd_pcm_hw_params_set_format (playback_handle, hw_params, bps_format)) < 0) {
    fprintf (stderr, "Cannot set sample format (%s)\n", snd_strerror (err));
    exit (1);
  }

  if ((err = snd_pcm_hw_params_set_rate_near (playback_handle, hw_params, &wi.rate, 0)) < 0) {
    fprintf (stderr, "Cannot set sample rate to %d (%s)\n", wi.rate, snd_strerror (err));
    exit (1);
  }

  if ((err = snd_pcm_hw_params_set_channels (playback_handle, hw_params, wi.channels)) < 0) {
    fprintf (stderr, "Cannot set channel count to %d (%s)\n", wi.channels, snd_strerror (err));
    exit (1);
  }

  if ((err = snd_pcm_hw_params (playback_handle, hw_params)) < 0) {
    fprintf (stderr, "Cannot set parameters (%s)\n", snd_strerror (err));
    exit (1);
  }

  snd_pcm_hw_params_free (hw_params);

  if ((err = snd_pcm_prepare (playback_handle)) < 0) {
  fprintf (stderr, "Cannot prepare audio interface for use (%s)\n", snd_strerror (err));
  exit (1);
  }

  for(i=0;i<(wi.data_len-2048);i+=2048) {
    if ((err = snd_pcm_writei (playback_handle, wi.data+i, 512)) != 512) {
      fprintf (stderr, "Write to audio interface failed (%s)\n", snd_strerror (err));
      exit (1);
    }
  }

  // commit the remaining of data to alsa
  if(i<wi.data_len) snd_pcm_writei(playback_handle, wi.data+i, (wi.data_len-i)/wi.bytes_per_sample);	// if error just ignore, it's the last frame anyway!

  // flush alsa
  snd_pcm_drain(playback_handle);

  // and we're done.
  snd_pcm_close (playback_handle);

  exit (0);
}
