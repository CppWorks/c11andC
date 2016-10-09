#include <pthread.h>
#include <limits.h>
#include "errors.h"

#define THREADS 2
// control structure shared by the test threads containing the synchoroniaion and invariant data
typedef struct control_tag {
    int counter, busy;
    pthread_mutex_t mutex;
    pthread_cond_t cv;
} control_t;

control_t control = {0, 1, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};

// This routie is installed as the cancellation clean up handler around the cancelable condition wait. It will be called by the system when the therad is canceled

void cleanup_handler(void* arg)
{
    // mutex is locked at this moment...
    control_t *st = (control_t*)arg;
    int status;
    st->counter--;
    printf("clean up handler: counter == %d\n", st->counter);
    status = pthread_mutex_unlock(&st->mutex);
    if (status != 0)
        err_abort(status, "unlock in clean up handler");
}
    // Mutlple threads are created running this routine (controlled by the THREADS macro). This maintain a counter invariant
    // which expresses the number of running threads. They specify a nonzero value to pthrad_cleanup_pop to run the same finalization action when cancellation nodes not occur
void *thread_routine(void* arg)
{
    printf("im starting\n");
    int status;
    pthread_cleanup_push(cleanup_handler, (void*)&control);
    status = pthread_mutex_lock(&control.mutex);
    if (status != 0)
        err_abort(status, "lock mutex");
    control.counter++;
    while (control.busy)
    {
        status = pthread_cond_wait(&control.cv, &control.mutex);
        if (status != 0)
            err_abort(status, "wait on condition");
    }
    pthread_cleanup_pop(1);
    return NULL;
}

int main (int argc, char *argv[])
{
    pthread_t thread_id[THREADS];
    int count;
    void *result;
    int status;

    for (count == 0; count < THREADS; count++)
    {
        status = pthread_create(&thread_id[count], NULL, thread_routine, NULL);
        if (status != 0)
            err_abort(status, "Create thread");
    }
    sleep(5);

    for (count = 0; count < THREADS; count++)
    {
        status = pthread_cancel(thread_id[count]);
        if (status != 0)
            err_abort(status, "cancel thread");
        status = pthread_join(thread_id[count], &result);
        if (status != 0)
            err_abort(status, "join thread");
        if (result == PTHREAD_CANCELED)
            printf("thread %d canceled\n", count);
        else
            printf("thread %d is not cancled\n", count);
    }

    return 0;
}
// gcc -Wno-implicit alarm.c -o hello -lpthread
