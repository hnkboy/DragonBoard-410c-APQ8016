#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>


#include "../../include/base/list.h"
#include "../../include/base/hali_base.h"
#include "../../include/base/hali_error.h"
#include "../../include/base/hali_thread.h"
#include "../../include/base/hali_timer.h"
#include "../../include/base/hali_modules.h"
#include "../../include/base/hali_sem.h"

#include "../../scenarios/include/hali_mosquitto.h"


/*basic init*/
ulong hali_main_baseinit()
{
    ulong ulret = ERROR_FAILED;
    /*初始化挂节点*/
    hali_hanging_init();
    /*初始化线程管理节点*/
    hali_threadmgr_init();
    /*初始化主线程*/
    ulret  = hali_mainthread_init();
    /*初始化信号量*/
    ulret |= hali_sem_init();
    /*初始化连接*/
    ulret |= hali_mosquitto_init();

    return ulret;
}
/*basic init*/
void hali_main_basefini()
{
    /*去初始化信号量*/
    hali_sem_fini();
    /*去初始化主线程*/
    hali_mainthread_fini();
    /*去初始化线程管理节点*/
    hali_threadmgr_fini();

    /*去初始化连接*/
    hali_mosquitto_fini();

    /*去初始化挂节点*/
    hali_hanging_fini();

    return;
}

/*定制的初始化*/
ulong hali_main_custominit()
{
    ulong ulret = ERROR_FAILED;

    /*初始化各个线程*/
    ulret = hali_inithread_init();

    /*初始化各业务模块*/
    ulret |= hali_module_init();

    return ulret;
}
/*定制的去初始化*/
void hali_main_customfini()
{
    /*去初始化各业务模块*/
    hali_module_fini();
    /*去初始化各个线程*/
    hali_inithread_fini();


    return;
}


/*线程epoll回调的处理*/
void hali_main_proccess(void)
{
    pthread_t tpthreadid = pthread_self();
    int iepollfd = -1;
    iepollfd = hali_threadmgr_getcurepfd();
    if (-1 != iepollfd)
    {
        hali_epoll_schedule(iepollfd);
    }
    return;
}

API void hali_main_thrdwait(void)
{
    int icnt;
    for (icnt = 0; icnt < g_uitollthrdnum; icnt++)
    {
        hali_sem_wait(HALI_SEM_THRDSYN);
    }
    return;
}




int main()
{
    ulong ulret = ERROR_FAILED;

    ulret = hali_main_baseinit();

    if (ERROR_SUCCESS == ulret)
    {
        printf("basic init success\n\r");
        ulret = hali_main_custominit();
        /**/
    }
    else
    {
        printf("basic init faild\n\r");

    }
    /*等到各线程初始化完毕*/
    hali_main_thrdwait();

    if (ERROR_SUCCESS == ulret)
    {

        /**/
        printf("basic custominit success\n\r");
        hali_main_proccess();
    }
    else
    {
        printf("basic custominit faild\n\r");

    }
    hali_main_customfini();
    hali_main_basefini();


    return (int)ulret;
}


