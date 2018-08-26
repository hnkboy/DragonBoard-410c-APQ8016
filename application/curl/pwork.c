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
#include "homemain.h"

#include <curl/curl.h>
#include "json.h"
#include "getinmemory.h"
#include "voiceclient.h"
#include "pvoice.h"

struct tm nowTime;
#define EPOLL_SIZE 20
#define VOICEMQNAME "/pworkmq"
mqd_t mqdwork;

int mqueue_send2work(const char *buf,long len)
{
	int ret;
    ret = mq_send(mqdwork, buf, strlen(buf), 2);
    if (ret == -1) {
        perror("voice mq_send()");
		return -1;
    } 
    printf("send to pwork mqueue msg: %s.\n",buf);
	return 0;
    
}

static void myfunc()
{
	printf("hello work thread\n");
}


unsigned char gsm_mor_flag;
struct tagmor_s {
    char mor_flag;
    char mor_done;    
    char clocksharp_flag;
    char clocksharp_done;  
}gmor_s;

int getNowTime(void);
#if 0
extern unsigned char gsm_flag;
#endif
void task_proc(){
#if 0
	if (1 ==gsm_flag)
	{
		printf("gsm_flag == 1\n");
        (void)quemsg_snd_voice("opendoor.wav",12);
		gsm_flag = 0;
			
	}
#endif
    switch (getNowTime())
    {
        case 1: 
            (void)quemsg_snd_voice("opendoor.wav",12);
		    printf("morning task \n");
            break;
        case 2: 
            (void)quemsg_snd_voice("opendoor.wav",12);
		    printf("clock sharp task\n");
            break;
        default :break;
    }

}
void *workmain(void*p){
	int signum;
    sigset_t gset;
    int timerfd;	
    int ep_fd_vo,ep_cnt,i,flag;
    int ret = 0;
    int isbreak = 0;
	struct epoll_event event;
    struct epoll_event* pevents;
	struct timespec startTime, intervalTime;
	struct itimerspec newValue;
    gset = get_sigset();
    myfunc();
    /* timerfd */
   	timerfd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);  
    if (timerfd < 0)
    {
        perror("work timerfd_open()");
        exit(1);
    }
	startTime.tv_sec = 1;    
    startTime.tv_nsec = 0;                                //相当于立即到达超时时间
    intervalTime.tv_sec = 1;                             //首次超时后，每三秒超时一次
    intervalTime.tv_nsec = 0;
    newValue.it_value = startTime;
    newValue.it_interval = intervalTime;
	//设置超时时间，且为相对时间
    if (timerfd_settime(timerfd, 0, &newValue, NULL) < 0)
    {
        perror("work timerfd_settime()");
        exit(1);
    }

	/*mqueue*/
  	mqdwork = mq_open(VOICEMQNAME, O_RDWR|O_CREAT, 0600, NULL);
    if (mqdwork == -1) {
        perror("work mq_open()");
        exit(1);
    } 
    /*epoll*/
	ep_fd_vo=epoll_create(20);
    event.events=EPOLLIN;
    event.data.fd=mqdwork;
    epoll_ctl(ep_fd_vo,EPOLL_CTL_ADD,mqdwork,&event);

    event.events=EPOLLIN;
    event.data.fd=timerfd;
    pevents=malloc(sizeof(struct epoll_event)*20);
    epoll_ctl(ep_fd_vo,EPOLL_CTL_ADD,timerfd,&event);
	while( 0 == isbreak)
    {
        ep_cnt=epoll_wait(ep_fd_vo,pevents,20,100);
        if (ep_cnt < 0)
        {
            break;
        }
        for(i=0;i<ep_cnt;i++)
        {
            if(mqdwork == pevents[i].data.fd)
            {
                char rbuf[BUFSIZ] = {0};
                int val;
				ret = mq_receive(mqdwork, rbuf, BUFSIZ, &val);
				if (ret == -1) {
					perror("voice mq_receive err()");
				}
                printf("workthr rcv mqueue msg %s ,prio:%d\n",rbuf,val);
				if(strcmp(rbuf,"exit")==0)
       			{
					(void)mq_close(mqdwork);
                    isbreak = 1;
					break;
        		}	
				//playvoice(rbuf,ret);
            }
			else if ((timerfd == pevents[i].data.fd) &&
					 pevents[i].events & EPOLLIN)
			{
                uint64_t data;
                read(pevents[i].data.fd, &data, sizeof(uint64_t));
				
    			//printf("timerfd read data is %lu.\n",data);
		        task_proc();
			}
        }
		
    }
    close(timerfd);
    close(mqdwork);
    close(ep_fd_vo);
    printf("work thread exit\n");
    return 0;

}

int getNowTime(void)
{

    struct timespec time;
    unsigned char uc_ret = 0;
    char current[1024];
    clock_gettime(CLOCK_REALTIME, &time);  //获取相对于1970到现在的秒数
    localtime_r(&time.tv_sec, &nowTime);

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
            uc_ret = 1;
            return uc_ret;
         }
    }
    else 
    {
         gmor_s.mor_flag = 0;          
         gmor_s.mor_done = 0;
    }
    if (nowTime.tm_sec > 0 )
    {

        gmor_s.clocksharp_flag = 1;
        if (0 == gmor_s.clocksharp_done )
        {
            printf("one oclock sharp \n");
            gmor_s.clocksharp_done = 1;
            uc_ret = 2;
        }
    }
    else
    {
        gmor_s.clocksharp_flag = 0;
        gmor_s.clocksharp_done = 0;
    }

    return uc_ret;
}

