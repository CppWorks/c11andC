#include <pthread.h>

// Structure descriing a barrier
typedef struct barrier_tag
{
    pthread_mutex_t mutex; // control asscess to barrier
    pthread_cond_t cv; // wait for bariier
    int valid; // set when valid
    int threshold; //number of threads required
    int counter; // current number of threads
    unsigned long cycle; //count cycles
    int updatingIncrement;
} barrier_t;
#define BARRIER_VALID 0xdbcafe

// Support static initialization of barriers

#define BARRIER_INITIALIZER(cnt) \
                     {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, \
      BARRIER_VALID, cnt, cnt, 0}
//#define barrier function
extern int barrier_int(barrier_t *barrier, int count);
extern int barrier_destroy(barrier_t *barrier);
extern int barrier_wait(barrier_t* barrier);
// gcc -Wno-implicit alarm.c -o hello -lpthread

