#ifndef __PPKT
#define __PPKT

void pkt_send(char *pbuf, int lent);
int mqueue_send2pkt(const char *buf,long len);
void *prevpktmain(void*p);
#endif
