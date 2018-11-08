#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<pthread.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include "homemain.h"
#include "alsaplay.h"
#include "alsaplay_wav.h"

#include <curl/curl.h>
#include "json.h"
#include "getinmemory.h"
#include "postcallback.h"
#include "queue.h"
#include "pvoice.h"


int cmdfifofd = -1;
int g_msid = -1;
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
    quemsg_snd(g_msid,200,buf,40);
}
void quemsg_snd_exit()
{
	(void)quemsg_snd(g_msid, 200,"exit",4);
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
            // playback_mp3(svstr,strvolume);
            //mplayer_slave_setvolume(atoi(strvolume));
            alsa_set_volume(atoi(strvolume));
            (void)mplayer_slave_snd(svstr);
        }
        else
        {
        
            printf("playback_wav\n");
            //alsa_set_volume(atoi(strvolume));
            //mplayer_slave_setvolume(atoi(strvolume));
            alsa_set_volume(atoi(strvolume));
            //playback_wav(pbuf);    
            (void)mplayer_slave_snd(pbuf);
        }
    }
}

void *voicemain(void*p){
	int signum;
    char buf[40];
    sigset_t gset;
    gset = get_sigset();
    myfunc();
    if (0 != mplayer_slave_set())
    {
        return 0;
    }
	while(1){
		/* 队列初始化 */
		g_msid = queue_init(123);  
		if (g_msid == -1)
	    {
			return NULL;
		}
		(void)quemsg_rcv(g_msid,200,buf);
		//printf("rcv queue msg: %s \n",buf);
		if(strcmp(buf,"exit")==0)
		{
			queue_fini(g_msid);
            mplayer_slave_exit();
            printf("voice thread exit\n");

			break;
		}
		else
		{
			playvoice(buf,&buf[20]);
		}

	}
}
int mplayer_slave_set(void) 
{
	char *pathname = "./mplayer/cmdfifo";
#if 1
  	if(access(pathname, F_OK) == 0)
	{
	//	printf("cmdfifo exist\n");
		unlink(pathname);
		mkfifo(pathname, 0777);
	}
	else
	{
		mkfifo(pathname, 0777);
	}
#endif
#if 1
    pid_t pid = fork();
    if (pid < 0)
    {
        printf("fork error");
    }
    else if ( pid == 0)
    {
    
        execlp("mplayer", "mplayer", "-slave", "-quiet", "-idle", "-input", "file=./mplayer/cmdfifo", NULL, NULL);	//命名管道cmdfifo有命令数据，mplayer会自动读取
        return 1;
    }
    else 
    {
        sleep(1);
        cmdfifofd = open(pathname, O_WRONLY | O_NONBLOCK);
        if(cmdfifofd < 0)
        {
            printf("mplayer open file error\n\r");
            return -1;
        }
    }
#endif 
    return 0;

}
int mplayer_slave_snd(char *arg)
{

	char pathname[50] = {0};
    if (cmdfifofd > -1)
    {
	    strcat(pathname, "loadfile ");
        // strcat(pathname,"./voice/");
        strcat(pathname,arg);
        strcat(pathname,"\n");
        printf("mplayer pathname = %s\n\r",pathname);
        write(cmdfifofd,pathname,strlen(pathname));
        return 0;
    }
    else 
    {
        printf("mplaer-slave error send \n\r");
        return -1;
    }
}
void mplayer_slave_setvolume(int num)
{
    char strtmp[50];
    if (cmdfifofd > -1)
    {
        strtmp[0]= '\0';
        sprintf(strtmp,"volume %d 1\n",num);
        printf("mplayer  snd %s",strtmp);
        write(cmdfifofd, strtmp,strlen(strtmp));
    }
}
void mplayer_slave_exit(void)
{
    if (cmdfifofd > -1)
    {
        write(cmdfifofd, "quit\n",5);
    }
}
