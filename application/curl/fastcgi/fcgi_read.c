#include <stdio.h>
#include <fcgi_stdio.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define PKTMQNAME "/pktmq"
int main()
{
    mqd_t mqd_r,mqd_w;
    int count = 0;
	char rbuf[BUFSIZ] = {0};
	char wbuf[BUFSIZ] = {0};
	int val;
	int ret;
    /*mqueue*/
    mqd_r = mq_open("/fastcgi_write", O_RDWR|O_CREAT, 0600, NULL);
    if (mqd_r == -1)
    {
        perror("pkt mq_open()");
        exit(1);
    } 
    mqd_w = mq_open(PKTMQNAME, O_RDWR|O_CREAT, 0600, NULL);
    if (mqd_w == -1)
    {
        perror("pkt mq_open()");
        exit(1);
    } 
    while (FCGI_Accept() >= 0)
    {
        printf("Content-type: text/html\r\n"
                "\r\n"
                ""
                "FastCGI Hello!"
                "Request number %d running on host%s "
                "Process ID: %d\n", ++count, getenv("SERVER_NAME"), getpid());
        val = 8089;
		ret = mq_send(mqd_w, wbuf, BUFSIZ, val);
		if (ret == -1) {
			perror("pkt mq_receive err()");
            exit(1);
		}
		ret = mq_receive(mqd_r, rbuf, BUFSIZ, &val);
		if (ret == -1) {
			perror("pkt mq_receive err()");
		}
		printf("pktthr rcv mqueue msg %s,prio:%d\n",rbuf,val);
    }
    (void)mq_close(mqd_r);
    (void)mq_close(mqd_w);
    return 0;
}
