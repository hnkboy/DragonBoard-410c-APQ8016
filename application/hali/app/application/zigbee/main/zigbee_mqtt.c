#ifdef __cplusplus
extern "C"{
#endif


#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <sys/stat.h>        /* For mode constants */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <mosquitto.h>


#include "../../../include/base/hali_base.h"
#include "../../../include/base/list.h"
#include "../../../include/base/hali_error.h"
#include "../../../include/base/hali_thread.h"
#include "../../../include/base/hali_epoll.h"
#include "../../../include/base/hali_timer.h"
#include "../../../include/base/hali_tlv.h"
#include "../../../scenarios/include/hali_mosquitto.h"


#include "../include/zigbee.h"
#include "zigbee_serial.h"


API void zigbee_mqttmsgproc(void *pdata)
{
    struct mosquitto_message *pstmessage = NULL;
    ZIGBEE_DEV_NODE_S *pstdevnode;
    uchar ucstate = SWITCH_STATE_OFF;
    pstmessage = ( struct mosquitto_message *)pdata;
    ulong ulerr;
    if(pstmessage->payloadlen)
    {
        printf("zigbee%s %s", pstmessage->topic, pstmessage->payload);

        if (0 == strcmp(pstmessage->topic, "/mqtt/topic/light01/command"))
        {

            ulerr = zigbee_devnode_getattrvlaue(0x796F, TLV_RESP_SWITCH_STATE, &ucstate);
            if(ERROR_SUCCESS == ulerr)
            {
                printf("zigbee get switch state %d\n",ucstate);
            }

            if (0 == strcmp(pstmessage->payload, "ON"))
            {
                if (SWITCH_STATE_OFF == ucstate)
                {
                    zigbee_serialsendswitchcmd(0x796F,SWITCH_STATE_ON);
                }
            }
            else
            {
                if (SWITCH_STATE_ON == ucstate)
                {
                    zigbee_serialsendswitchcmd(0x796F, SWITCH_STATE_OFF);
                }
            }
        }


    }
    else
    {
        printf("%s (null)\n", pstmessage->topic);
    }

}
API void zigbee_mqttsub(void )
{
    hali_mosquitto_subscribe("/mqtt/topic/light01/command");
    return;
}
API void zigbee_mqttswitchpub(int devid, uint uistate)
{
    if (SWITCH_STATE_ON == uistate)
    {
        hali_mosquitto_publish("/mqtt/topic/light01/state","ON");

    }
    else if (SWITCH_STATE_OFF == uistate)
    {
        hali_mosquitto_publish("/mqtt/topic/light01/state","OFF");
    }
    return;
}

API void zigbee_mqtttemperpub(int devid, uint temper,uint humi)
{
    char str[50];
    str[0]= '\0';

    snprintf(str,50,
           "{\n\r\"temperatrue\" : %u,\n\r\"humidity\" : %u\n\r }",temper,humi);

    hali_mosquitto_publish("home/sensor1", str);
    return;
}


#ifdef __cplusplus
}
#endif



