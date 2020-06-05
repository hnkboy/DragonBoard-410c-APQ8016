#ifndef included_vlib_init_h
#define included_vlib_init_h

#include <stdio.h>

typedef struct {
  char *tagstr1;
  char *tagstr2;
  char *tagstr3;
}str_s;

str_s *vlib_getst(void);

#define _VLIB_INIT_FUNCTION_SYMBOL(x, type)	\
  _vlib_##type##_function_##x

/*<!--# ##的使用 define是预编译指令，都转化为代码-->*/
#define FOO(arg) \
  printf("%s\n",#arg)


#define FOO1(arg) \
  str_s *sts = vlib_getst(); \
  sts->tag##arg = "Wellcome1 ##"

#define FOO2(arg) \
  str_s *sts2 = vlib_getst(); \
  sts2->tag##arg = "Wellcome2 ##"






/*<!--用于定义一个字符串数组-->*/

#define VLIB_INITS(...)  (char*[]) { __VA_ARGS__, 0}


#endif

