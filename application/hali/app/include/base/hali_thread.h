
#ifndef __HALITHREAD_
#define __HALITHREAD_

#define THRDNAMELEN_MAX 20


extern uint g_uitollthrdnum;

/*线程类型*/
typedef enum enthreadtype
{
    HALI_THREADTYPE_MAIN = 0,
    HALI_THREADTYPE_WORK,
    HALI_THREADTYPE_SVR,
    HALI_THREADTYPE_MAX
}HALI_THREADTYPE_E;

/*模块类型*/
typedef enum enmoduletype
{
    HALI_MODULE_BASIC = 0,

    HALI_MODULE_ZIGBEE,
    HALI_MODULE_MAX
}HALI_MODULE_E;


typedef void *(*pfthreadtask_init)(void *pstthreadnode);
typedef void  (*pfthreadtask_fini)(void *pstthreadnode);
typedef ulong (*pfmsgcallbackfun)(char *,int);


typedef struct stthreadepst
{
    int       iepollfd;
    SL_HEAD_S stfdlist;
}HALI_THREAD_EP_S;

typedef struct stthreadprivaldata
{
    SL_NODE_S stnode;
    pthread_t tpthreadid;
    uint      uithreadseqnum;
    int       imqfd;
    uchar     szthrdname[THRDNAMELEN_MAX+1];
    void (*pfdelethandler)(void *);
    ulong (*pfmsgcallbackfun)(char *,int);
    HALI_THREAD_EP_S stthrdep;
}HALI_THREAD_NODE_S;

typedef struct stthreadmgr
{
    SL_HEAD_S sthrdnodehead;
    uint uitypethrdnum;
}HALI_THREAD_MGR_S;


#define hali_thread_idcacl(uitype,uindex)  ((uitype<<24)|(uindex))

#define hali_thread_typecacl(uithreadid)  (uithreadid>>24)







extern char *g_paucthrdname[HALI_THREADTYPE_MAX];

void hali_threadmgr_Init(void);
void hali_threadmgr_fini(void);
HALI_THREAD_NODE_S *hali_threadmgr_getnode(uint uithreadid);
HALI_THREAD_NODE_S *hali_threadmgr_findbythrdid(pthread_t tpthreadid);
HALI_THREAD_NODE_S *hali_threadmgr_getcurthrdnode(void);
int hali_threadmgr_getcurepfd(void);

HALI_THREAD_NODE_S *hali_threadmgr_creat(uint uithreadseqnum);
void hali_threadmgr_destroy(HALI_THREAD_NODE_S *psthreadinfo);


 ulong  hali_threadmgr_add2list(HALI_THREAD_NODE_S *psthreadinfo);
 void hali_threadmgr_del4list(uint uithreadseqnum);

void hali_threadmgr_delall(uint uithrdtype);


/*创建线程初始化*/
ulong hali_inithread_init();
void hali_inithread_fini();
ulong hali_thread_mqmsgcallbackproc(int fd);


ulong hali_mainthread_init(void);
void hali_mainthread_fini(void);


/*个业务线程启动后初始化*/
void *hali_svrthread_init(void *pthreadnode);
void hali_svrthread_fini(void *pthreadnode);
ulong hali_svrthread_msgproc(char *pucbuf, int ilen);

void *hali_workthread_init(void *pstthreadnode);
void hali_workthread_fini(void *pstthreadnode);
ulong hali_workthread_msgproc(char *pucbuf, int ilen);
ulong hali_mainthread_msgproc(char *pucbuf, int ilen);

ulong hali_thread_create(uint uithrType,
                                uint uiindex,
                                pfthreadtask_init pfthreadinit,
                                pfmsgcallbackfun pfmsgcallback,
                                pfthreadtask_fini pfthreadfini);


void hali_thread_epollwait(HALI_THREAD_NODE_S *pstthrdnode);





/*挂节点的初始化*/
void hali_hanging_init(void);
void hali_hanging_fini(void);




#endif

