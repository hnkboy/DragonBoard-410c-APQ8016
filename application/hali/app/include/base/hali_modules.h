
#ifndef __HALIMOD_
#define __HALIMOD_



typedef ulong (*HALI_MODULE_MSGPROC_PF)(char *,int);

typedef ulong (*HALI_MODULE_REGINIT_PF)(void);

typedef void (*HALI_MODULE_REGFINI_PF)(void);

typedef void (*HALI_MODULE_MQTTPROC_PF)(void *);
typedef void (*HALI_MODULE_MQTTSUB_PF)(void);


extern HALI_MODULE_MSGPROC_PF g_svrmsgproc[HALI_MODULE_MAX];
extern HALI_MODULE_MSGPROC_PF g_workmsgproc[HALI_MODULE_MAX];
extern HALI_MODULE_MSGPROC_PF g_mainmsgproc[HALI_MODULE_MAX];
extern HALI_MODULE_REGINIT_PF g_moduleinit[HALI_MODULE_MAX];
extern HALI_MODULE_REGFINI_PF g_modulefini[HALI_MODULE_MAX];
extern HALI_MODULE_MQTTPROC_PF g_mqttmsgproc[HALI_MODULE_MAX];
extern HALI_MODULE_MQTTSUB_PF  g_mqttsub[HALI_MODULE_MAX];


void hali_module_regsvrthrmsgproc(uint moduletype, HALI_MODULE_MSGPROC_PF pfmsgcallback);
void hali_module_unregsvrthrmsgproc(uint moduletype);

void hali_module_regworkthrmsgproc(uint moduletype, HALI_MODULE_MSGPROC_PF pfmsgcallback);
void hali_module_unregworkthrmsgproc(uint moduletype);

void hali_module_regmainthrmsgproc(uint moduletype, HALI_MODULE_MSGPROC_PF pfmsgcallback);
void hali_module_unregmainthrmsgproc(uint moduletype);

void hali_module_regmodinit(uint moduletype, HALI_MODULE_REGINIT_PF pfunc);
void hali_module_unregmodinit(uint moduletype);


void hali_module_regmodfini(uint moduletype, HALI_MODULE_REGFINI_PF pfunc);
void hali_module_unregmodfini(uint moduletype);



void hali_module_regmqttmsgproc(uint moduletype, HALI_MODULE_MQTTPROC_PF pfunc);
void hali_module_unregmqttmsgproc(uint moduletype);

void hali_module_regmqttsubproc(uint moduletype, HALI_MODULE_MQTTSUB_PF pfunc);
void hali_module_unregmqttsubproc(uint moduletype);














ulong hali_module_init(void);
void hali_module_fini(void);






#endif


