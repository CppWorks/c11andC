#include <pthread.h>
#include <signal.h>
#include "errors.h"
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <time.h>

sem_t semaphore;


// Wait for the SIGINT signal. When it has occurred 5 times, set the interrrupted flat (the main thread's wait predicate) and signal a condition variable. The main thread will exit
void* signal_waiter(void *arg)
{
    printf("im in threaD\n");
    return NULL;
};


// sginal catching funciton

void signal_catcher(int sig)
{
    // increments (unlocks) the semaphore pointed to by sem. so decrement can happen with positive value...wake up thread..
    if (sem_post(&semaphore) == -1 || sem_post(&semaphore) == -1)  // so we can talk to threads in here without threads objects.
        err_abort(1, "post semphore");
    int* sval = (int*) malloc(sizeof(int)); 
    sem_getvalue(&semaphore, sval);
    printf("signal sem value is %d\n", *sval);
};

// Thread start routine which waits on te semaphone

void* sem_waiter(void *arg)
{
    int number = (int) arg;
    int counter;
    // each thread waits 5 times
    for (counter = 1; counter <= 5; counter++)
    {
        int* sval = (int*) malloc(sizeof(int)); 
        sem_getvalue(&semaphore, sval);
        printf("sem value is %d\n", *sval);

        while (sem_wait(&semaphore) == -1) // block if semaphore is zero
        {
            if (errno != EINTR)
                err_abort(1, "wait on semaphoere");
        };
         sem_getvalue(&semaphore, sval);
        printf("sem value after wake is %d\n", *sval);
        printf("%d waking (%d)...\n", number, counter);
    }
    return NULL;
};

#define THREADS 1

int main (int argc, char *argv[])
{
     int thread_count, status;
     struct sigevent sig_event;
     struct sigaction  sig_action;
     sigset_t sig_mask;
     timer_t timer_id;
     struct itimerspec timer_val;
     pthread_t sem_waiters[THREADS];

    sem_init(&semaphore, 0, 0);
    // create 5 threads tro wait on a semaphore
    for (thread_count = 0; thread_count < THREADS; thread_count++)
    {
        printf("im here\n");
        status = pthread_create(&sem_waiters[thread_count], NULL, sem_waiter, (void*)thread_count);
       // status = pthread_create(&sem_waiter[thread_count], NULL, signal_waiter, NULL);
        if(status != 0 )
            err_abort(status, "create sigwaiter");
    }
    // Set up a repeating timer using signal number SIGRTMIN
    // set to occur every 2 seconds;
    sig_event.sigev_value.sival_int = 0;
    sig_event.sigev_signo = SIGRTMIN;
    sig_event.sigev_notify = SIGEV_SIGNAL; // Upon timer expiration, generate the signal sigev_signo for the process
    if (timer_create(CLOCK_REALTIME, &sig_event, &timer_id) == -1)
    {
        err_abort(1, "create timer");
    }
    sigemptyset(&sig_mask);

    sigaddset(&sig_mask, SIGRTMIN);
    sig_action.sa_handler = signal_catcher;
    sig_action.sa_mask = sig_mask;
    sig_action.sa_flags = 0;
    if (sigaction(SIGRTMIN, &sig_action, NULL) == -1)
        err_abort(1, "SET SIGNAL ACTION");
    timer_val.it_interval.tv_sec = 2;
    timer_val.it_interval.tv_nsec = 0;
    timer_val.it_value.tv_sec = 2;
    timer_val.it_value.tv_nsec = 0;


    if (timer_settime(timer_id, 0, &timer_val, NULL) == -1)
        err_abort(1, "Set timer");
    // wait for all threads to complete
    for (thread_count = 0; thread_count < 5; thread_count++)
    {
        status = pthread_join(sem_waiters[thread_count], NULL);
        if(status != 0 )
            err_abort(status, "join thread");
    }
  //  pthread_exit(NULL);
    return 0;
}
// gcc -Wno-implicit alarm.c -o hello -lpthread --lrt

