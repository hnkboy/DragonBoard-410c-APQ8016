#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<pthread.h>
#include<time.h>
#include "homemain.h"
#include "alsaplay.h"
#include "alsaplay_wav.h"

#include <curl/curl.h>
#include "json.h"
#include "getinmemory.h"
#include "postcallback.h"
#include "queue.h"

static void myfunc()
{
	printf("hello voice thread\n");
}


void quemsg_snd_voice(char *pbuf ,char *strvolume)
{
	char buf[40];
    memcpy(buf,pbuf,strlen(pbuf));
    buf[strlen(pbuf)] = '\0';
    memcpy(&buf[20],strvolume,strlen(strvolume));
    buf[strlen(strvolume)+20] = '\0';
    printf("%s,%s\n",buf,&buf[20]);
    //quemsg_snd(200,pbuf,strlen(pbuf));
    quemsg_snd(200,buf,40);
}

void playvoice(char *pbuf ,char *strvolume)
{
    char svstr[100] = {0};
    if (0 != pbuf) 
    {
        char *p = strstr(pbuf,"mp3");
        if (NULL != p)
        { 
            printf("playback_mp3\n");
            sprintf(svstr,"./voice/%s",pbuf);
            //playback_wav(svstr,strvolume);    
            playback_mp3(svstr,strvolume);
        }
        else
        {
        
            printf("playback_wav\n");
            alsa_set_volume(atoi(strvolume));
            playback_wav(pbuf);    
        }
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
			playvoice(buf,&buf[20]);
		}

	}
}
