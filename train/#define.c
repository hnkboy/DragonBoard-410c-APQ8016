
#include <stdio.h>
#include <string.h>
#include "#define.h"

#define TEXTTT  "nihao a aaaaaaaaaaaaaaaaaaa"

str_s st1;

str_s *vlib_getst(void)
{
    return &st1;
}

char **fun(void)
{
    char **sr;

    sr = VLIB_INITS ("ip_main_init", "ip6_lookup_init",
                            "ip6_hop_by_hop_ioam_init");
    return sr;
}

int main(int argc, char * * argv)
{
    char **sr;

    char *name;
    memset(&st1,0,sizeof(st1));
    //name =  ##argc##;
    FOO1(str2);
    FOO2(str1);
    FOO(name);


    printf("%s\n",st1.tagstr2);

    sr = VLIB_INITS ("ip_main_init", "ip6_lookup_init",
                            "ip6_hop_by_hop_ioam_init");

    printf("OX%p\n", sr[1]);

    sr = fun();

    printf("OX%p\n", sr[1]);

    printf("%s\n",sr[1]);

    printf("%s\n",TEXTTT);   /*<!--验证字符常量放在了哪里-->*/

    return 0;
}


