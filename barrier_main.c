#include <pthread.h>
#include "barrier.h"
#include "errors.h"

#define  THREADS 2
#define ARRAY 6
#define INLOOPS 1
#define OUTLOOPS 1


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
    barrier->updatingIncrement = 0;
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
    return (status != 0 ? status : status2);
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
    printf("barrier count is %d\n", barrier->counter);
    if (--barrier->counter == 0)
    {
        printf("wake up\n");
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
            printf("waiting in a cycle\n");
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


// kepp track of each thread

typedef struct thrad_tag
{
    pthread_t thread_id;
    int number;
    int increment;
    int arrays[ARRAY];
} thread_t;

barrier_t barrier;
thread_t thread[THREADS];

// Start routine for threads
void *thread_routine(void *arg)
{
    thread_t *self = (thread_t*)arg;
    printf("thread %d \n", self->number);
    int in_loop, out_loop, count, status;
    // loop through outlooops barrier cycles;
    for (out_loop = 0; out_loop <= OUTLOOPS; out_loop++)
    {
        status = barrier_wait(&barrier);  // so they start using the increment after it is edited...
        // this inner loop just add a value to each element in the working array
        for (in_loop = 0; in_loop < INLOOPS; in_loop++)
            for (count = 0; count < ARRAY; count++)
            {
                printf("increment is %d\n", self->increment);
                self->arrays[count] += self->increment; // thread 1: 1 2 3 4 5 . thread 2: 2 3 4 5 6 
            }
        status = barrier_wait(&barrier);
        if (status  > 0)
            err_abort(status, "wait on barrier");
        // the barrier causes one thread to return with the special return status -1.
        // The thread receiving this value increments each element in the shared array
        if (status == -1)
        {
            printf("incrment +1\n");
            int thread_num;
            for (thread_num = 0; thread_num < THREADS; thread_num++)
                thread[thread_num].increment += 1;  // thread 1 incrment becomes to 1, thread 2 incrment becomes to 2
        }
    }
    return NULL;
}

int main (int argc, char *argv[])
{
    int thread_count, array_count;
    int status;
    barrier_int(&barrier, THREADS);
    // CREATE a set of threads that will use the barrier
    for (thread_count = 0; thread_count < THREADS; thread_count++)
    {
        thread[thread_count].increment = thread_count;  // thread 1 incremant is 0, thread 2 incrment is 1
        thread[thread_count].number = thread_count;
        for (array_count = 0; array_count < ARRAY; array_count++)
            thread[thread_count].arrays[array_count] = array_count + 1; // thread 1: 1 2 3 4 5. thread 2: 1 2 3 4 5
        status = pthread_create(&thread[thread_count].thread_id, NULL, thread_routine, (void*)&thread[thread_count]);
        if (status != 0)
            err_abort(status, "create thread");
    }
    // now join with each thread
    for (thread_count = 0; thread_count < THREADS; thread_count++)
    {
        status  = pthread_join(thread[thread_count].thread_id, NULL);
       
        if (status != 0)
            err_abort(status, "JOIN thread");
        printf("%02d: ( %d)", thread_count, thread[thread_count].increment);
        for (array_count = 0; array_count < ARRAY; array_count++)
            printf("%d", thread[thread_count].arrays[array_count]);
        printf("\n");
    }
    // to be thorough, destroy the barrier
    barrier_destroy(&barrier);
    return 0;
}


// gcc -Wno-implicit alarm.c -o hello -lpthread

