#ifndef __PVOICE_
#define __PVOICE_

extern int g_msid;
void quemsg_snd_voice(char *pbuf ,char *strvolume);
void quemsg_snd_exit(void);
void *voicemain(void*p);
#endif
