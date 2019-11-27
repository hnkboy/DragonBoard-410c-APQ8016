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

/**
 *  功能描述:
 *  @param param1 epoll fd

 *
 *  @return
 */
HALI_EPOLLDATA_NODE_S *hali_epoll_getepdatalist(SL_HEAD_S *pstfdlist, int fd)
{
    HALI_EPOLLDATA_NODE_S *pstepdata = NULL;
	SL_NODE_S *pstnode = NULL;
	SL_NODE_S *pstnext = NULL;

    SL_FOREACH_SAFE(pstfdlist,pstnode,pstnext)
	{

		pstepdata = SL_ENTRY(pstnode,HALI_EPOLLDATA_NODE_S, stnode);

		if(fd == pstepdata->fd)
		{
			return pstepdata;
		}
	}
    pstepdata = NULL;
    return pstepdata;
}

/**
 *  功能描述:
 *  @param param1 epoll fd

 *
 *  @return
 */
ulong hali_epoll_addepdatalist(SL_HEAD_S *pstfdlist, HALI_EPOLLDATA_NODE_S *pstepdata)
{
    ulong ulerrcode = ERROR_FAILED;
    HALI_THREAD_NODE_S *pstcurthrdinfo = NULL;
    HALI_EPOLLDATA_NODE_S *pstepdatatmp = NULL;

    int iret = -1;
    struct epoll_event event;
    pstepdatatmp = hali_epoll_getepdatalist(pstfdlist,pstepdata->fd);
    if (NULL == pstepdatatmp)   /*没查到就添加*/
    {
        sl_addafter(pstfdlist, &pstepdata->stnode);
        ulerrcode = ERROR_SUCCESS;
    }
    return ulerrcode;
}
/**
 *  功能描述:
 *  @param param1 epoll fd

 *
 *  @return
 */
ulong hali_epoll_delepdatalist(SL_HEAD_S *pstfdlist, int fd)
{
    ulong ulerrcode = ERROR_FAILED;
    HALI_THREAD_NODE_S *pstcurthrdinfo = NULL;
    HALI_EPOLLDATA_NODE_S *pstepdata = NULL;

    struct epoll_event event;
    pstepdata = hali_epoll_getepdatalist(pstfdlist,fd);
    if (NULL != pstepdata)
    {
        sl_del(pstfdlist, &pstepdata->stnode);
        ulerrcode = ERROR_SUCCESS;
    }
    return ulerrcode;
}


/**
 *  功能描述:
 *  @param iepollfd
 *  @return
 */
void hali_epoll_schedule(int iepollfd)
{

    struct epoll_event event;
    HALI_EPOLLDATA_NODE_S *pstepolldata = NULL;
    int ieventcnt;
    for( ; ; )
    {
        ieventcnt = epoll_wait(iepollfd, &event, 1, -1);
        if (0 > ieventcnt)
        {
            break;
        }
        if (event.events & EPOLLIN)
        {
            pstepolldata = event.data.ptr;
            if (NULL != pstepolldata)
            {
                pstepolldata->pfcallbackfun(pstepolldata->fd);
            }
        }

    }
    return;
}
/**
 *  功能描述: 增加epoll监听事件
 *  @param param1 epoll fd
 *  @param param2 监听fd
 *  @param param3 事件的回调处理
 *
 *  @return
 */

ulong hali_epoll_add(int fd,  ulong (*pfcallback)(int))
{
    ulong ulerrcode = ERROR_FAILED;
    HALI_THREAD_NODE_S *pstcurthrdinfo = NULL;
    HALI_EPOLLDATA_NODE_S *pstepdata = NULL;

    int iret = -1;
    struct epoll_event event;
    if (NULL == pfcallback)
    {
        return ulerrcode;
    }

    pstcurthrdinfo = hali_threadmgr_getcurthrdnode();
    if ((NULL != pstcurthrdinfo) && (-1 != pstcurthrdinfo->stthrdep.iepollfd) )
    {
        /*创建内存空间*/
        pstepdata = (HALI_EPOLLDATA_NODE_S *)malloc(sizeof(HALI_EPOLLDATA_NODE_S));
        if (NULL != pstepdata)
        {
            memset(pstepdata,0,sizeof(HALI_EPOLLDATA_NODE_S));
            /*填充数据*/
            pstepdata->fd = fd;
            pstepdata->pfcallbackfun = pfcallback;
            /*加入链表*/
            ulerrcode = hali_epoll_addepdatalist(&pstcurthrdinfo->stthrdep.stfdlist, pstepdata);
        }

        if (ERROR_SUCCESS == ulerrcode)
        {
            event.events=EPOLLIN;
            event.data.ptr = (void *)pstepdata;
            iret = epoll_ctl(pstcurthrdinfo->stthrdep.iepollfd, EPOLL_CTL_ADD, fd, &event);
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
ulong hali_epoll_del(int fd)
{
    ulong ulerrcode = ERROR_FAILED;
    HALI_THREAD_NODE_S *pstcurthrdinfo = NULL;
    HALI_EPOLLDATA_NODE_S *pstepdata = NULL;
    struct epoll_event event;

    pstcurthrdinfo = hali_threadmgr_getcurthrdnode();
    if ((NULL != pstcurthrdinfo) && (-1 != pstcurthrdinfo->stthrdep.iepollfd))
    {
        (void)epoll_ctl(pstcurthrdinfo->stthrdep.iepollfd, EPOLL_CTL_DEL, fd, &event);
        pstepdata = hali_epoll_getepdatalist(&pstcurthrdinfo->stthrdep.stfdlist, fd);
        if(NULL != pstepdata)
        {
            (void)hali_epoll_delepdatalist(&pstcurthrdinfo->stthrdep.stfdlist,fd);
            free(pstepdata);
        }
        ulerrcode = ERROR_SUCCESS;
    }

    return ulerrcode;
}



#ifdef __cplusplus
}
#endif



