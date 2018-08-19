#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<pthread.h>
#include<time.h>
#include<sys/epoll.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <mqueue.h>

#include "../../homemain.h"
#include "../../alsa/playback.h"
#include "../../alsa/playback_wav.h"

#include <curl/curl.h>
#include "../../json.h"
#include "../../getinmemory.h"
#include "../../postcallback.h"
#include "../../voiceclient.h"

struct tm nowTime;
#define EPOLL_SIZE 20
#define VOICEMQNAME "/pvoicemq"
mqd_t mqdvoice;


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


int getNowTime(void);
void task_porc(){

		printf("gsm_flag == 1\n");
			
}
int main(int argc,char *argv[]){
	int signum;
    sigset_t gset;
	
	int ep_fd_vo,ep_cnt,i,flag;
    int ret;
	struct epoll_event event;
    struct epoll_event* pevents;
    myfunc();

	
	/*mqueue*/
  	mqdvoice = mq_open(VOICEMQNAME, O_RDWR|O_CREAT, 0666, NULL);
    if (mqdvoice == -1) {
        perror("voice mq_open()");
        exit(1);
    } 
    /*epoll*/
	ep_fd_vo=epoll_create(20);
    event.events=EPOLLIN;
    event.data.fd=mqdvoice;
    epoll_ctl(ep_fd_vo,EPOLL_CTL_ADD,mqdvoice,&event);
	while( 0 == ret)
    {
        ep_cnt=epoll_wait(ep_fd_vo,pevents,20,-1);
        printf("ep_cnt %d",ep_cnt);
        if (ep_cnt < 0)
        {
            break;
        }
        for(i=0;i<ep_cnt;i++)
        {
            if(mqdvoice == pevents[i].data.fd)
            {
      printf("epoll wait");
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
            }
        }
//		task_porc();
		
    }
    close(ep_fd_vo);
    printf("voice thread exit\n");
    return 0;

}


