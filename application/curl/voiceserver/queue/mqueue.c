#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>

#define MQNAME "/mqtest"
#define EPSIZE 10


int main()
{

    mqd_t mqd;
    int ret, epfd, val, count;
    char buf[BUFSIZ];
    struct mq_attr new, old;
    struct epoll_event ev, rev;

    mqd = mq_open(MQNAME, O_RDWR);
    mqd = mq_open(MQNAME, O_RDWR|O_CREAT, 0600, NULL);
    if (mqd == -1) {
        perror("mq_open()");
        exit(1);
    }

    /* 因为有epoll帮我们等待描述符是否可读，所以对mqd的处理可以设置为非阻塞 */
    new.mq_flags = O_NONBLOCK;

    if (mq_setattr(mqd, &new, &old) == -1) {
        perror("mq_setattr()");
        exit(1);
    }

    epfd = epoll_create(EPSIZE);
    if (epfd < 0) {
        perror("epoll_create()");
        exit(1);
    }

    /* 关注描述符是否可读 */
    ev.events = EPOLLIN;
    ev.data.fd = mqd;

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, mqd, &ev);
    if (ret < 0) {
        perror("epoll_ctl()");
        exit(1);
    }

    while (1) {
        ret = epoll_wait(epfd, &rev, EPSIZE, -1);
        if (ret < 0) {
            /* 如果被信号打断则继续epoll_wait */
            if (errno == EINTR) {
                continue;
            } else {
                perror("epoll_wait()");
                exit(1);
            }
        }

        /* 此处处理所有返回的描述符（虽然本例子中只有一个） */
        for (count=0;count<ret;count++) {
            ret = mq_receive(rev.data.fd, buf, BUFSIZ, &val);
            if (ret == -1) {
                if (errno == EAGAIN) {
                    break;
                }
                perror("mq_receive()");
                exit(1);
            }
            printf("msq: %s, prio: %d\n", buf, val);
        }

    }

    /* 恢复描述符的flag */
    if (mq_setattr(mqd, &old, NULL) == -1) {
        perror("mq_setattr()");
        exit(1);
    }

    ret = mq_close(mqd);
    if (ret == -1) {
        perror("mp_close()");
        exit(1);
    }
    exit(0);
}
