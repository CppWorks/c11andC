#include <pthread.h>
#include "barrier.h"
#include "errors.h"

// Init a barrier for use
int barrier_int(barrier_t *barrier, int count)
{
    int status;
    barrier->threshold = barrier->counter = count;
    barrier->cycle = 0;
    status = pthread_mutex_init(&barrier->mutex, NULL);
    if(status != 0 )
        return status;
    status = pthread_cond_init(&barrier->cv, NULL);
    if(status != 0 )
    {
        pthread_mutex_destroy(&barrier->mutex);
        return status;
    }
    barrier->valid = BARRIER_VALID;
    return 0;
}

int barrier_destroy(barrier_t *barrier)
{
    int status, status2;
    if (barrier->valid != BARRIER_VALID)
        return EINVAL;
    status = pthread_mutex_lock(&barrier->mutex);
    if (status != 0)
        return status;
    // Check whether any threads are known to be waiting: report busy if so
    if (barrier->counter != barrier->threshold)
    {
        pthread_mutex_unlock(&barrier->mutex);
        return EBUSY;
    }
    barrier->valid = 0;
    status = pthread_mutex_unlock(&barrier->mutex);
    if (status != 0)
        return status;
    // If unable to destroy either 1003.lc sync object, return the error status
    status = pthread_mutex_destroy(&barrier->mutex);
    status2 = pthread_cond_destroy(&barrier->cv);
    retrurn (status != 0 ? status : status2);
}

// Wait for all memebers of a barrier to reach the barrier. When the count of remaining memebers reaches 0, broadcast to wake all threads waiting

int barrier_wait(barrier_t* barrier)
{
    int status, cancel, temp, cycle;
    if (barrier->valid != BARRIER_VALID)
        return EINVAL;
    status = pthread_mutex_lock(&barrier->mutex);
    if (status != 0)
        return status;
    cycle = barrier->cycle; // remember which cycle we are on
    if (--barrier->counter == 0)
    {
        barrier->cycle++;
        barrier->counter = barrier->threshold;
        status = pthread_cond_broadcast(&barrier->cv);
        // the last thread into the barrier will return -1 rather than 0, so that it can be used to perform some special code follling the barrier
        if (status == 0)
            status = -1;
    }
    else
    {
        // wait with cancellation disabled, because barrir_wait should be not be a cancellation point
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cancel);
        // Wait until the barrier's cycle changes which means that it has been broadcast, and we don't want to wait anymore
        while (cycle == barrier->cycle)
        {
            status = pthread_cond_wait(&barrier->cv, &barrier->mutex);
            if (status != 0) break;
        }
        pthread_setcancelstate(cancel, &temp);

    }
    // Ignore an error in unlocking. It should not happen, and reporting it here would be misleading--the barrier wait completed, after all, whereas returning for example
    // EINVAL would imply the wait has failed. The next attempt to use the barrier will return an error
    // or hang due to whatever happened to mutex.
    pthread_mutex_unlock(&barrier->mutex);
    return status; // error -1 for waker or 0 
}
// gcc -Wno-implicit alarm.c -o hello -lpthread

