#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include "errors.h"

// Thread start routine. If priority scheduing is suppourted, report the thread's scheduling attributes.

void *thread_routine(void* arg)
{
    int my_policy;
    struct sched_param my_param;
    int status;
    // If the priority scheduling optio nis not defined, then we can do nohting with the output of pthread_getschedparam
    // so just report that the thread ran, and exit
#if defined (_POSIX_THREAD_PRIORITY_SCHEDULING) && !defined (sun)
        status = pthread_getschedparam(pthread_self(), &my_policy, &my_param);
    if (status != 0)
        err_abort(status, "get sched");
    printf("thread routine running at %s/%d\n", (my_policy == SCHED_FIFO ? "FIFO" : (my_policy == SCHED_RR ? "RR" : (my_policy == SCHED_OTHER ? "Other" : "unkonwn"))), my_param.sched_priority);
#else
    printf("thread routine running\n");
#endif
    return NULL;
}

int main (int argc, char *argv[])
{
    pthread_t thread_ID;
    pthread_attr_t thread_attr;
    int thread_policy;
    struct sched_param thread_param;
    int status, rr_min_priority, rr_max_prority;
    status = pthread_attr_init(&thread_attr);
    if (status != 0)
        err_abort(status, "init attr");
    // if priority scheduing option is defined, set various scheduling paramters, note that it is important that you remember to set the inheritsched attribute to PTHREAD_EXPLICIT_SHED,
    // or the policy and priority that you've set will be ignored . The default is to inherit scheduling information from the crating thread
#if defined (_POSIX_THREAD_PRIORITY_SCHEDULING) && !defined(sun)
        status = pthread_attr_getschedpolicy(&thread_attr, &thread_policy);
    if (status != 0)
        err_abort(status, "get policy");
    status = pthread_attr_getschedparam(&thread_attr, &thread_param);
    if (status != 0)
        err_abort(status, "get sched param");
    printf("Default public is %s, priority is %d\n", (thread_policy == SCHED_FIFO ? "FIFO" :  (thread_policy == SCHED_RR ? "RR" : (thread_policy == SCHED_OTHER ?  "Other" : "unkonwn"))), thread_param.sched_priority);
    //status = pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);
    // do them through attribute
    status = pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);
    if (status != 0)
        err_abort(status, "unable to set sched_rr policy\n");
    else
    {
        // just for the same of exercise, we will use the middle of the priority range allowed for sched_rr. this should ensure tha the thread
        // will be run, without blocking everything else. Because any assumptions about how a thread's priority interacts with other threads(even in other processes)
        // are nonportable, especially on an implementaion that defaults to system contention scope.
        // you may have t oadjust this code befoer it will work on some system
        rr_min_priority = sched_get_priority_min(SCHED_RR);
        if (rr_min_priority == -1)
            err_abort(rr_min_priority, "get sched rr min priority");
        rr_max_prority =  sched_get_priority_max(SCHED_RR);
        if (rr_max_prority == -1)
            err_abort(rr_max_prority, "get sched rr max priority");
        thread_param.sched_priority = (rr_min_priority + rr_max_prority)/2;
        printf("sched rr priority RANGE is %d to %d, use %d\n", rr_min_priority, rr_max_prority, thread_param.sched_priority);
        status = pthread_attr_setschedparam(&thread_attr, &thread_param);
        if (status != 0)
            err_abort(status, "set params\n");
        printf("creating thread at RR %d\n", thread_param.sched_priority);
        status = pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED);
        if (status != 0)
            err_abort(status, "set inherit\n");
    }
#else
    printf("priority scheduling not suppourted\n");
#endif
    status = pthread_create(&thread_ID, &thread_attr, thread_routine, NULL);

    if (status != 0)
        err_abort(status, "CREATE thread \n");
    status = pthread_join(thread_ID, NULL);

    if (status != 0)
        err_abort(status, "join thread \n");
    printf("main exiting\n");
    return 0;
}
// gcc -Wno-implicit alarm.c -o hello -lpthread
