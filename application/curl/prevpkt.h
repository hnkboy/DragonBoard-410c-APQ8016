#ifndef __PPKT
#define __PPKT

void pkt_send(char *pbuf, int lent);
int mqueue_send2pkt(const char *buf,long len);
void *prevpktmain(void*p);


void prevpkt_tlvproc(char *arg,char len);
void prevpkt_mq_fcgiwrite(void);
#endif
