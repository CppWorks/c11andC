#include <pthread.h>
#include <limits.h>
#include "errors.h"

#define  NUM_THREADS 3

void *PrintHello(void *threadid)  {
    long tid = (long)threadid;
    printf("Hello World! It's me, thread #%ld!\n", tid);
    return threadid;
}

int main (int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int rc;
    intptr_t t;
    // create all the threads
    for(t=0; t<NUM_THREADS; t++){
        printf("In main: creating thread %ld\n", (long) t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
        if (rc) { fprintf(stderr, "failed to create thread #%ld - %s\n",
            (long)t, strerror(rc));
        exit(EXIT_FAILURE);
        };
    }
    pthread_yield(); // useful to give other threads more chance to run
    // join all the threads
    for(t=0; t<NUM_THREADS; t++){
        printf("In main: joining thread #%ld\n", (long) t);
        rc = pthread_join(&threads[t], NULL);
        if (rc) { fprintf(stderr, "failed to join thread #%ld - %s\n",
            (long)t, strerror(rc));
        exit(EXIT_FAILURE);
        }
    }
}
// gcc -Wno-implicit alarm.c -o hello -lpthread
