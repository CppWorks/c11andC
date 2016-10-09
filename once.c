#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "errors.h"
pthread_once_t once_block = PTHREAD_ONCE_INIT;
pthread_mutex_t mutex;
// This is one time initialisation route. It will be called exactly once, no matter how many calls to pthread_once with the same control structure are made dring the course of the program

void once_init_routine(void)
{
    int status;
    status = pthread_mutex_init(&mutex, NULL);
    printf("init once\n");
    if (status != 0)
        err_abort(status, "init mutex");
    // Thread start route that calls pthread_once
}

void* thread_routine(void *arg)
{
    int status;
    status = pthread_once(&once_block, once_init_routine);
    if (status !=0)
        err_abort(status, "once init");
    status = pthread_mutex_lock(&mutex);
    if (status !=0)
        err_abort(status, "lock mutex");
    printf("thread routine has locked mutex .\n");
    status = pthread_mutex_unlock(&mutex);
    if (status !=0)
        err_abort(status, "unlock mutex");
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t thread_id;
    char *input, buffer[64];
    int status;
    status = pthread_create(&thread_id, NULL, thread_routine, NULL);
    if (status !=0)
        err_abort(status, "CREATE THREAD");
    status = pthread_once(&once_block, once_init_routine);
    if (status !=0)
        err_abort(status, "once init");
    if (status !=0)
        err_abort(status, "lock mutex");
    printf("main has the lock mutex.\n");
    status = pthread_mutex_unlock(&mutex);
    if (status !=0)
        err_abort(status, "unlock mutex");

    status = pthread_join(thread_id, NULL);
    if (status !=0)
        err_abort(status, "JOIN THREAD");

}