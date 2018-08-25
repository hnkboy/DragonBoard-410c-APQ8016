#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<pthread.h>
#include<time.h>
#include<sys/epoll.h>
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

#include "homemain.h"

#include <curl/curl.h>
#include "json.h"
#include "getinmemory.h"
#include "voiceclient.h"

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

extern unsigned char gsm_flag;
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
void *workmain(void*p){
	int signum;
    sigset_t gset;
	
	int ep_fd_vo,ep_cnt,i,flag;
    int ret = 0;
    int isbreak = 0;
	struct epoll_event event;
    struct epoll_event* pevents;
    gset = get_sigset();
    myfunc();

	
	/*mqueue*/
  	mqdwork = mq_open(VOICEMQNAME, O_RDWR|O_CREAT, 0600, NULL);
    if (mqdwork == -1) {
        perror("voice mq_open()");
        exit(1);
    } 
    /*epoll*/
	ep_fd_vo=epoll_create(20);
    event.events=EPOLLIN;
    event.data.fd=mqdwork;
    pevents=malloc(sizeof(struct epoll_event)*20);
    epoll_ctl(ep_fd_vo,EPOLL_CTL_ADD,mqdwork,&event);
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
				playvoice(rbuf,ret);
            }
        }
		task_porc();
		
    }
    close(ep_fd_vo);
    printf("work thread exit\n");
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
         }
    }
    else 
    {
         gmor_s.mor_flag = 0;          
         gmor_s.mor_done = 0;
    }
    if (nowTime.tm_min == 0 )
    {

        gmor_s.clocksharp_flag = 1;
        if (0 == gmor_s.clocksharp_done )
        {
            printf("One oclock sharp \n");
            gmor_s.clocksharp_done = 1;
        }
    }
    else
    {
        gmor_s.clocksharp_flag = 0;
    }

    if (gmor_s.clocksharp_done | gmor_s.mor_done)
    {
        return 1;
    }

    return 0;
}

