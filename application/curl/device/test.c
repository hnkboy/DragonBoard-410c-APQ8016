#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cJSON/cJSON.h"

/* Create a bunch of objects as demonstration. */
static int print_preallocated(cJSON *root)
{
    /* declarations */
    char *out = NULL;
    char *buf = NULL;
    char *buf_fail = NULL;
    size_t len = 0;
    size_t len_fail = 0;
    /* formatted print */
    out = cJSON_Print(root);

    /* create buffer to succeed */
    /* the extra 5 bytes are because of inaccuracies when reserving memory */
    len = strlen(out) + 5;
    buf = (char*)malloc(len);
    if (buf == NULL)
    {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    /* create buffer to fail */
    len_fail = strlen(out);
    buf_fail = (char*)malloc(len_fail);
    if (buf_fail == NULL)
    {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    /* Print to buffer */
    if (!cJSON_PrintPreallocated(root, buf, (int)len, 1)) {
        printf("cJSON_PrintPreallocated failed!\n");
        if (strcmp(out, buf) != 0) {
            printf("cJSON_PrintPreallocated not the same as cJSON_Print!\n");
            printf("cJSON_Print result:\n%s\n", out);
            printf("cJSON_PrintPreallocated result:\n%s\n", buf);
        }
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    /* success */
    printf("%s\n", buf);

    /* force it to fail */
    if (cJSON_PrintPreallocated(root, buf_fail, (int)len_fail, 1)) {
        printf("cJSON_PrintPreallocated failed to show error with insufficient memory!\n");
        printf("cJSON_Print result:\n%s\n", out);
        printf("cJSON_PrintPreallocated result:\n%s\n", buf_fail);
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    free(out);
    free(buf_fail);
    free(buf);
    return 0;
}



//void zigbee_devnode_print2json（void）{
void main(void){
	char pbuf[40];
	cJSON *root = NULL;
	cJSON *dev = NULL;
	cJSON *devnode = NULL;
	cJSON *devdata = NULL;
    int i = 0;

	DEV_NODE_S *pstDevNode = NULL;
	SL_NODE_S *pstDevNode = NULL;
	SL_NODE_S *pstDevNext = NULL;

	END_NODE_S *pstEndNode = NULL;
	SL_HEAD_S *pstHead ;
	SL_NODE_S *pstNode = NULL;
	SL_NODE_S *pstNext = NULL;

	root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("JIALI (\"Bee\") Nimble"));
    cJSON_AddItemToObject(root, "dev", dev = cJSON_CreateArray());
#if 0
    for (i = 0; i < 4; i++)
    {
        cJSON_AddItemToArray(dev, devnode = cJSON_CreateObject());
        cJSON_AddStringToObject(devnode, "devid", "12");
        cJSON_AddStringToObject(devnode, "devtype", "1");
        cJSON_AddStringToObject(devnode, "devrange", "100");
        cJSON_AddItemToObject(devnode, "devdata", devdata = cJSON_CreateObject());
        cJSON_AddStringToObject(devdata, "temperature", "12");
        cJSON_AddStringToObject(devdata, "humidity", "12");
    }
#endif
	SL_FOREACH_SAFE(&g_DevNode,pstDevNode,pstDevNext){

		pstDevNode = SL_ENTRY(pstDevNode,DEV_NODE_S,stNode);

        cJSON_AddItemToArray(dev, devnode = cJSON_CreateObject());
        cJSON_AddNumberToObject(devnode, "devid", pstDevNode->devid);
        cJSON_AddNumberToObject(devnode, "devtype", pstDevNode->devtype);
        cJSON_AddNumberToObject(devnode, "devrange", pstDevNode->range);

		SL_FOREACH_SAFE(pstHead,pstNode,pstNext){
		
			pstEndNode = SL_ENTRY(pstNode,END_NODE_S,stNode);
			printf("tlv   tag: %s\n",pstEndNode->uiTag);
			printf("tlv vlaue: %s\n",pstEndNode->pValue);
			cJSON_AddItemToObject(devnode, "devdata", devdata = cJSON_CreateObject());
			cJSON_AddStringToObject(devdata, shelpstr[pstEndNode->uiTag], pstEndNode->pValue);
		}
	}
 }

	print_preallocated(root);
    cJSON_Delete(root);
}

