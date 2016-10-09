#include <pthread.h>
#include "workq.h"
#include "errors.h"
#include <stdlib.h>
#include <time.h>

int workq_init(workq_t *wq, 
    int threads, //max threads
    void (*engine)(void *))
{
    int status;
    int status1;
    status = pthread_attr_init(&wq->attr);
    if (status != 0)
        return status;
    status = pthread_attr_setdetachstate(&wq->attr, PTHREAD_CREATE_DETACHED);
    if (status != 0)
    {
      //  pthread_attr_destroy(&wq->attr);
        return status;
    }
    status = pthread_mutex_init(&wq->mutex, NULL);
    if (status != 0)
    {
      //  pthread_attr_destory(&wq->attr);
        return status;
    }
    status = pthread_cond_init(&wq->cv, NULL);
    if (status != 0)
    {
        pthread_mutex_destroy(&wq->mutex);
     //   status = pthread_attr_destory(&wq->attr);
        return status;
    }
    wq->quit = 0; // not time to quit
    wq->first = wq->last = NULL; // NO queue entries
    wq->parallelism = threads; // max servers;
    wq->counter = 0; // no server threads yet
    wq->idle = 0 ;// no idle servers;
    wq->engine = engine;
    wq->valid = WORKQ_VALID;
    return 0;
}

// destory a work queue
int workq_destroy(workq_t *wq)
{
    int status, status1, status2;
    if (wq->valid != WORKQ_VALID)
        return EINVAL;
    status = pthread_mutex_lock(&wq->mutex);
    if (status != 0)
        return status;
    wq->valid = 0; //prevent any other operations
    // check whether any threads are active and run them down
    // set the quit flag, broadcast to wake any servers that may be asleep. wait for all threads to quit (counter goes to 0)
    // becasue we dont use join, we dont need to worry about tracking thread ids
    if (wq->counter > 0)
    {
        wq->quit = 1;
        // if any threads are idling, wake them
        if (wq->idle > 0)
        {
            status = pthread_cond_broadcast(&wq->cv);
            if (status != 0)
            {
                pthread_mutex_unlock(&wq->mutex);
                return status;
            }

        }
        // just to prove that every rule has an exception. Im using the cv condition for two seprate predicates here
        // Thats ok, since the case used here applies only once during the life of a work queue --during rundown.
        // The overhead is minimal and it is not worth creating a seperae condition variable that would wait and be signaled exactly once!
        while (wq->counter > 0)
        {
            status = pthread_cond_wait(&wq->cv, &wq->mutex);
            if (status != 0)
            {
               
                pthread_mutex_unlock(&wq->mutex);
                return status;
            }

        }
    }
     printf("im quiting with counter %d\n", wq->counter);
    status = pthread_mutex_unlock(&wq->mutex);
    if (status != 0)
    {
        return status;
    }
    status = pthread_mutex_destroy(&wq->mutex);
    status1 = pthread_cond_destroy(&wq->cv);
   // status2 = pthread_attr_destory(&wq->attr);
    printf("%d value \n", status);
    return (status);
}


// thread start routine to serve the work queue
static void *workq_server(void *arg)
{
    struct timespec timeout;
    workq_t* wq = (workq_t*)arg;
    workq_ele_t *we;
    int status, timedout;
    // we dont need to validate the workq_t here...we dont create werver threads until requests are queued(the queue has been initialised by then!)
    // we wait for all server trhreads to terminate before destroying a work queue
    printf("a worker is starting\n");
    status = pthread_mutex_lock(&wq->mutex);
     printf("locking in server\n");
    if (status != 0)
    {
        return NULL;
    }
    while (1){
        timedout = 0; 
        printf("worker waititng for work\n");
        clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_sec += 2;
        while (wq->first == NULL && !wq->quit)
        {
            // Sever thredas exit after waiting 2 seconds
            wq->idle++;
            status  = pthread_cond_timedwait(&wq->cv, &wq->mutex, &timeout);
             printf("get work now\n");
            wq->idle--;
            if (status == ETIMEDOUT)
            {
                printf("worker wait timed out\n");
                timedout = 1;
                break;
            }
            else if (status != 0)
            {
                // this should return failure, but thats awkward becaue this routine is running asynchronously. Failure is unlikely, and I have 
                // chosen to avoid that level of complication. The sever thread will return and allow another server thread to pick up the work later.
                // Note that if this were the only server threads, the queue would not be serviced until a new work item is queued. That could be fixed by creating a new server here.
                printf("worker wait failed\n");
                wq->counter--;
                pthread_mutex_unlock(&wq->mutex);
                return NULL;
            }

        }
        printf("work queue 0x%p quite : %d\n", wq->first, wq->quit);
        we = wq->first;
        if (we != NULL)
        {
            wq->first = we->next;
            if (wq->last = we)
                wq->last = NULL;
            status = pthread_mutex_unlock(&wq->mutex);
            if (status != 0)
                return NULL;
            printf("worker calling engine \n");
            wq->engine(we->data);
            free(we);
            status = pthread_mutex_lock(&wq->mutex); // lock to get next item
            if (status != 0)           
                return NULL;
        }
        // if there is no more work requests, and the servers have been asked to quite, then shut down
        if (wq->first == NULL && wq->quit)
        {
            printf("worker shut down\n");
            wq->counter--;
            // note just to prove that every rule has an excepting, im using the cv condition for two separate predicates here
            // That is ok since the case used here applies only once during the life time or a work queue
            // during rundown. The overhead is minimal and its not worh creting a new cond that would wait and be signaled only once
            if (wq->counter == 0)
                pthread_cond_broadcast(&wq->cv);
            status = pthread_mutex_unlock(&wq->mutex);
            return NULL;
        }
        // if there is no more work and we wait for as long as we are allowed, then terminate this server thread;
        if (wq->first == NULL && timedout)
        {
            printf("engine terminating to time out\n");
            wq->counter--;
            break;
        }
    }
    pthread_mutex_unlock(&wq->mutex);
    printf("worker exiting\n");
    return NULL;
}

