#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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
	printf("hello voice thread\n");
}


void quemsg_snd_voice(char *pbuf ,unsigned int lenth)
{
    lenth = lenth;
    quemsg_snd(200,pbuf,strlen(pbuf));
}

void playvoice(char *pbuf ,unsigned int lenth)
{
    char svstr[100] = {0};
    if (0 != pbuf) 
    {
        sprintf(svstr,"./voice/%s",pbuf);
        printf("%s\n",pbuf);
        playback_wav(svstr);    
    }
}

void *voicemain(void*p){
	int signum;
    char buf[40];
    sigset_t gset;
    gset = get_sigset();
    myfunc();
	while(1){

		(void)quemsg_rcv(200,buf);
		//printf("rcv queue msg: %s \n",buf);
		if(strcmp(buf,"exit")==0)
		{
			queue_fini();
            printf("voice thread exit\n");
			break;
		}
		else
		{

			playvoice(buf,sizeof(buf));
		}

	}
}
