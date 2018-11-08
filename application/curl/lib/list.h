
#ifndef _LIST 
#define _LIST 
typedef struct stgnode_s
{   
	struct stgnode_s *pNext; 
}LS_NODE_S;

typedef struct stghead_s
{   
	LS_NODE_S *pFirst; 
}LS_HEAD_S;





void sl_init(LS_HEAD_S *pstLsHead){

	pstLsHead->pFirst = NULL;
}

void sl_addafter(LS_HEAD_S *pHead,LS_NODE_S *pNode){
    LS_NODE_S *pstNodeTemp = NULL;
	if (NULL == pHead->pFirst){

		pHead->pFirst = pNode;
	}
	else{
        pstNodeTemp = pHead->pFirst;
		while(NULL != pstNodeTemp->pNext)
		{
            pstNodeTemp = pstNodeTemp->pNext;
		}
		pstNodeTemp->pNext = pNode;		
	}
}
void sl_addhead(LS_HEAD_S *pHead,LS_NODE_S *pNode){
	pNode->pNext = pHead->pFirst;
	pHead->pFirst = pNode;
}


void sl_del(LS_HEAD_S *pHead,LS_NODE_S *pNode){

    LS_NODE_S *pstNodeTemp = NULL;
	if (NULL == pHead->pFirst){
        return;
	}
    if (pNode == pHead->pFirst){
        pHead->pFirst = pNode->pNext;  
    }
    else{
        pstNodeTemp = pHead->pFirst;
		while(pNode != pstNodeTemp->pNext)
		{
            pstNodeTemp = pstNodeTemp->pNext;
		}
		pstNodeTemp->pNext = pNode->pNext;		
    }
}

LS_NODE_S *ls_next(LS_NODE_S *pNode){

	return (pNode->pNext);
}
LS_NODE_S *sl_first(LS_HEAD_S *pHead){

	return (pHead->pFirst);
}

#ifndef container_of
#define container_of(ptr, type, member) ({ \
	const typeof( ((type *)0)->member ) *__mptr = (ptr);  \
	(type *)( (char *)__mptr - offsetof(type,member) );})
#endif

#define SL_ENTRY(ptr, type,member) (container_of(ptr,type,member))

#define SL_FOREACH_SAFE(pstList, pstNode,pstNext) \
	for((pstNode = sl_first(pstList));\
		(NULL != pstNode)&&({(pstNext) = ls_next(pstNode); 1;});\
		(pstNode) = (pstNext))


#endif
