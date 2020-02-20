#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <string.h>

#include "../../include/base/list.h"
#include "../../include/base/hali_base.h"
#include "../../include/base/hali_error.h"
#include "../../include/base/hali_thread.h"
#include "../../include/base/hali_modules.h"



//#define MQTT_HOST "192.168.0.1"
//#define MQTT_HOST "127.0.0.1"
#define MQTT_HOST "192.168.0.136"

#define MQTT_PORT  1883
#define MQTT_KEEP_ALIVE 60
#define MQTT_MSG_MAX_SIZE  512




/*mosquitto 全局变量*/
struct mosquitto *g_pstmosq;


typedef enum  tag_devid
{
    ZIGBEE_DEV_ID1,
    ZIGBEE_DEV_MAX,
}MQTT_DEVTYPE_E;






API void hali_mosquitto_subscribe(char *puctopic)
{

    if ((NULL != g_pstmosq) && (NULL != puctopic))
    {
        mosquitto_subscribe(g_pstmosq, NULL, puctopic, 2);
    }
    //zigbee_debug(ZIGBEE_DEBUG_EVENT,"sub:%s,\n",puctopic);
    printf("sub:%s,\n",puctopic);

}
API void hali_mosquitto_publish(char *puctopic, char *pucmessage)
{

    ulong ulerr = ERROR_FAILED;
    if ((NULL == g_pstmosq) || (NULL == puctopic) || (NULL == pucmessage))
    {

        return ;
    }

    printf("pub:%s,\n",puctopic);
    mosquitto_publish(g_pstmosq,NULL,puctopic,strlen(pucmessage),pucmessage,0,0);
}

void hali_mosquitto_connectcallback(struct mosquitto *pstmosq, void *userdata, int iresult)
{
    uint uimodtype;
    if(!iresult){

        /*挂节点注册*/

        for(uimodtype = HALI_MODULE_BASIC; uimodtype < HALI_MODULE_MAX; uimodtype++)
        {
            if (NULL != g_mqttsub[uimodtype])
            {
                (void)g_mqttsub[uimodtype]();
            }
        }

        /* Subscribe to broker information topics on successful connect. */
        hali_mosquitto_subscribe("1");
    }else{
        printf("Connect failed\n");
    }
}
void hali_mosquitto_messagecallback(struct mosquitto *pstmosq, void *userdata, const struct mosquitto_message *pstmessage)
{

    uint uimodtype;


    for(uimodtype = HALI_MODULE_BASIC; uimodtype < HALI_MODULE_MAX; uimodtype++)
    {
        if (NULL != g_mqttmsgproc[uimodtype])
        {
            (void)g_mqttmsgproc[uimodtype]((void *)pstmessage);
        }
    }

    if(pstmessage->payloadlen){
        printf("%s %s", pstmessage->topic, pstmessage->payload);
    }else{
        printf("%s (null)\n", pstmessage->topic);
    }
    fflush(stdout);
}


API ulong hali_mosquitto_init(void)
{
    struct mosquitto *pstmosq = NULL;
    ulong ulerr = ERROR_FAILED;
    int iret = -1;
    bool bsession = true;
    /*libmosquitto 库初始化*/
    mosquitto_lib_init();
    /*创建mosquitto客户端*/
    pstmosq = mosquitto_new(NULL,bsession,NULL);
    if (NULL != pstmosq)
    {
        /*开启一个线程，在线程里不停的调用 mosquitto_loop() 来处理网络信息*/
        iret = mosquitto_loop_start(pstmosq);
        if(MOSQ_ERR_SUCCESS == iret)
        {
            /*设置回调*/
            mosquitto_connect_callback_set(pstmosq, hali_mosquitto_connectcallback);
            mosquitto_message_callback_set(pstmosq, hali_mosquitto_messagecallback);
            /*建立连接*/
            iret = mosquitto_connect(pstmosq, MQTT_HOST, MQTT_PORT, MQTT_KEEP_ALIVE);

        }
        else
        {
            printf("mosquitto loop error\n");
        }
    }
    if (MOSQ_ERR_SUCCESS != iret)
    {
        if (NULL != pstmosq)
        {
            mosquitto_destroy(pstmosq);
        }
        mosquitto_lib_cleanup();

    }
    else
    {
        g_pstmosq = pstmosq;
        ulerr = ERROR_SUCCESS;
    }


    return ulerr;

}
API void hali_mosquitto_fini(void)
{
    if (NULL != g_pstmosq)
    {
        mosquitto_destroy(g_pstmosq);
    }
    mosquitto_lib_cleanup();
    return ;
}


