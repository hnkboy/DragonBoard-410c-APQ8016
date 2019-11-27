
#ifndef _HALI_LIST_
#define _HALI_LIST_



typedef struct stgnode_s
{
	struct stgnode_s *pnext;
}SL_NODE_S;

typedef struct stghead_s
{
	SL_NODE_S *pfirst;
}SL_HEAD_S;

static inline void sl_init(SL_HEAD_S *pstlshead)
{

	pstlshead->pfirst = NULL;
}

static inline void sl_addafter(SL_HEAD_S *phead,SL_NODE_S *pnode)
{
    SL_NODE_S *pstnodetemp = NULL;
	if (NULL == phead->pfirst){

		phead->pfirst = pnode;
	}
	else{
        pstnodetemp = phead->pfirst;
		while(NULL != pstnodetemp->pnext)
		{
            pstnodetemp = pstnodetemp->pnext;
		}
		pstnodetemp->pnext = pnode;
	}
}
static inline void sl_addhead(SL_HEAD_S *phead,SL_NODE_S *pnode)
{
	pnode->pnext = phead->pfirst;
	phead->pfirst = pnode;
}

static inline void sl_del(SL_HEAD_S *phead, SL_NODE_S *pnode)
{

    SL_NODE_S *pstnodetemp = NULL;
	if (NULL == phead->pfirst){
        return;
	}
    if (pnode == phead->pfirst){
        phead->pfirst = pnode->pnext;
    }
    else{
        pstnodetemp = phead->pfirst;
		while(pnode != pstnodetemp->pnext)
		{
            pstnodetemp = pstnodetemp->pnext;
		}
		pstnodetemp->pnext = pnode->pnext;
    }
}

static inline SL_NODE_S *sl_next(SL_NODE_S *pnode){

	return (pnode->pnext);
}
static inline SL_NODE_S *sl_first(SL_HEAD_S *phead){

	return (phead->pfirst);
}


#ifndef offsetof
#define offsetof(type, member) ( (int) & ((type*)0) -> member )
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({ \
	const typeof( ((type *)0)->member ) *__mptr = (ptr);  \
	(type *)( (char *)__mptr - offsetof(type,member) );})
#endif

#define SL_ENTRY(ptr, type, member) (container_of(ptr,type,member))

#define SL_FOREACH_SAFE(pstlist, pstnode,pstnext) \
	for((pstnode = sl_first(pstlist));\
		(NULL != pstnode)&&({(pstnext) = sl_next(pstnode); 1;});\
		(pstnode) = (pstnext))

static inline void sl_freeall(SL_HEAD_S *phead, void (*func)(void *))
{
    SL_NODE_S *pstnode,*pstnext;
    SL_FOREACH_SAFE(phead, pstnode, pstnext)
    {
        func((void *)pstnode);
    }
    return;
}



#endif
