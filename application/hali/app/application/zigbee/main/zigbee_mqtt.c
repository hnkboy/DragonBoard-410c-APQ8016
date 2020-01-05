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
    uchar *pcstr;
    uchar aidhex[6];
    int devid;
    if(pstmessage->payloadlen)
    {
        printf("zigbee%s %s", pstmessage->topic, pstmessage->payload);

        if ((NULL == strcmp(pstmessage->topic, "command"))   &&
            (NULL == strcmp(pstmessage->topic, "home/mqtt/topic/switch")) &&)
        {
            pcstr = strcmp(pstmessage->topic, "0x");
            strlcpy(aidhex, pcstr, 6);
            devid = htoi(aidhex);
            ulerr = zigbee_devnode_getattrvlaue(devid, TLV_RESP_SWITCH_STATE, &ucstate);
            if(ERROR_SUCCESS == ulerr)
            {
                printf("zigbee get id:%x switch state %d\n", devid, ucstate);
            }

            if (0 == strcmp(pstmessage->payload, "ON"))
            {
                if (SWITCH_STATE_OFF == ucstate)
                {
                    zigbee_serialsendswitchcmd(devid, SWITCH_STATE_ON);
                }
            }
            else
            {
                if (SWITCH_STATE_ON == ucstate)
                {
                    zigbee_serialsendswitchcmd(devid, SWITCH_STATE_OFF);
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
    //hali_mosquitto_subscribe("/mqtt/topic/light01/command");
    return;
}
API void zigbee_mqttswitchsub(int devid)
{
    char topic[50];
    snprintf(topic, 50,
             "home/mqtt/topic/switch/0x%4x/command", devid);
    hali_mosquitto_subscribe(topic);
    return;
}
API void zigbee_mqttswitchpub(int devid, uint uistate)
{
    char topic[50];
    snprintf(topic, 50,
             "home/mqtt/topic/switch/0x%4x/state", devid);
    if (SWITCH_STATE_ON == uistate)
    {
        hali_mosquitto_publish(topic,"ON");

    }
    else if (SWITCH_STATE_OFF == uistate)
    {
        hali_mosquitto_publish(topic,"OFF");
    }
    return;
}

API void zigbee_mqtttemperpub(int devid, uint temper,uint humi)
{
    char str[50];
    str[0]= '\0';
    char topic[50];
    snprintf(topic, 50,
             "home/mqtt/topic/sensor/0x%4x", devid);

    snprintf(str,50,
             "{\"temperature\" : %u,\"humidity\" : %u}\n",temper,humi);

    hali_mosquitto_publish(topic, str);
    return;
}


#ifdef __cplusplus
}
#endif