int workq_add(workq_t *wq, void *element)
{
    workq_ele_t* item;
    pthread_t id;
    int status;
    if (wq->valid != WORKQ_VALID)
        return EINVAL;
    // create and initialise a request structure
    item  = (workq_ele_t*)malloc(sizeof(workq_ele_t));
    if (item == NULL)
        return ENOMEM;
    item->data = element;
    item->next = NULL;
    printf("i want to lock\n");
    status = pthread_mutex_lock(&wq->mutex);
    if (status != 0)
    {
        free(item);
        return status;
    }
    // add request to the end of the queue updating the first and last operator
    if (wq->first == NULL)
        wq->first = item;
    else
        wq->last->next = item;
    wq->last = item;
    // if any threads are idling, wake one
    if (wq->idle > 0)
    {
        status = pthread_cond_signal(&wq->cv);
        if (status != 0)
        {
            pthread_mutex_unlock(&wq->mutex);
            return status;
        }
    }
    else if (wq->counter < wq->parallelism)
    {
        // if there were no idling threads and we are allowed to create a new wthread.do so
        
        status = pthread_create(&id, &wq->attr, workq_server, (void*)wq);
        if (status != 0)
        {
            pthread_mutex_unlock(&wq->mutex);
            return status;
        }
        wq->counter++;
        printf("creating new worker %d\n",  wq->counter);
    }
    status = pthread_mutex_unlock(&wq->mutex);
    printf("unlocking in add\n");

    return 0;
}

#define ITERATIONS 15
typedef struct power_tag
{
    int value;
    int power;
}power_t;

typedef struct engine_tag
{
    struct engine_tag *link;
    pthread_t thread_id;
    int calls;
}engine_t;


pthread_key_t engine_key; // keep track of active engines
pthread_mutex_t engine_list_mutex = PTHREAD_MUTEX_INITIALIZER;
engine_t* engine_list_head = NULL;
workq_t workq;

// THREADS sepecific data destructor routitne for engine_key
void destructor(void *value_ptr)
{
    engine_t* engine = (engine_t*)value_ptr;
    // construct a linked list in reverse order
    pthread_mutex_lock(&engine_list_mutex);
    engine->link = engine_list_head;
    engine_list_head = engine;
    pthread_mutex_unlock(&engine_list_mutex);
}

// This is the routine calle dby the work queue servers to perform operations in parallel

void engine_routine(void *arg)
{
    engine_t* engine;
    power_t* power = (power_t*)arg;
    int result, count;
    int status;
    engine = (engine_t* )pthread_getspecific(engine_key);
    if (engine == NULL)
    {
        engine = (engine_t*)malloc(sizeof(engine_t));
        status = pthread_setspecific(engine_key, (void*)engine);
        if (status != 0)
            err_abort(status, "Set tsd");
        engine->thread_id = pthread_self();
        engine->calls = 1;
    }
    else
        engine->calls++;
    result = 1;
    printf("eingen computing %d ^ %d\n", power->value, power->power);
    for (count = 1; count <= power->power; count++)
        result *= power->value;
    free(arg);
}

// thread start routine that issues work queue request
void *thread_routine(void *arg)
{
    power_t* element;
    int count;
    unsigned int seed = (unsigned int)time(NULL);
    int status;
    // loop making requests
    for (count = 0; count < ITERATIONS; count++)
    {
        element = (power_t*)malloc(sizeof(power_t));
        if (element == NULL)
            err_abort(1, "allocate element");
        element->value = rand_r(&seed) %20;
        element->power = rand_r(&seed) % 7;
        printf("request %d^%d\n", element->value, element->power);
        status = workq_add(&workq, (void*)element);
        if (status != 0)           
            err_abort(status, "add to work queue");
        sleep(rand_r(&seed) % 5);
    }
    return NULL;
}
int main(int argc, char* argv[])
{
    pthread_t thread_id;
    engine_t *engine;
    int count, calls = 0;
    int status;
    status = pthread_key_create(&engine_key, destructor);
    if (status != 0)           
        err_abort(status, "create key");
    status = workq_init(&workq, 4, engine_routine);
    if (status != 0)           
        err_abort(status, "init work queue");
    status = pthread_create(&thread_id, NULL, thread_routine, NULL);
    if (status != 0)           
        err_abort(status, "create thread");
    (void)thread_routine(NULL);  // main thread also call this function
    status = pthread_join(thread_id, NULL);
    if (status != 0)           
        err_abort(status, "join thread");
    status = workq_destroy(&workq);
    if (status != 0)           
        err_abort(status, "destroy work queue");
    // by now all of engine_t structure have been placed on the list by the engine thread destructors, so we can count and summarise them
    engine  = engine_list_head;
    while (engine != NULL)
    {
        count++;
        calls += engine->calls;
        printf("engine %d: %d calls\n", count, engine->calls);
        engine = engine->link;
    }
    printf("%d engine threads processed %d calls\n", count, calls);
    return 0;
}