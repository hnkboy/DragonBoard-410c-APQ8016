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
static void myfunc()
{
	printf("hello listen thread\n");
}
void *listenmain(void*p){
	int signum;
    sigset_t gset;
    gset = get_sigset();
    myfunc();
	while(1){
		sigwait(&gset,&signum);
		if(SIGUSR1==signum)
            myfunc();
		if(SIGUSR2==signum)
		{
            printf("plisten Iwillsleep2secondandexit\n");
            sleep(2);
            break;
		}
	}
}
