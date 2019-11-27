
#ifndef __HALITHREADMSG_
#define __HALITHREADMSG_



#define HALI_THRDMSGID(uitype,uimodule,uindex)  ((uitype<<24) | (uimodule<<16)| (uindex))
#define HALI_THRDMSGGETMOD(uimsgid)  ((uimsgid>>16) & (0xff))

/*main 发送定义*/



#define HALI_THRDMSGTYPE_QUIT   HALI_THRDMSGID(HALI_THREADTYPE_MAIN, HALI_MODULE_BASIC,0x1)
#define HALI_THRDMSGTYPE_PRINT  HALI_THRDMSGID(HALI_THREADTYPE_MAIN, HALI_MODULE_BASIC,0x2)



typedef struct stthreadmsgdata
{
    uint uimsgType;
    uint uilenth;
    void *pbuf;
}HALI_THRDMSG_DATA_S;



ulong hali_threadmsg_sendmsg(uint uithrType,
                                      uint uiindex,
                                      uint uimsgtype,
                                      void *psendbuf,
                                      uint uilenth);


#endif

