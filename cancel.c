#include <pthread.h>
#include <limits.h>
#include "errors.h"


static int counter;

// Thread start routine that loop until cenceld.
// Thread can be canceled only when it calls pthread_testcancel, which it does each 1000 iterations.

void * thread_routine(void *arg)
{
    DPRINTF(("thread_routing_starting\n"));
    for (counter = 0; ; counter++)
    {
        if (counter % 1000 == 0)
        {
            DPRINTF(("calling testcancel\n"));
            pthread_testcancel();  // This creates a cancellation point where we can respond to a cancellation request.
        }
    }

}

int main (int argc, char *argv[])
{
    pthread_t thread_id;
    void *result;

    pthread_attr_t thread_attr;

    int status;

    status = pthread_create(&thread_id, NULL, thread_routine, NULL);
    if (status != 0)
        err_abort(status, "Create thread");
    
    sleep(2);
    DPRINTF(("calling cancel\n"));
    status = pthread_cancel(thread_id);
    if (status != 0)
        err_abort(status, "cancel thread");

    DPRINTF(("join\n"));
    status = pthread_join(thread_id, &result);
    if (status != 0)
        err_abort(status, "join thread");
    if (result == PTHREAD_CANCELED)
        printf("thread canceled at iteration %d\n", counter/1000);
    else
        printf("Thread was not cancelled\n");

    return 0;
}
// gcc -Wno-implicit alarm.c -o hello -lpthread
