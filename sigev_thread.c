#include <pthread.h>
#include <signal.h>
#include "errors.h"
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

timer_t timer_id;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


int counter = 0;

// Thread start routine to nofity the application when the timer expirs. This routine is fun as if it were a new thread, each time the timer expires.

// When the timer has expired 5 times, the ain thread will be awakened and will terminate the program
void timer_thread(void *arg)
{
    int status;
    status  = pthread_mutex_lock(&mutex);
    if(status != 0 )
        err_abort(status, "lock mutex");
    if (++counter >= 5)
    {
        status = pthread_cond_signal(&cond);
        if(status != 0 )
            err_abort(status, "signal condition");
    }
    status = pthread_mutex_unlock(&mutex);
    if(status != 0 )
        err_abort(status, "unlock mutex");
    printf("Timer %d\n", counter);
}


// Compile it with -lrt option. It will get compiled.
int main (int argc, char *argv[])
{
    pthread_t signal_thread_id;
    int status;
    struct itimerspec ts;
    struct sigevent se;
    // set sigevent structre to cause the signal to be delivered by creting a new thread;

    se.sigev_notify = SIGEV_THREAD;
    se.sigev_value.sival_ptr = &timer_id;
    se.sigev_notify_function = timer_thread;
    se.sigev_notify_attributes = NULL;

    // specify a repeating timer that fir3es every 5 seconds
    ts.it_value.tv_sec =  5;  // this is the first expiration duration.
    ts.it_value.tv_nsec = 0;
    ts.it_interval.tv_sec = 5; // this sets the interval between successive calls
    ts.it_interval.tv_nsec = 0;

    DPRINTF(("CREATING TIMER\N"));
    status = timer_create(CLOCK_REALTIME, &se, &timer_id);
    if (status == -1)
        err_abort(status, "create timer");
    printf("SETTING timer for 5-second expiration..\n");
    status = timer_settime(timer_id, 0 , &ts, 0);
    if (status == -1)
        err_abort(status, "set timer");
    status  = pthread_mutex_lock(&mutex);
    if(status != 0 )
        err_abort(status, "lock mutex");
    while (counter < 5)
    {
        status = pthread_cond_wait(&cond, &mutex);
        if (status != 0 )
            err_abort(status, "wait on cond");       
    }
    status = pthread_mutex_unlock(&mutex);
    if(status != 0 )
        err_abort(status, "unlock mutex");
   return 0;
}
// gcc -Wno-implicit alarm.c -o hello -lpthread

