#include <pthread.h>
#include <signal.h>
#include "errors.h"

#define THREAD_COUNT 1
#define ITERATIONS 940000

unsigned long thread_count = THREAD_COUNT;
unsigned long iterations = ITERATIONS;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
volatile sig_atomic_t sentinel = 0;
pthread_once_t once = PTHREAD_ONCE_INIT;
pthread_t *array = NULL, null_pthread = {0};
int bottom = 0;
int inited = 0;

// handle sigusra in the target thread, to suspend it until receiving sigsur2(resume)
// it is all about signal catching
void suspend_signal_handler(int sig)
{
    sigset_t signal_set;
    // block all signals except sigusrs2 while suspended
    sigfillset(&signal_set); // initialise with all signals
    sigdelset(&signal_set, SIGUSR2);  // delete SIGUSR2
    sentinel = 1;
    sigsuspend(&signal_set); // this will wait for the signal SIGUSR2
    // once i am here, i have been resumed and the resume signal handler has been run to completion
    return;
}

// handle SIGUSR2 in the target thread to resume it. notte that the signal handler does nothing. It exists only because we need to cause sigsuspend() to return
void resume_signal_hanlder(int sig)
{
    return;
}

// dynamically initialize the suspend package when first used called by pthread_once
void suspend_init_routine(void)
{
    int status;
    struct sigaction sigusr1, sigusr2;
    // allocate the suspened threads array. This array is used to guarantee idempotency
    bottom = 10;
    array = (pthread_t *)calloc(bottom, sizeof(pthread_t));
    // install the signal handlers for suspend resume
    sigusr1.sa_flags = 0;
    sigusr1.sa_handler = suspend_signal_handler;
    sigemptyset(&sigusr1.sa_mask); //initialises the signal set pointed to by set, such that all signals defined in this document are excluded.
    sigaddset(&sigusr1.sa_mask, SIGUSR2); // he sigaddset() function adds the individual signal specified by the signo to the signal set pointed to by set.

    sigusr2.sa_flags = 0;
    sigusr2.sa_handler = resume_signal_hanlder;
    sigemptyset(&sigusr2.sa_mask);

    status = sigaction(SIGUSR1, &sigusr1, NULL);

    if (status == -1)
    {
        printf("installing suspend handler\n");
    }
    status = sigaction(SIGUSR2, &sigusr2, NULL);
    if (status == -1)
    {
        printf("installing resume handler\n");
    }
    inited = 1;
    return;
}


// Suspend a thread by sending it a signal (SIGUSR1), which will block the thread until another signal (SIGUSR2) arrives
// multiple calls to the thd_suspend for a single thread have no additional effect on the thread -- a signle thd_continue call will cause it to resume execution

int thd_suspend(pthread_t target_thread)
{
    int status;
    int i = 0;
    // the first call to thd_suspend will initiailisee the package
    status = pthread_once(&once, suspend_init_routine);
    if (status != 0)
        return status;
    // seialise access to suspend, makes life easier
    status  = pthread_mutex_lock(&mut);
    if (status != 0)
        return status;
    // threads that are suspended are added to the target_array
    // a request to suspend a thread already listed inthe array is ignored. Sending a second SIGUSR1 would cause the thread to resuspend itself as soon as it is resumed
    while (i < bottom)
    {
        if (pthread_equal(array[i++], target_thread))
        {
            status = pthread_mutex_unlock(&mut);
            return status;
        }
    }
    // ok we really need to suspend this thread, so lets find the location in the array that we will use, if we run off the end, realloc the array for more space
    i = 0;
    while (i < bottom && array[i] != 0)
    {
        i++;
    }
    if (i==bottom)
    {
        array = (pthread_t *)realloc(array, (++bottom * sizeof(pthread_t)));
        if (array == NULL)
        {
            pthread_mutex_unlock(&mut);
            return errno;
        }
        array[bottom] = null_pthread;
        // clear new entry
    }
    // clear the sentinel and signal the thread to suspend
    sentinel = 0;
    status = pthread_kill(target_thread, SIGUSR1);
     printf("im here with %d\n", status);
    if (status != 0)
    {
        pthread_mutex_unlock(&mut);
        printf("i fail\n");
        return status;
    }
    // wait for the sentinel to change
    while (sentinel ==0)
        sched_yield();

    array[i] = target_thread;
    status = pthread_mutex_unlock(&mut);
    return status;



}

