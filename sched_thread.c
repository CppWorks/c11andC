#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include "errors.h"

#define THREADS 5
// Structure describing each thread
typedef struct thread_tag{
    int index;
    pthread_t id;
} thread_t;

thread_t threads[THREADS];
int rr_min_priority;

// Thread start routine that will e its own priority
// pass in thread it self...with index for priority set...
void *thread_routine(void *arg)
{
    thread_t *self = (thread_t*)arg;
    int my_policy;
    struct sched_param my_param;
    int status;
    my_param.sched_priority = rr_min_priority + self->index;
    DPRINTF(("Thread %d will set SCHED_FIFO, priority %d\n", self->index,my_param.sched_proirity));
    // for a running thread we set the policy and priority in a single call
    status = pthread_setschedparam(self->id, SCHED_RR, &my_param);
    if (status != 0)
        err_abort(status, "set sched");
    status = pthread_getschedparam(self->id, &my_policy, &my_param);
    if (status != 0)
        err_abort(status, "get sched");
    printf("thread routine %d, running at %s/%d\n", self->index, (my_policy == SCHED_FIFO ? "FIFO" : (my_policy == SCHED_RR ? "RR" : (my_policy == SCHED_OTHER ? "Other" : "unkown"))), my_param.sched_priority);
    return NULL;
}


int main (int argc, char *argv[])
{
    int count, status;
    rr_min_priority = sched_get_priority_min(SCHED_RR);
    if (rr_min_priority == -1)
    {
        err_abort(rr_min_priority, "get RR min");
    }
    for (count = 0; count < THREADS; count++)
    {
        threads[count].index = count;
        status = pthread_create(&threads[count].id, NULL, thread_routine, (void *)&threads[count]);
        if (status != 0)
            err_abort(status, "create  thread");
    }
    for (count = 0; count < THREADS; count++)
    {
        status = pthread_join(threads[count].id, NULL);
        if (status != 0)
            err_abort(status, "JOIN THREAD");
    }
    printf("Main thread exiting");
    return 0;
}
// gcc -Wno-implicit alarm.c -o hello -lpthread
