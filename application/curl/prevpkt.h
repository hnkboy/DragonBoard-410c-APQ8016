#ifndef __PPKT
#define __PPKT

int mqueue_send2pkt(const char *buf,long len);
void *prevpktmain(void*p);
#endif
