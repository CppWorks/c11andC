#include <pthread.h>

// Structure to keep track of work queue requests
typedef struct workq_ele_tag
{
    struct workq_ele_tag *next;
    void *data;
} workq_ele_t;

// structure describing a work queue
typedef struct workq_tag
{
    pthread_mutex_t mutex; // control accesss to queue
    pthread_cond_t cv; // wait for work
    pthread_attr_t attr; // create detached threads
    workq_ele_t *first, *last; // work queue
    int valid; // valid
    int quit; // workq should quit
    int parallelism; // max threads;
    int counter ;//current threas;
    int idle; // number of idle threds
    void (*engine)(void *arg); //user egngine....this is a function pointer.
} workq_t;
//// define work queue function
extern int workq_init(workq_t *wq, 
    int threads, //max threads
    void (*engine)(void *)); // engine routine
extern int workq_destroy(workq_t *wq);
extern int workq_add(workq_t *wq, void *element);

#define WORKQ_VALID 0xdec1992
// gcc -Wno-implicit alarm.c -o hello -lpthread

