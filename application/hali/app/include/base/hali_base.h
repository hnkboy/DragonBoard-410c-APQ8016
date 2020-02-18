#ifndef __HALIBASE_
#define __HALIBASE_

#define IN
#define OUT
#define INOUT

#define API
#define NO_API

#define VOID void

#ifndef BOOL_T
#define BOOL_T ushort
#endif

#define BOOL_TRUE  ((BOOL_T)1)
#define BOOL_FALSE ((BOOL_T)0)



/*

typedef unsigned long ulong;
typedef unsigned int uint;
*/
typedef unsigned char uchar;
/*16进制字符串转换init型*/
int htoi(char s[]);


#endif




