
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
    uchar *hitstr;
    uchar aidhex[6 + 1];
    int devid;
    if(pstmessage->payloadlen)
    {
        printf("zigbee%s %s", pstmessage->topic, pstmessage->payload);

        if ((NULL != strstr(pstmessage->topic, "command"))   &&
            (NULL != strstr(pstmessage->topic, "/home/mqtt/topic/switch/0x")))
        {
            hitstr = strstr(pstmessage->topic, "0x");
            memset(aidhex, 0, sizeof(aidhex));
            strncpy(aidhex, hitstr, 6);
            devid = htoi(aidhex);

            zigbee_debug(ZIGBEE_DEBUG_EVENT, "zigbee get id:%x ,%s\n", devid,hitstr);

            ulerr = zigbee_devnode_getattrvlaue(devid, TLV_RESP_SWITCH_STATE, &ucstate);
            if(ERROR_SUCCESS == ulerr)
            {
                zigbee_debug(ZIGBEE_DEBUG_EVENT, "zigbee get id:%x switch state %d\n", devid, ucstate);
            }

            if (NULL != strstr(pstmessage->payload, "ON"))
            {
                if (SWITCH_STATE_OFF == ucstate)
                {
                    zigbee_serialsendswitchcmd(devid, SWITCH_STATE_ON);
                }
            }
            else
            {
                if (SWITCH_STATE_OFF != ucstate)
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
    hali_mosquitto_subscribe("/home/mqtt/topic/switch/0x7970/command");
    hali_mosquitto_subscribe("/home/mqtt/topic/switch/0x7971/command");
    return;
}
API void zigbee_mqttswitchsub(int devid)
{
    char topic[50];
    memset(topic,0,sizeof(topic));
    snprintf(topic, 50,
             "/home/mqtt/topic/switch/0x%4x/command", devid);
    //printf("topic %s,\n",topic);
    hali_mosquitto_subscribe(topic);
    return;
}
API void zigbee_mqttswitchpub(int devid, uint uistate)
{
    char topic[50];
    memset(topic,0,sizeof(topic));
    snprintf(topic, 50,
             "/home/mqtt/topic/switch/0x%4x/state", devid);
    if (SWITCH_STATE_OFF == uistate)
    {
        hali_mosquitto_publish(topic,"OFF");

    }
    else
    {
        hali_mosquitto_publish(topic,"ON");
    }
    return;
}

API void zigbee_mqtttemperpub(int devid, uint temper,uint humi)
{
    char str[50];
    str[0]= '\0';
    char topic[50];
    memset(topic,0,sizeof(topic));
    snprintf(topic, 50,
             "/home/mqtt/topic/sensor/0x%4x", devid);

    snprintf(str,50,
             "{\"temperature\" : %u,\"humidity\" : %u}\n",temper,humi);

    hali_mosquitto_publish(topic, str);
    return;
}


#ifdef __cplusplus
}
#endif
