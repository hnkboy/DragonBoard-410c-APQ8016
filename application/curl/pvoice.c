#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<pthread.h>
#include<time.h>
#include<sys/epoll.h>
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

#include "homemain.h"
#include "playback.h"
#include "playback_wav.h"

#include <curl/curl.h>
#include "json.h"
#include "getinmemory.h"
#include "postcallback.h"
#include "voiceclient.h"

struct tm nowTime;
#define EPOLL_SIZE 20
#define VOICEMQNAME "/pvoicemq"
mqd_t mqdvoice;

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
int mqueue_send2voice(const char *buf,long len)
{
	int ret;
    ret = mq_send(mqdvoice, buf, len, 2);
    if (ret == -1) {
        perror("voice mq_send()");
		return -1;
    } 
    printf(" send to pvoice mqueue msg: %s\n",buf);
	return 0;
    
}

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

int getNowTime(void);
void task_porc(){

	if (1 ==gsm_flag)
	{
		printf("gsm_flag == 1\n");
		voicesend("opendoor.wav");
		gsm_flag = 0;
			
		
	}
	if (1 == getNowTime())
	{
		voicesend("opendoor.wav");
	}

}
void *voicemain(void*p){
	int signum;
    sigset_t gset;
	
	int ep_fd_vo,ep_cnt,i,flag;
    int ret;
	struct epoll_event event;
    struct epoll_event* pevents;
    gset = get_sigset();
    myfunc();

	
	/*mqueue*/
  	mqdvoice = mq_open(VOICEMQNAME, O_RDWR|O_CREAT, 0600, NULL);
    if (mqdvoice == -1) {
        perror("voice mq_open()");
        exit(1);
    } 
    /*epoll*/
	ep_fd_vo=epoll_create(20);
    event.events=EPOLLIN;
    event.data.fd=mqdvoice;
    pevents=malloc(sizeof(struct epoll_event)*20);
    epoll_ctl(ep_fd_vo,EPOLL_CTL_ADD,mqdvoice,&event);
	while( 0 == ret)
    {
        ep_cnt=epoll_wait(ep_fd_vo,pevents,20,100);
        if (ep_cnt < 0)
        {
            break;
        }
        for(i=0;i<ep_cnt;i++)
        {
            if(mqdvoice == pevents[i].data.fd)
            {
                char rbuf[BUFSIZ];
                int val;
				ret = mq_receive(mqdvoice, rbuf, BUFSIZ, &val);
				if (ret == -1) {
					perror("voice mq_receive err()");
				}
                printf("voice rcv mqueue msg %s ,prio:%d\n",rbuf,val);
				if(strcmp(rbuf,"exit")==0)
       			{
					(void)mq_close(mqdvoice);
                    ret = -1;
					break;
        		}	
				playvoice(rbuf,ret);

            }
        }
		task_porc();
		
    }
    close(ep_fd_vo);
    printf("voice thread exit\n");
    return 0;

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
        
    //sprintf(current, "%04d%02d%02d%02d:%02d:%02d", nowTime.tm_year + 1900, nowTime.tm_mon+1, nowTime.tm_mday, 
    //  nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);
    //printf("%s\n",current);
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

