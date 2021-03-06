#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<string.h>
#define BUF_SIZE 1024


int sock;
int getvoicesock()
{
    return sock;    
}
int voicesockopen()
{
    
    struct sockaddr_in addr;

    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock==-1)
        error_handling("socket() error");

    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr("192.168.0.81");
    addr.sin_port=htons(atoi("2000"));
    
    if(connect(sock,(struct sockaddr*)&addr,sizeof(addr))==-1)
        error_handling("connect() error");
    
    return 0;
}
int voicesend(char* arg)
{
    write_handling(sock,arg);
    read_handling(sock);
    return 0;
}
void voicesockclose()
{
    close(sock);
}
//写进程
void write_handling(int sock, char *pbuf)
{
    char buf[BUF_SIZE];
    //while(1)
    //{   
        memset(buf,0,BUF_SIZE);
        //fputs("Input:",stdout);
//        fgets(buf,BUF_SIZE,stdin);
        if(!strcmp(buf,"q\n")||
           !strcmp(buf,"Q\n"))
        {
            shutdown(sock,SHUT_WR);
            return;
        }
        write(sock,pbuf,strlen(pbuf));
   // }
}

//读进程
void read_handling(int sock)
{
    int str_len;
    char buf[BUF_SIZE];
    //while(1)
    //{
        str_len=read(sock,buf,BUF_SIZE-1);
        if(str_len<=0)
            return;
        buf[str_len]=0;
        printf("the message from server:%s\n",buf);
    //}
}


void error_handling(char* message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
