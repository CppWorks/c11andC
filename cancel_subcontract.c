#include <pthread.h>
#include <limits.h>
#include "errors.h"

#define THREADS 3

typedef struct team_tag
{
    int join_i;
    // join index
    pthread_t workers[THREADS];
    // thread identifiers
} team_t;

// start routine for worker threads. They loop waiting for a cancellation request
void* worker_routine(void *arg)
{
    int c = (int)arg;
    printf("work routine is %d\n",c);
    int counter;
    for (counter = 0; ; counter++)
        if ((counter % 1000) == 0)
            pthread_testcancel();
}

// cancellation cleanup handler for the contractor thread. It will cancel and deatch each worker in the team
void cleanup(void* arg)
{
    team_t *team = (team_t*)arg;
    int count, status;
    printf("join i is %d\n",team->join_i);
    for (count = 0; count < THREADS; count++)
    {
        status = pthread_cancel(team->workers[count]);
        if (status != 0)
            err_abort(status, "canel worker");
        status = pthread_detach(team->workers[count]);
        if (status != 0)
            err_abort(status, "deatch worker");
        printf("Cleanup: canceled %d\n", count);
    }
}

// thread start routine for the contractor. It creates a team of worker thread, and join with them. When canceled, the cleanup
// handler will cancel and deatch the remaining threads; so we can quit while the workers finish their cancel. don't hang...

void *thread_routine(void* arg)
{
    team_t team; // team info
    int count;
    void* result;
    // return status
    int status;
    for (count = 0; count < THREADS; count++)
    {
        status = pthread_create(&team.workers[count], NULL, worker_routine, (void*)(count));
        if (status != 0)
            err_abort(status, "create worker");
    }
    pthread_cleanup_push(cleanup, (void*)&team);
    for (team.join_i = 0; team.join_i < THREADS; team.join_i++)
    {
         printf("im here with %d\n", team.join_i);
        status = pthread_join(team.workers[1], &result);
        if (status != 0)
            err_abort(status, "join worker");
    }
    printf("im here ahhaha\n");
    pthread_cleanup_pop(0);
    return NULL;
}

int main (int argc, char *argv[])
{
    pthread_t thread_id;
    int status;

    status = pthread_create(&thread_id, NULL, thread_routine, NULL);

    if (status != 0)
        err_abort(status, "Create TEAM");
    sleep(5);
    printf("cancelling....\n");
    status = pthread_cancel(thread_id);
    if (status != 0)
        err_abort(status, "cancel team");

    status = pthread_join(thread_id, NULL);
    if (status != 0)
        err_abort(status, "JOIN team");
    // we will finish until all threadss are done
    pthread_exit(NULL);
    return 0;
}
// gcc -Wno-implicit alarm.c -o hello -lpthread
