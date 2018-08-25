#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<pthread.h>
#include<time.h>
#include "homemain.h"
#include "playback.h"
#include "playback_wav.h"

#include <curl/curl.h>
#include "json.h"
#include "getinmemory.h"
#include "postcallback.h"
#include "queue.h"

static void myfunc()
{
	printf("hello work thread\n");
}
extern unsigned char gsm_flag;
void *workmain(void*p){
	int signum;
    char buf[40];
    sigset_t gset;
    gset = get_sigset();
    myfunc();
    while(1){
        (void)quemsg_rcv(200,buf);
        printf("rcv queue msg: %s \n",buf);
        if(strcmp(buf,"exit")==0){
             queue_fini();
             printf("work thread exit\n");
             break;
        }
    }
}
