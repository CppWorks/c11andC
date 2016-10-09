#include <pthread.h>
#include <signal.h>
#include "errors.h"
#include <unistd.h>
#include <sys/types.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int interrupted = 0;
sigset_t signal_set;

// Wait for the SIGINT signal. When it has occurred 5 times, set the interrrupted flat (the main thread's wait predicate) and signal a condition variable. The main thread will exit
void* signal_waiter(void *arg)
{
    int sig_number;
    int signal_count = 0;
    int status;
    while (1)
    {
        sigwait(&signal_set, &sig_number);
        if (sig_number == SIGINT)
        {
            printf("Got SIGINT (%d of 5)\n", signal_count+1);
            if (++signal_count >= 5)
            {
                status  = pthread_mutex_lock(&mutex);
                if(status != 0 )
                    err_abort(status, "lock mutex");
                interrupted = 1;
                status = pthread_cond_signal(&cond);
                if(status != 0 )
                    err_abort(status, "signal condition");
                status = pthread_mutex_unlock(&mutex);
                if(status != 0 )
                    err_abort(status, "unlock mutex");
                break;
            }
        }
    }
    return NULL;
}


int main (int argc, char *argv[])
{
    pthread_t signal_thread_id;
    int status;
    //start by masking the interesting signal, SIGINT in the intial thread. Because all threads inherit the signal mask from their creattor
    // all threads in the precess will have SIGINT masked unless one explicility unmask it. The semantics of sigwait requires that all threads
    // inlcuindg the thread calling sigwait have the signal masked, for reliable operations. Otherwise ,a signal that arrives while the sigwaiter is not blocked in sigwait might
    // be delievered to anothe thread
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGINT);
    status = pthread_sigmask(SIG_BLOCK, &signal_set, NULL);
    if(status != 0 )
        err_abort(status, "SET SIGNAL MAST");
    // create the sigwait thread
    status = pthread_create(&signal_thread_id, NULL, signal_waiter, NULL);
    if(status != 0 )
        err_abort(status, "create sigwaiter");

    // wait for the sigwait thread to receive SIGINT and signal the condition variable
    status = pthread_mutex_lock(&mutex);
    if(status != 0 )
        err_abort(status, "lock mutex");
    while (!interrupted)
    {
        status = pthread_cond_wait(&cond, &mutex);
        if(status != 0 )
            err_abort(status, "wait for interrupt");
    }

    status = pthread_mutex_unlock(&mutex);
    if(status != 0 )
        err_abort(status, "unlock mutex");
    printf("main terminating with SIGINT\n");
   return 0;
}
// gcc -Wno-implicit alarm.c -o hello -lpthread

