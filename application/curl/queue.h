#ifndef __PQUE
#define __PQUE
int queue_init(unsigned char     num);
int quemsg_snd(int msid, long type,char *a,int len);
int quemsg_rcv(int msid, long type,char *aumsg);
void queue_fini(int msid);

#endif
