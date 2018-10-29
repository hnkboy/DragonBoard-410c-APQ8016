#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <mqueue.h>
#include "plisten.h"
#include "prevpkt.h"
#include "puart.h"
#include "pwork.h"
#include "pvoice.h"
#include "alsaplay.h"
#include "alsaplay_wav.h"


#include <curl/curl.h>
#include "json.h"
#include "getinmemory.h"
#include "postcallback.h"
#include "voiceclient.h"
#include "prevpkt.h"
#include "queue.h"

#if 0
pthread_t tlistenid;
pthread_t tuartid;
#endif 
pthread_t tid;
pthread_t tvoiceid;
pthread_t tworkid;
pthread_t trevpktid;
sigset_t set;

int giExit = 0;
void myfunc()
{
	printf("mythread hello\n");
}
static void *mythread(void*p){
	int signum;
	while(1){
		sigwait(&set,&signum);
		if(SIGUSR1==signum)
            myfunc();
		if(SIGUSR2==signum)
		{
            printf("mythread  exit \n");
            break;
		}
	}
}
sigset_t get_sigset(void){
    return set;    
}
int getexitsigle()
{
    return giExit;    
}
int main()
{
	char tmp;
	void *status;

    weather stweather;

	/* 信号初始化 */     
	sigemptyset(&set);
	sigaddset(&set,SIGUSR1);
	sigaddset(&set,SIGUSR2);
	sigprocmask(SIG_SETMASK,&set,NULL);
	/* 创建子线程 */
	pthread_create(&tid,NULL,mythread,NULL);
	pthread_create(&trevpktid,NULL,prevpktmain,NULL);
	pthread_create(&tworkid,NULL,workmain,NULL);
    pthread_create(&tvoiceid,NULL,voicemain,NULL);
#if 0
	pthread_create(&tlistenid,NULL,listenmain,NULL);	
	pthread_create(&tuartid,NULL,puartmain,NULL);
    voicesockopen();	
#endif 
    usleep(100);
    while(1)
    {    

        printf("\n:");
        tmp = 'j';
        sleep(1);
        scanf("%c",&tmp);
        if('a'==tmp)
        {
	   //	testvioce();
       //   pthread_kill(tid,SIGUSR1);//发送SIGUSR1，打印字符串。
       //  (void)quemsg_snd_voice("opendoor.wav","50");
       //   (void)quemsg_snd_voice("oclock.mp3","90");
            char pbuf[7];
            pbuf[0]=0x0d;
            pbuf[1]=0x0a;
            pbuf[2]=0x01;
            pbuf[3]=0x01;
            pbuf[4]=0x00;
            pbuf[5]=0x0d;
            pbuf[6]='\0';
            pkt_send(pbuf,6);
        }
		else if ('p'==tmp)
        {
//            pthread_kill(tid,SIGUSR1);//发送SIGUSR1，打印字符串。
          (void)quemsg_snd_voice("tmp.mp3","90");
 		}
		else if ('w'==tmp)
        {
			//playback_wav("/root/02.wav","20");
            pthread_kill(tid,SIGUSR1);//发送SIGUSR1，打印字符串。
 		}
		else if ('e'==tmp)
        {
			char *pvstr;
		    curl_get_weather(pvstr,&stweather);
			printf("weather message is %s,%d,h:%s,l:%s\n",pvstr,stweather.tmp,stweather.shightemp,stweather.slowtemp);
            pthread_kill(tid,SIGUSR1);//发送SIGUSR1，打印字符串。
 		}
		else if ('r'==tmp)
        {
			char svstr[100]={0};
			sprintf(svstr,"当前温度%d 摄氏度  今天    %s    %s",stweather.tmp,stweather.shightemp,stweather.slowtemp);
            printf("svstr is %s,len is %d",svstr,strlen(svstr));
			curl_post_data(svstr,strlen(svstr));
            pthread_kill(tid,SIGUSR1);//发送SIGUSR1，打印字符串。
 		}
		else if ('t'==tmp)
        {
            (void)mqueue_send2work("weather",7);
 		}
        else if('q'==tmp)
        {
			//发出SIGUSR2信号，让线程退出，如果发送SIGKILL，线程将直接退出。
			quemsg_snd_exit();
            (void)mqueue_send2pkt("exit",4);
            (void)mqueue_send2work("exit",4);
			pthread_kill(tid,SIGUSR2);
			//等待线程tid执行完毕，这里阻塞。
            //sleep(1);
			#if 0
			pthread_kill(tlistenid,SIGUSR2);
            giExit = -1;
			ipthread_join(tlistenid,&status);
			pthread_join(tuartid,&status);
			#endif 
			pthread_join(tid,&status);
			pthread_join(tvoiceid,&status);		
			pthread_join(trevpktid,&status);
			pthread_join(tworkid,&status);
			printf("finish\n");
            //voicesockclose();
			break;
        }
        else
            continue;
    }
    return 0;
}

