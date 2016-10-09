#include <pthread.h>
#include <limits.h>
#include "errors.h"

// structure used as the value for thread-specific data key
typedef struct tsd_tag
{
    pthread_t thread_id;
    char *string;
} tsd_t;

pthread_key_t tsd_key; // thread-specifid data key

pthread_once_t key_once = PTHREAD_ONCE_INIT;

// one-time initialisation routine used with the pthread once control block

void once_routie(void)
{
    int status;
    printf("initialising key\n");
    status = pthread_key_create(&tsd_key, NULL);
    if (status != 0)
        err_abort(status, "Create KEY");
}

// THREAD START ROUTINE THAT USES PTHREA_ONCE TO DYNAMICALLY CREATE A THREAD-SPECIFID DATA KEY

void *thread_routine(void* arg)
{
    tsd_t* value;
    int status;
    status = pthread_once(&key_once, once_routie);
    if (status != 0)
        err_abort(status, "once init");
    value = (tsd_t*)malloc(sizeof(tsd_t)); // private for each thread
    if (value == NULL)
        printf("allocate key value");
    status = pthread_setspecific(tsd_key, value);
    if (status != 0)
        err_abort(status, "set tsd");
    printf("%s set tsd value %p\n", (char*)arg, value);
    value->thread_id = pthread_self();
    value->string = (char*)arg;
    value = (tsd_t*) pthread_getspecific(tsd_key);
    printf("%s starting...\n", value->string);
    sleep(2);
    value = (tsd_t*) pthread_getspecific(tsd_key);
    printf("\%s done ...\n", value->string);
    return NULL;
}
int main (int argc, char *argv[])
{
    pthread_t thread1, thread2;
    int status;
    status = pthread_create(&thread1, NULL, thread_routine, "thread 1");
    if (status != 0)
        err_abort(status, "cretae thrad 1");
    status = pthread_create(&thread2, NULL, thread_routine, "thread 2");
    if (status != 0)
        err_abort(status, "cretae thrad 2");
    pthread_exit(NULL);
}
// gcc -Wno-implicit alarm.c -o hello -lpthread
