#include <stdio.h>
#include <fcgi_stdio.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define EPOLL_SIZE 20
int main()
{
    mqd_t mqd_w, mqd_r;
    int count = 0;
	char wbuf[BUFSIZ] = {0};
	int val = 1;
	int ret;
	int  bisbreak = 0;
    int ep_fd,ep_cnt,i;
    struct epoll_event event;
    struct epoll_event* pevents;
    /*mqueue*/
    mqd_w = mq_open("/fastcgi_write", O_RDWR|O_CREAT, 0600, NULL);
    if (mqd_w == -1)
    {
        perror("pkt mq_open()");
        exit(1);
    } 
    mqd_r = mq_open("/fastcgi_read", O_RDWR|O_CREAT, 0600, NULL);
    if (mqd_r == -1)
    {
        perror("pkt mq_open()");
        exit(1);
    } 
    ep_fd=epoll_create(EPOLL_SIZE);

    event.events=EPOLLIN;
    event.data.fd=mqd_r;
    epoll_ctl(ep_fd,EPOLL_CTL_ADD,mqd_r,&event);
    pevents=malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

	while( 0 == bisbreak)
    {
        ep_cnt=epoll_wait(ep_fd,pevents,EPOLL_SIZE,-1);
        for(i=0;i<ep_cnt;i++)
        {
            if(mqd_r == pevents[i].data.fd)
            {
                char rbuf[BUFSIZ] = {0};
                int val;
				ret = mq_receive(mqd_r, rbuf, BUFSIZ, &val);
				if (ret == -1) {
					perror("pkt mq_receive err()");
				}
                printf("pktthr rcv mqueue msg %s,prio:%d\n",rbuf,val);
				if(strcmp(rbuf,"exit")==0)
       			{
                    bisbreak = 1;
					break;
        		}
				else
				{
					bzero(wbuf,BUFSIZ);
					sprintf(wbuf,"\n\r hello fcgi %d\n\r",count);
					ret = mq_send(mqd_w, wbuf, BUFSIZ, val);
					if (ret == -1) {
						perror("pkt mq_receive err()");
					}
					printf("send mqueue msg %s,prio:%d\n",wbuf,val);
					count ++;
				}	
            }
        }
    }
    (void)mq_close(mqd_w);
    (void)mq_close(mqd_r);
    return 0;
}
