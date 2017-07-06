#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
        do \
        { \
                perror(m); \
                exit(EXIT_FAILURE); \
        } while(0)

#define CONSUMERS_COUNT 2   //消费者人数
#define PRODUCERS_COUNT 2   //生产者人数 
#define BUFFSIZE 5         

int g_buffer[BUFFSIZE];    //缓冲区数目

unsigned short in = 0;      //放入产品的指针（生产到哪个缓冲区）
unsigned short out = 0;     //取出缓冲区指针 （在哪个缓冲区消费的）
unsigned short produce_id = 0;     
unsigned short consume_id = 0;

sem_t g_sem_full; //可以使用的空缓冲区数（缓冲区中可以生产多少产品）
sem_t g_sem_empty;  //缓冲区内可以使用的产品数（可以消费的产品数）
pthread_mutex_t g_mutex;  //互斥信号量

pthread_t g_thread[CONSUMERS_COUNT + PRODUCERS_COUNT]; //线程ID typedef unsigned long int pthread_t;

void *consume(void *arg){
    int i;
    int num = (int)arg;
    while (1){
        printf("%d wait buffer not empty\n", num);
        sem_wait(&g_sem_empty);
        pthread_mutex_lock(&g_mutex);
        //遍历缓冲区，看有哪些缓冲区是可以生产产品的
        for (i = 0; i < BUFFSIZE; i++){
            printf("%02d ", i);
            if (g_buffer[i] == -1)
                printf("%s", "null");
            else
                printf("%d", g_buffer[i]);

            if (i == out)
                printf("\t<--consume");

            printf("\n");
        }
        //produce()操作(生产产品)
        consume_id = g_buffer[out];
        printf("%d begin consume product %d\n", num, consume_id);
        g_buffer[out] = -1;
        //将取出缓冲区的指针偏移1（下个生产的位置）
        out = (out + 1) % BUFFSIZE;
        printf("%d end consume product %d\n", num, consume_id);
        pthread_mutex_unlock(&g_mutex);
        sem_post(&g_sem_full);
        sleep(1);
    }
    return NULL;
}

void *produce(void *arg){
    int num = (int)arg;
    int i;
    while (1){
        printf("%d wait buffer not full\n", num);
        sem_wait(&g_sem_full);
        pthread_mutex_lock(&g_mutex);
        for (i = 0; i < BUFFSIZE; i++){
            printf("%02d ", i);
            if (g_buffer[i] == -1)
                printf("%s", "null");
            else
                printf("%d", g_buffer[i]);

            if (i == in)
                printf("\t<--produce");

            printf("\n");
        }

        printf("%d begin produce product %d\n", num, produce_id);
        g_buffer[in] = produce_id;
        in = (in + 1) % BUFFSIZE;
        printf("%d end produce product %d\n", num, produce_id++);
        pthread_mutex_unlock(&g_mutex);
        sem_post(&g_sem_empty);
        sleep(5);
    }
    return NULL;
}

int main(void){
    int i;
    for (i = 0; i < BUFFSIZE; i++)
        g_buffer[i] = -1;

    sem_init(&g_sem_full, 0, BUFFSIZE);
    sem_init(&g_sem_empty, 0, 0);

    pthread_mutex_init(&g_mutex, NULL);


    for (i = 0; i < CONSUMERS_COUNT; i++)
        pthread_create(&g_thread[i], NULL, consume, (void *)i);

    for (i = 0; i < PRODUCERS_COUNT; i++)
        pthread_create(&g_thread[CONSUMERS_COUNT + i], NULL, produce, (void *)i);

    for (i = 0; i < CONSUMERS_COUNT + PRODUCERS_COUNT; i++)
        pthread_join(g_thread[i], NULL);

    sem_destroy(&g_sem_full);
    sem_destroy(&g_sem_empty);
    pthread_mutex_destroy(&g_mutex);

    return 0;
}
