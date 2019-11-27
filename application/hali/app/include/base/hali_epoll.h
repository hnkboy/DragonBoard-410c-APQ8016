
#ifndef __HALIEPOLL__
#define __HALIEPOLL__



typedef ulong (*HALI_EPCALLFUN_PF)(int);


typedef struct taghali_epolldata_st
{
    SL_NODE_S stnode;
    int       fd;
    HALI_EPCALLFUN_PF pfcallbackfun;
}HALI_EPOLLDATA_NODE_S;

ulong hali_epoll_add(int fd,  ulong (*pfcallback)(int));
ulong hali_epoll_del(int fd);


void hali_epoll_schedule(int iepollfd);



#endif

