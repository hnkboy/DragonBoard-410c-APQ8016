#ifndef __COMMON_H
#define __COMMON_H

/*
	File		:common.h
	Description	:�Զ����ͷ�ļ����������õ�ϵͳͷ�ļ������õĺ꣬�Զ���Ĺ��ߺ���
*/

/*ͷ�ļ�ͳһ����*/

/*��׼ͷ�ļ�*/
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/*linuxϵͳ����ͷ�ļ�*/
#include <unistd.h>
#include <sys/types.h>

#define MAXLINE 4096  /*һ�е���󳤶�*/

/*�Զ��庯��ԭ��*/
/*��err_��ͷ���Ǵ������������ڳ���ʱ��ӡ��ʾ��Ϣ��������ز���*/

/*ϵͳ������صķ��������󣬴�ӡ�û���ʾ��Ϣ��ϵͳ��ʾ��Ϣ��Ȼ�󷵻�*/
void err_ret(const char *msg, ...);

/*ϵͳ������ص��������󣬴�ӡ�û���ʾ��Ϣ��ϵͳ��ʾ��Ϣ��Ȼ��ֱ�ӵ���exit()��ֹ����*/
void err_sys(const char *msg, ...);

/*ϵͳ������ص��������󣬴�ӡ�û���ʾ��Ϣ��ϵͳ��ʾ��Ϣ����������ת���ļ���Ȼ�����exit()��ֹ����*/
void err_dump(const char *fmt, ...);

/*ϵͳ�����޹صķ��������󣬴�ӡ�û���ʾ��Ϣ��Ȼ�󷵻�*/
void err_msg(const char *msg, ...);

/*ϵͳ�����޹صķ��������󣬴�ӡ�û���ʾ��Ϣ��errorָ����ϵͳ��ʾ��Ϣ��Ȼ�󷵻�*/
void err_cont(int error, const char *msg, ...);

/*ϵͳ�����޹ص��������󣬴�ӡ�û���ʾ��Ϣ��Ȼ�����exit()��ֹ����*/
void err_quit(const char *msg, ...);

/*ϵͳ�����޹ص��������� ��ӡ�û���ʾ��Ϣ��errorָ����ϵͳ��ʾ��Ϣ��Ȼ�����exit()��ֹ����*/
void err_exit(int error, const char *msg, ...);

#endif




















