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
#include "../../include/base/hali_timer.h"


/**
 *  功能描述:
 *  @param param1
 *
 *  @return
 */
int hali_timer_openfd(int iwaits)
{

	struct timespec starttime, stintervaltime;
	struct itimerspec stnewvalue;
    int itimerfd = -1;

    /* timerfd */
   	itimerfd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
    if (itimerfd < 0)
    {
        perror("timerfd_open()");
        return itimerfd;
    }
	starttime.tv_sec = iwaits;
    starttime.tv_nsec = 0;                                //相当于立即到达超时时间
    stintervaltime.tv_sec = iwaits;                       //首次超时后，每一秒超时一次
    stintervaltime.tv_nsec = 0;
    stnewvalue.it_value = starttime;
    stnewvalue.it_interval = stintervaltime;
	/*设置超时时间，且为相对时间*/
    if (timerfd_settime(itimerfd, 0, &stnewvalue, NULL) < 0)
    {
        perror("work timerfd_settime()");
        return itimerfd;
    }

    return itimerfd;
}
/**
 *  功能描述:
 *  @param param1
 *
 *  @return
 */
void hali_timer_closefd(int itimerfd)
{

    close(itimerfd);

    return ;
}
/**
 *  功能描述:
 *  @param param1
 *
 *  @return
 */

ulong hali_timer_create(int iwaits, ulong (*pfcallback)(int),int *pitimerfd)
{
    int itimerfd = -1;
    ulong ulerrcode = ERROR_FAILED;
    int iepollfd = -1;
    /* timerfd */
    itimerfd = hali_timer_openfd(iwaits);
    if (-1 != itimerfd)
    {
        /*epoll*/
        ulerrcode = hali_epoll_add(itimerfd, pfcallback);
        if(ERROR_SUCCESS != ulerrcode)
        {
            hali_timer_closefd(itimerfd);
        }
    }
    *pitimerfd = itimerfd;
    return ulerrcode;
}
/**
 *  功能描述:
 *  @param param1
 *
 *  @return
 */

void hali_timer_destroy(int itimerfd)
{
    ulong ulerrcode = ERROR_FAILED;
    int iepollfd = -1;

    iepollfd = hali_threadmgr_getcurepfd();

    /*epoll*/
    ulerrcode = hali_epoll_del(itimerfd);
    if (ERROR_FAILED == ulerrcode)
    {
        perror("timerfd_epoll del error()");
    }

    /* timerfd */
    hali_timer_closefd(itimerfd);

    return ;
}


#ifdef __cplusplus
}
#endif



