
#ifndef _LIST_
#define _LIST_
typedef struct stgnode_s
{
	struct stgnode_s *pNext;
}SL_NODE_S;

typedef struct stghead_s
{
	SL_NODE_S *pFirst;
}SL_HEAD_S;





static inline void sl_init(SL_HEAD_S *pstLsHead){

	pstLsHead->pFirst = NULL;
}

static inline void sl_addafter(SL_HEAD_S *pHead,SL_NODE_S *pNode){
    SL_NODE_S *pstNodeTemp = NULL;
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
static inline void sl_addhead(SL_HEAD_S *pHead,SL_NODE_S *pNode){
	pNode->pNext = pHead->pFirst;
	pHead->pFirst = pNode;
}

static inline void sl_del(SL_HEAD_S *pHead,SL_NODE_S *pNode){

    SL_NODE_S *pstNodeTemp = NULL;
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

static inline SL_NODE_S *ls_next(SL_NODE_S *pNode){

	return (pNode->pNext);
}
static inline SL_NODE_S *sl_first(SL_HEAD_S *pHead){

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
