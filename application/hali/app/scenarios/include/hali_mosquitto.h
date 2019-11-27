#ifndef __HALIMQTT_
#define __HALIMQTT_

void hali_mosquitto_subscribe(char *puctopic);
void hali_mosquitto_publish(char *puctopic, char *pucmessage);

ulong hali_mosquitto_init(void);
void hali_mosquitto_fini(void);



#endif

