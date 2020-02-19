#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include "../../include/base/list.h"
#include "../../include/base/hali_base.h"
#include "../../include/base/hali_error.h"
#include "../../include/base/hali_thread.h"
#include "../../include/base/hali_modules.h"
#include "../../include/base/hali_cli.h"

HALI_CLI_PROC_PF apfclifunc[HALI_CLI_MAX];

char *g_clistr[HALI_CLI_MAX]=
{

	[HALI_CLI_DISPLAY_ZIGBEEDEV]    =   "devls",
    [HALI_CLI_DISPLAY_HELP]    =   "help",
    [HALI_CLI_DISPLAY_DEVPKTBUG]    =   "debug",

};


API ulong hali_cli_msgcallbackproc(int fd)
{

    int iretcn;

    int tmp;
    ulong ulret = ERROR_SUCCESS;
    uchar buf[BUFSIZ];
    iretcn = read(fd,buf,sizeof(buf));
    char *cmp;
    if (iretcn > 0)
    {
        buf[iretcn] = '\0';

        for (tmp = 0; tmp < HALI_CLI_MAX; tmp++)
        {
            cmp = NULL;

            if ( NULL != g_clistr[tmp])
            {
                cmp = strstr(buf, g_clistr[tmp]);
            }
            if (NULL != cmp)
            {
                apfclifunc[tmp](cmp);
                break;
            }

        }

    }
    return ulret;

}


API void hali_cli_reg(uint type, HALI_CLI_PROC_PF pfunc)
{
    apfclifunc[type] = pfunc;
    return;
}
API void hali_cli_printfhelpmsg(char *pstr)
{
    uint uindex;
    for (uindex = 0; uindex < HALI_CLI_MAX; uindex++)
    {
        printf("CMD: %s \n",g_clistr[uindex]);
    }
    return;
}

#ifdef __cplusplus
}
#endif



