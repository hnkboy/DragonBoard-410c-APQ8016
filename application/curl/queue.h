#ifndef __PQUE
#define __PQUE
int queue_init();
int get_queid();
int quemsg_snd(long type,char *a,int len);
int quemsg_rcv(long type,char *aumsg);
void queue_fini();

#endif
