#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include "../../include/base/list.h"
#include "../../include/base/hali_base.h"
#include "../../include/base/hali_error.h"
#include "../../include/base/hali_thread.h"
#include "../../include/base/hali_epoll.h"


/*******************STATIC FUNC*********/
static int _init_stlv_env(STLV_ENV *penv)
{
	SLOG_OPTION option;
	int ret = -1;

	if(!penv)
		return -1;

	if(penv->slog_fd>=0)
	{
		return 0;
	}

	//open slog descriptor
	memset(&option , 0 , sizeof(option));
	strncpy(option.type_value._local.log_name , STLV_LOG_NAME , sizeof(option.type_value._local.log_name));

	ret = slog_open(SLT_LOCAL , STLV_LOG_LEVEL , &option , NULL);
	if(ret < 0)
	{
		printf("%s slog_open failed!\n" , __FUNCTION__);
		return -1;
	}

	penv->slog_fd = ret;
	slog_log(penv->slog_fd , SL_INFO , "%s success!" , __FUNCTION__);
	return 0;
}




/*******************API FUNC******/
/*
 * pack_stlv
 * 封装数据为stlv包
 * @type:refer STLV_T_PR_XX or STLV_T_CO_XX
 * @packed_buff:封装后的缓冲区
 * @value:将要封装的数据
 * @v_len:数据长度
 * @return: 0 failed; >0 封装后的长度
 */

unsigned int pack_stlv(char type , unsigned char *packed_buff , unsigned char *value , unsigned int vlen)
{
	int slog_fd = -1;
	char obj = -1;
	unsigned int ret = 0;

	/***Try Init Env*/
	STLV_ENV *penv = &stlv_env;
	if(penv->slog_fd < 0)
		_init_stlv_env(penv);
	slog_fd = penv->slog_fd;

	/***Arg Check*/
	if(!packed_buff || !value)
	{
		slog_log(slog_fd , SL_ERR , "%s failed! some arg nil!" , __FUNCTION__);
		return 0;
	}

	/***Get OBJ*/
	obj = STLV_TYPE_2_OBJ(type);
	if(obj!=STLV_T_OBJ_PRIMITIVE && obj!=STLV_T_OBJ_COMPOS)
	{
		slog_log(slog_fd , SL_ERR , "%s failed! type is illegal!" , __FUNCTION__ , type);
		return 0;
	}

	/***PACK*/
	if(obj == STLV_T_OBJ_PRIMITIVE)
		ret =  _pack_stlv_primitive(penv , type , packed_buff , value);
	else
		ret =  _pack_stlv_construct(penv , type , packed_buff , value , vlen);

	if(!ret)
	{
		slog_log(slog_fd , SL_ERR , "%s failed! type:%d" , __FUNCTION__ , type);
		return 0;
	}

	/***CheckSum*/
	*((unsigned short *)(packed_buff+ret)) = htons(_stlv_check_sum(penv , (STLV_PACK  *)packed_buff));
	return ret + 2;	//include 2bytes of check_sum
}

//supported function of type:Long
unsigned int pack_stlv_long(unsigned char *packed_buff , unsigned char *value)
{
	unsigned int ret;
	do
		{
			if(sizeof(long) == 32)	//32bit OS
			{
				ret = pack_stlv(STLV_T_PR_INT32 , packed_buff , value , 0);
			}
			else	//64bit OS
			{
				ret = pack_stlv(STLV_T_PR_INT64 , packed_buff , value , 0);
			}
	}while(0);

	return ret;
}







#ifdef __cplusplus
}
#endif



