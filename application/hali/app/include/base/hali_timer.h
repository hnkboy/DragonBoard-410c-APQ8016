
#ifndef __HALITIMER_
#define __HALITIMER_

ulong hali_timer_create(int iwaits, ulong (*pfcallback)(int),int *pitimerfd);

void hali_timer_destroy(int itimerfd);


#endif

