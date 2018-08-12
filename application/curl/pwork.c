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
	printf("hello work thread\n");
}
extern unsigned char gsm_flag;
void *workmain(void*p){
	int signum;
    sigset_t gset;
    gset = get_sigset();
    myfunc();
	while(1){
/*
        if (1 ==gsm_flag)
        {
	        printf("gsm_flag == 1\n");
            playback_wav("/root/opendoor.wav");
            gsm_flag = 0;
                
            
        }
        usleep(100);
*/
		sigwait(&gset,&signum);
		if(SIGUSR1==signum)
            myfunc();
		if(SIGUSR2==signum)
		{
            printf("Iwillsleep2secondandexit\n");
            sleep(2);
            break;
		}
	}
}
