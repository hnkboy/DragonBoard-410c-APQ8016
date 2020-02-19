#ifndef __HALICLI_
#define __HALICLI_


typedef enum enclitype
{
    HALI_CLI_DISPLAY_ZIGBEEDEV = 0,
    HALI_CLI_DISPLAY_HELP,
    HALI_CLI_DISPLAY_DEVPKTBUG,
    HALI_CLI_MAX
}HALI_CLI_TYPE_E;


typedef void (*HALI_CLI_PROC_PF)(char *pcstr);

void hali_cli_reg(uint type, HALI_CLI_PROC_PF pfunc);

API ulong hali_cli_msgcallbackproc(int fd);
API void hali_cli_printfhelpmsg(char *pstr);


#endif


