#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include "alsaplay.h"

unsigned char temp,gsm_flag;
unsigned char str2 = 0x0;
unsigned char Uart2_Buffer[50];
unsigned char Uart2_Rx=0;
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	 struct termios newtio,oldtio;
	 if  ( tcgetattr( fd,&oldtio)  !=  0) {
	  perror("SetupSerial 1");
	  return -1;
	 }
	 bzero( &newtio, sizeof( newtio ) );
	 newtio.c_cflag  |=  CLOCAL | CREAD; //CLOCAL:忽略modem控制线  CREAD：打开接受者
	 newtio.c_cflag &= ~CSIZE; //字符长度掩码。取值为：CS5，CS6，CS7或CS8

	 switch( nBits )
	 {
	 case 7:
	  newtio.c_cflag |= CS7;
	  break;
	 case 8:
	  newtio.c_cflag |= CS8;
	  break;
	 }

	 switch( nEvent )
	 {
	 case 'O':
	  newtio.c_cflag |= PARENB; //允许输出产生奇偶信息以及输入到奇偶校验
	  newtio.c_cflag |= PARODD;  //输入和输出是奇及校验
	  newtio.c_iflag |= (INPCK | ISTRIP); // INPACK:启用输入奇偶检测；ISTRIP：去掉第八位
	  break;
	 case 'E':
	  newtio.c_iflag |= (INPCK | ISTRIP);
	  newtio.c_cflag |= PARENB;
	  newtio.c_cflag &= ~PARODD;
	  break;
	 case 'N': 
	  newtio.c_cflag &= ~PARENB;
	  break;
	 }

	 switch( nSpeed )
	 {
	 case 2400:
	  cfsetispeed(&newtio, B2400);
	  cfsetospeed(&newtio, B2400);
	  break;
	 case 4800:
	  cfsetispeed(&newtio, B4800);
	  cfsetospeed(&newtio, B4800);
	  break;
	 case 9600:
	  cfsetispeed(&newtio, B9600);
	  cfsetospeed(&newtio, B9600);
	  break;
	 case 115200:
	  cfsetispeed(&newtio, B115200);
	  cfsetospeed(&newtio, B115200);
	  break;
	 case 460800:
	  cfsetispeed(&newtio, B460800);
	  cfsetospeed(&newtio, B460800);
	  break;
	 default:
	  cfsetispeed(&newtio, B9600);
	  cfsetospeed(&newtio, B9600);
	  break;
	 }

	 if( nStop == 1 )
	  newtio.c_cflag &=  ~CSTOPB; //CSTOPB:设置两个停止位，而不是一个
	 else if ( nStop == 2 )
	 newtio.c_cflag |=  CSTOPB;
	 
	 newtio.c_cc[VTIME]  = 0; //VTIME:非cannoical模式读时的延时，以十分之一秒位单位
	 newtio.c_cc[VMIN] = 0; //VMIN:非canonical模式读到最小字符数
	 tcflush(fd,TCIFLUSH); // 改变在所有写入 fd 引用的对象的输出都被传输后生效，所有已接受但未读入的输入都在改变发生前丢弃。
	 if((tcsetattr(fd,TCSANOW,&newtio))!=0) //TCSANOW:改变立即发生
	 {
	  perror("com set error");
	  return -1;
	 }
	 printf("set done!\n\r");
	 return 0;
}

static void myfunc()
{
	printf("hello uart thread\n");
}
void *puartmain(void *p)
{
	 int fd1,nset,nread,ret;
     int i;
//	 char buf[100]={"test com data!...........\n"};
	 char buf[100]={"test com data!...........\n"};
	 char buf1[100];
     /*uart thread*/
    myfunc();
     /*uart thread end*/
     buf[0] = 0x0d;
     buf[1] = 0x0a;
     buf[2] = 0x01;
     buf[3] = 0x0d;
     buf[4] = '\0';

	 fd1 = open( "/dev/ttySAC0", O_RDWR  | O_NOCTTY);
	 if (fd1 == -1)
	  exit(1);
	 printf("open  SAC0 success!!\n");

	 nset = set_opt(fd1, 115200, 8, 'N', 1);
	 if (nset == -1)
	  exit(1);
	 printf("SET  SAC1 success!!\n");
	 printf("enter the loop!!\n");

	 while (1)

	 { 
	   memset(buf1, 0, sizeof(buf1));
   /*    
	   ret = write(fd1, buf, 10);
	   if( ret > 0){
	      printf("write success!  wait data receive\n");
	   }*/
	   nread = read(fd1, buf1, 1);
	   if(nread > 0){
		//printf("redatad: nread = %s\n\n\r", buf1);
		temp = buf1[0];
		//printf("tmp=%x\n\n\r", temp);
		//printf("str2=%d\n\n\r", str2);
        switch(str2){
        case 0x0:
            if (temp == 0x0D)str2=0x1;break;	
        case 0x1:
            if (temp == 0x0A)str2=0x2;else str2=0x0;break;	
        case 0x2:
            if (temp == 0xef)str2=0x0;else {str2=0x3;Uart2_Buffer[Uart2_Rx]=temp; Uart2_Rx++;}break;	
        case 0x3:
             Uart2_Buffer[Uart2_Rx]=temp;
             Uart2_Rx++;
             if(Uart2_Buffer[Uart2_Rx-1]==0x0D){
                    for (i=0;i<Uart2_Rx-1;i++)
                    	printf("%02x ",Uart2_Buffer[i]);
                    printf("\n\r");

                    str2=0x0;
                    Uart2_Rx=0;
                    if (0x1 ==Uart2_Buffer[(Uart2_Buffer[3]+4)])
                    gsm_flag=1;
 
             }
             if(Uart2_Rx==40){ Uart2_Rx=0; } break;
        default:break;
        }
	   }
	   usleep(10);
       if (-1 == getexitsigle())
       {
           
          break;    
       }
	  //nread = read(fd1, buf1,1);
	  //if(buf1[0] == 'q')
	   //break;
/*
		sigwait(&gset,&signum);
		if(SIGUSR1==signum)
            myfunc();
		if(SIGUSR2==signum)
		{
            printf("Iwillsleep2secondandexit\n");
            sleep(2);
            break;
		}
*/

	 }
 close(fd1);

 return 0;
}
