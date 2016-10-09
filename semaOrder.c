
#include "errors.h"

#include <pthread.h>
#include <stdio.h>

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

sem_t mySemaphore;

void *f1(void *param)
{
    printf("thread %d\n", (long)param);

    // signal semaphore
    // increment the value of mySemaphore by 1
    // if there are one or more threads waiting, wake
    sem_post(&mySemaphore);
}

void *f2(void *param)
{
    // wait until value of semaphore is greater than 0
    // decrement the value of semaphore by 1
    sem_wait(&mySemaphore);

    printf("thread %d\n", (long)param);
}


int main (int argc, char *argv[])
{


    pthread_t myThread[2];
    pthread_create(&myThread[0], 0, f1, 0);
    pthread_create(&myThread[1], 0, f2, 1);
    pthread_join(myThread[0], 0);
    pthread_join(myThread[1], 0);
    return 0;
}
// gcc -Wno-implicit alarm.c -o hello -lpthread