// Resume a suspended thread by sending it SIGUSR2 to break it out of the siguspend() in which it is waiting. Itf the target thread is not suspended, return with successs
int thd_continue(pthread_t target_thread)
{
    int status ;
    int i = 0;
    // the first call to thd_continue will initiase the package.
    status = pthread_once(&once, suspend_init_routine);
    if (status != 0)
        return status;
    // serialise access to suspend/resume, to make life easier
    status = pthread_mutex_lock(&mut);
    if (status != 0)
        return status;
    // make sure the thread is in the suspend array. If not, it has not been suspended (or it has already been resumed) and we can just carry on
    while (i < bottom && pthread_equal(array[i], target_thread))
        i++;
    if (i==bottom)
    {
        pthread_mutex_unlock(&mut);
        return 0;
    }
    //signal the thread to continue and remove the thread from the suspended array
    status = pthread_kill(target_thread, SIGUSR2);
    if (status != 0)
    {
        pthread_mutex_unlock(&mut);
        return status;
    }
    array[i] = 0;
    // clear array element
    status = pthread_mutex_unlock(&mut);
    return status;
}

static void* thread_routine(void *arg)
{
    int number = (int) arg;
    int status;
    int i;
    char buffer[128];
    for (i = 1; i <= iterations; i++)
    {
        //sleep(1);
        // everytime each thread does 2000 interations, print a progress report
        if (i % 2000 == 0)
        {
            sprintf(buffer, "Thread %02d: %d\n", number, i);
            write(1, buffer, strlen(buffer));
            sleep(1);
        }
        sched_yield();
    }
    return (void*)0;
}

void sig_handler(int signo)
{
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");
    else if (signo == SIGKILL)
        printf("received SIGKILL\n");
    else if (signo == SIGSTOP)
        printf("received SIGSTOP\n");
}


int main (int argc, char *argv[])
{


    pthread_t threads[THREAD_COUNT];
    pthread_attr_t detach;
    int status;
    void* result;
    int i;
    status = pthread_attr_init(&detach);
    if(status != 0 )
        err_abort(status, "init attr");
    status = pthread_attr_setdetachstate(&detach, PTHREAD_CREATE_DETACHED);
    if(status != 0 )
        err_abort(status, "set detach");
    for (i=0; i < THREAD_COUNT; i++)
    {
        status = pthread_create(&threads[i], &detach, thread_routine, (void*)i);
        if(status != 0 )
            err_abort(status, "create threaed");
    }
    sleep(1);


 //   for (i = 0; i < THREAD_COUNT/2; i++)
    i = 0;
    {
        printf("Suspending thread %d.\n", i);
      //  status = pthread_kill(threads[0], SIGUSR1);
      //  printf("Suspending status %d.\n", status);
        status = thd_suspend(threads[i]);
        if (status != 0)
            err_abort(status, "suspend threaed");
    }

    printf("sleeping.,...\n");
    sleep(2);
    //for (i = 0 ; i < THREAD_COUNT/2; i++)
    //{
        printf("continuing thread %d \n",i);
        status = thd_continue(threads[i]);
        if (status != 0)
            err_abort(status, "continus threaed"); 
    //}

/*    for (i = THREAD_COUNT/2; i < THREAD_COUNT; i++)
    {
        printf("Suspending thread %d.\n", i);
        status = thd_suspend(threads[i]);
        if (status != 0)
            err_abort(status, "suspend threaed");
    }

    printf("sleeping.,...\n");
    sleep(2);
    for (i = THREAD_COUNT/2 ; i < THREAD_COUNT; i++)
    {
        printf("continue thread %d \n",i);
        status = thd_continue(threads[i]);
        if (status != 0)
            err_abort(status, "continus threaed"); 
    }*/
   pthread_exit(NULL);
}
// gcc -Wno-implicit alarm.c -o hello -lpthread

