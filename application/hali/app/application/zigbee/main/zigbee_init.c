#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "../../../include/base/hali_base.h"
#include "../../../include/base/list.h"
#include "../../../include/base/hali_error.h"
#include "../../../include/base/hali_thread.h"
#include "../../../include/base/hali_epoll.h"
#include "../../../include/base/hali_timer.h"
#include "../../../include/base/hali_tlv.h"
#include "../../../include/base/hali_modules.h"
#include "../../../include/base/hali_cli.h"


#include "../include/zigbee.h"

int g_zigbeetimerfd = -1;


API ulong zigbee_init()
{
    zigbee_serialinit(2000);
    zigbee_devnode_init();
    hali_module_regmqttmsgproc(HALI_MODULE_ZIGBEE, zigbee_mqttmsgproc);
    hali_module_regmqttsubproc(HALI_MODULE_ZIGBEE, zigbee_mqttsub);

    hali_cli_reg(HALI_CLI_DISPLAY_HELP, hali_cli_printfhelpmsg);
    hali_cli_reg(HALI_CLI_DISPLAY_ZIGBEEDEV, zigbee_devnode_printall);
    hali_cli_reg(HALI_CLI_DISPLAY_DEVPKTBUG, zigbee_setdebug);
    /*创建10S定时器广播发现设备*/
    hali_timer_create(10, hali_dev_syntimeout, &g_zigbeetimerfd);
    return ERROR_SUCCESS;
}

API void zigbee_fini()
{
    zigbee_serialfini();
    zigbee_devnode_delall();

    hali_module_unregmqttmsgproc(HALI_MODULE_ZIGBEE);
    hali_module_unregmqttsubproc(HALI_MODULE_ZIGBEE);

    return;
}

#ifdef __cplusplus
}
#endif


