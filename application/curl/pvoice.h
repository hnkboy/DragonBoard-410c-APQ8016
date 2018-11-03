#ifndef __PVOICE_
#define __PVOICE_

extern int g_msid;
void quemsg_snd_voice(char *pbuf ,char *strvolume);
void quemsg_snd_exit(void);
void *voicemain(void*p);
int mplayer_slave_createfifo(void);
int mplayer_slave_set(void); 
int mplayer_slave_snd(char *arg);
void mplayer_slave_setvolume(int num);
void mplayer_slave_exit(void);
#endif
