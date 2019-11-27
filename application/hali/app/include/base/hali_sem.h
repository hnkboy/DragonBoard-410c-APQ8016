
#ifndef __HALISEM_
#define __HALISEM_



typedef enum enSemtype
{
    HALI_SEM_THRDSYN = 1,
    HALI_SEM_MAX
}HALI_SEMTYPE_E;

ulong hali_sem_init();

void hali_sem_fini();

void hali_sem_post(HALI_SEMTYPE_E e);
void hali_sem_wait(HALI_SEMTYPE_E e);


#endif


