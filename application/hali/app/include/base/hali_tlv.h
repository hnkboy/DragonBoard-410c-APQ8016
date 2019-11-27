#ifndef __HALITLV_
#define __HALITLV_



typedef struct stgTlvNode
{
	SL_NODE_S stnode;
    uint uitag;    /*T 数据类型*/
    uint uilen;    /*L 数据长度*/
    uchar aucdata[0];
}HALI_TLV_NODE_S;

API HALI_TLV_NODE_S *hali_tlv_creatnode(uint uitag,uchar *pvalue,uint uisize);

API void hali_tlv_freenode(void *pstnode);
API void hali_tlv_printdebug(SL_HEAD_S *psthead);



#endif

