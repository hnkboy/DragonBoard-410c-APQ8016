#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include "../../include/base/list.h"
#include "../../include/base/hali_base.h"
#include "../../include/base/hali_error.h"
#include "../../include/base/hali_thread.h"
#include "../../include/base/hali_epoll.h"



char *g_paucthrdname[HALI_THREADTYPE_MAX] =
{
    [HALI_THREADTYPE_MAIN] = "mainthread",
    [HALI_THREADTYPE_WORK] = "workthread",
    [HALI_THREADTYPE_SVR ] = "svrthread ",
};
/**
 *  功能描述: 增加epoll监听事件
 *  @param param1 epoll fd
 *  @param param2 监听fd
 *  @param param3 事件的回调处理
 *
 *  @return
 */

ulong hali_thread_addmq2epoll(HALI_THREAD_NODE_S *pstcurthrdinfo,  void (*pfcallback)(int))
{
    ulong ulerrcode = ERROR_FAILED;
    HALI_EPOLLDATA_NODE_S *pstepdata = NULL;

    int iret = -1;
    struct epoll_event event;
    if (NULL == pfcallback)
    {
        return ulerrcode;
    }
    if ((NULL != pstcurthrdinfo) &&
        (-1 != pstcurthrdinfo->stthrdep.iepollfd) &&
        (-1 != pstcurthrdinfo->imqfd))
    {
        /*创建内存空间*/
        pstepdata = (HALI_EPOLLDATA_NODE_S *)malloc(sizeof(HALI_EPOLLDATA_NODE_S));
        if (NULL != pstepdata)
        {
            /*填充数据*/
            pstepdata->fd = pstcurthrdinfo->imqfd;
            pstepdata->pfcallbackfun = pfcallback;
            /*加入链表*/
            ulerrcode = hali_epoll_addepdatalist(&pstcurthrdinfo->stthrdep.stfdlist, pstepdata);
        }

        if (ERROR_SUCCESS == ulerrcode)
        {
            event.events=EPOLLIN;
            event.data.ptr = (void *)pstepdata;
            iret = epoll_ctl(pstcurthrdinfo->stthrdep.iepollfd, EPOLL_CTL_ADD, pstcurthrdinfo->imqfd, &event);
            if (-1 != iret)
            {
                ulerrcode = ERROR_SUCCESS;
            }
        }
        else
        {
            free(pstepdata);
        }
    }
    return ulerrcode;
}

/**
 *  功能描述: 删除epoll监听
 *  @param param1 epoll fd
 *  @param param2 监听fd
 *  @param param3 事件的回调处理
 *
 *  @return
 */
ulong hali_thread_delmq4epoll(HALI_THREAD_NODE_S *pstcurthrdinfo)
{
    ulong ulerrcode = ERROR_FAILED;
    HALI_EPOLLDATA_NODE_S *pstepdata = NULL;
    struct epoll_event event;

    if ((NULL != pstcurthrdinfo) && (-1 != pstcurthrdinfo->stthrdep.iepollfd))
    {
        (void)epoll_ctl(pstcurthrdinfo->stthrdep.iepollfd, EPOLL_CTL_DEL, pstcurthrdinfo->imqfd, &event);
        pstepdata = hali_epoll_getepdatalist(&pstcurthrdinfo->stthrdep.stfdlist, pstcurthrdinfo->imqfd);
        if(NULL != pstepdata)
        {
            (void)hali_epoll_delepdatalist(&pstcurthrdinfo->stthrdep.stfdlist,pstcurthrdinfo->imqfd);
            free(pstepdata);
        }
        ulerrcode = ERROR_SUCCESS;
    }

    return ulerrcode;
}



/*线程epoll回调的处理*/
API ulong hali_thread_mqmsgcallbackproc(int fd)
{

    int iret = -1;
    char aucmqrbuf[BUFSIZ];
    uint uimqval;
    HALI_THREAD_NODE_S *pstthrdnode;
    pfmsgcallbackfun pfcallbackfun;
    char *pucbuf = NULL;
    ulong ulret = ERROR_SUCCESS;

    pstthrdnode = hali_threadmgr_getcurthrdnode();

    if (NULL == pstthrdnode)
    {
        return ulret;
    }

    memset(aucmqrbuf,0,BUFSIZ);
    //printf("%s rcv mqueue msg,prio:%u\n",pstthrdnode->szthrdname,uimqval);

    /*消息队列的处理*/
    if (pstthrdnode->imqfd == fd)
    {

        iret = mq_receive(pstthrdnode->imqfd, aucmqrbuf, BUFSIZ, &uimqval);
        if (-1 == iret)
        {

            printf("%s mq_receive err()",pstthrdnode->szthrdname);

        }
        else
        {
            pucbuf = aucmqrbuf;
            pfcallbackfun = pstthrdnode->pfmsgcallbackfun;
            ulret = pfcallbackfun(pucbuf, iret);
            //printf("%s rcv mqueue msg %s ,prio:%u\n",pstthrdnode->szthrdname,aucmqrbuf,uimqval);

        }
    }
    return ulret;

}


