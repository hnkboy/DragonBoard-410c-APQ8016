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
#include "voiceclient.h"

struct tm nowTime;

static void myfunc()
{
	printf("hello voice thread\n");
}
extern unsigned char gsm_flag;
unsigned char gsm_mor_flag;
struct tagmor_s {
    char mor_flag;
    char mor_done;    
}gmor_s;


void *voicemain(void*p){
	int signum;
    sigset_t gset;
    gset = get_sigset();
    myfunc();
	while(1){

        if (1 ==gsm_flag)
        {
	        printf("gsm_flag == 1\n");
            voicesend("opendoor.wav");
            gsm_flag = 0;
                
            
        }
       //usleep(100);
	    sleep(1);

        if (1 == getNowTime())
        {
            voicesend("opendoor.wav");
        }


        if (-1 == getexitsigle())
        {
           
            break;    
        }
	}
}

int getNowTime(void)
{

    struct timespec time;
    char current[1024];
    clock_gettime(CLOCK_REALTIME, &time);  //获取相对于1970到现在的秒数
    localtime_r(&time.tv_sec, &nowTime);

    if(nowTime.tm_hour < 16 )
    {
        nowTime.tm_hour += 8;    
    }
    else 
    {
        nowTime.tm_hour -= 16;    
    }
        
    sprintf(current, "%04d%02d%02d%02d:%02d:%02d", nowTime.tm_year + 1900, nowTime.tm_mon+1, nowTime.tm_mday, 
      nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);
    printf("%s\n",current);
    if (nowTime.tm_hour >6 && nowTime.tm_min >30 )
    {
         gmor_s.mor_flag = 1;
         if (0 == gmor_s.mor_done )
         {
             
            printf("morning \n");
            gmor_s.mor_done = 1;
            return 1;
         }
    }
    else 
    {
         gmor_s.mor_flag = 0;          
         gmor_s.mor_done = 0;
    }

    return 0;
}