/*按照类型和数量创建线程*/
ulong hali_thread_create(uint uithrType,
                                uint uiindex,
                                pfthreadtask_init pfthreadinit,
                                pfmsgcallbackfun pfcallbackfun,
                                pfthreadtask_fini pfthreadfini)
{
    HALI_THREAD_NODE_S *pstthrdnode = NULL;
    int iepollfd = -1;
    int imqfd = -1;
    uint uithreadseqnum;
    uchar szname[THRDNAMELEN_MAX +1];
    uchar szmqname[THRDNAMELEN_MAX +1];
    ulong ulerrcode = ERROR_FAILED;
    int iret = -1;
    pthread_t threadid;


    szname[0]='\0';
    szmqname[0]='\0';

    if (HALI_THREADTYPE_MAX <= uithrType)
    {
        return ulerrcode;
    }
    /*换算线程索引         */
    uithreadseqnum = hali_thread_idcacl(uithrType,uiindex);
    /*查找线程列表是否已经创建*/
    pstthrdnode = hali_threadmgr_getnode(uithreadseqnum);
    if(NULL == pstthrdnode)
    {
        pstthrdnode = hali_threadmgr_creat(uithreadseqnum);
    }

    if(NULL != pstthrdnode)
    {
        /*算出线程名字*/
        sprintf((char *)szname,"%s-%u",g_paucthrdname[uithrType],uiindex);
        sprintf((char *)szmqname,"/%s-%u",g_paucthrdname[uithrType],uiindex);
        strcpy((char *)pstthrdnode->szthrdname,(char *)szname);
        pstthrdnode->pfdelethandler = pfthreadfini;
        pstthrdnode->pfmsgcallbackfun =  pfcallbackfun;
        pstthrdnode->uithreadseqnum = uithreadseqnum;


        /*创建epoll挂回调     */
        iepollfd =  epoll_create(1);
        if (-1 != iepollfd)
        {
            pstthrdnode->stthrdep.iepollfd = iepollfd;
            sl_init(&pstthrdnode->stthrdep.stfdlist);

            imqfd = mq_open((char *)szmqname, O_RDWR|O_CREAT, 0600, NULL);
            if (-1 != imqfd)
            {
                pstthrdnode->imqfd = imqfd;

                ulerrcode = hali_thread_addmq2epoll(pstthrdnode, hali_thread_mqmsgcallbackproc);
            }
            if (ERROR_SUCCESS == ulerrcode)
            {
                iret = pthread_create(&threadid,NULL,pfthreadinit,(void *)pstthrdnode);
            }
            else
            {
                iret = -1;
            }
        }
        if (-1 == iret)
        {
            if (-1 != iepollfd)
            {
                (void)close(iepollfd);
            }
            if (-1 != imqfd)
            {
                (void)mq_close(imqfd);
            }
            hali_threadmgr_destroy(pstthrdnode);
        }
        else
        {
            ulerrcode = ERROR_SUCCESS;
        }
    }
    return ulerrcode;
}

/**
 *  功能描述: 线程节点去初始化
 *  @param pthreadnode
 *
 *  @return
 */
void hali_thread_destroy(void *pthreadnode)
{

    HALI_THREAD_NODE_S *pstthrdnode = (HALI_THREAD_NODE_S *)pthreadnode;

    if (NULL == pstthrdnode)
    {
        return;
    }

    if (-1 != pstthrdnode->imqfd)
    {
        (void)hali_thread_delmq4epoll(pstthrdnode);
        (void)mq_close(pstthrdnode->imqfd);
    }
    if (-1 != pstthrdnode->stthrdep.iepollfd)
    {
        (void)close(pstthrdnode->stthrdep.iepollfd);
    }

    hali_threadmgr_del4list(pstthrdnode->uithreadseqnum);
    hali_threadmgr_destroy(pstthrdnode);
    return;
}

#ifdef __cplusplus
}
#endif



