#include <pthread.h>
#include <math.h>
#include "errors.h"

// define client_threads
#define CLIENT_THREADS 12
#define REQ_READ 1
// Read with prompt;
#define REQ_WRITE 2
// Write;
#define REQ_QUIT 3
// quit server;

// Internal to sever pacgage -- one for each request
typedef struct request_tag
{
    struct request_tag *next;
    // link to next
    int operation; // function code
    int synchronous; // nonzero if synchronous
    int done_flag; // predicate for wait;
    pthread_cond_t done; // wait for completion;
    char  prompt[32]; // prompt string for reads;
    char text[128]; // read/write text;
} request_t;

// Static context for the server
typedef struct tty_server_tag
{
    request_t *first;
    request_t *last;
    int running;
    pthread_mutex_t mutex;
    pthread_cond_t request;
} tty_server_t;;

tty_server_t tty_sever = { NULL, NULL, 0, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};

// MAIN PROGRAM DATA;
int client_threads;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clients_done = PTHREAD_COND_INITIALIZER;


// Ther server start route. It waits for a request to appear. 
// in tty_server.requests using the request condition variable.
// It processes requests in FIFO order. If a request is marked synchronous (synchronous !=0) the server weill set done_flat
// and signal the requests's conditio nvariable.
// The client is responsible for freeing the request. If the request is not synchrous, the server will free the request on completion

void *tty_server_routine(void *arg)
{
    static pthread_mutex_t prompt_mutex = PTHREAD_MUTEX_INITIALIZER;
    request_t *request;
    int operation, len;
    int status;
    while (1)
    {
        status = pthread_mutex_lock(&tty_sever.mutex);
        if (status != 0)
            err_abort(status, "lock server mutex");
        // wait for data
        while (tty_sever.first == NULL)
        {
            status = pthread_cond_wait(&tty_sever.request, &tty_sever.mutex);
            if (status != 0)
                err_abort(status, "wait for request");
        }
        request = tty_sever.first;
        tty_sever.first = request->next;
        if (tty_sever.first == NULL)
            tty_sever.last = NULL;
        status = pthread_mutex_unlock(&tty_sever.mutex);
        if (status != 0)
            err_abort(status, "unlock server mutex");
        // process data
        operation = request->operation;
        switch (operation){
        case REQ_QUIT:
            break;
        case REQ_READ:
            if (strlen(request->prompt) > 0)
                printf(request->prompt);
            if (fgets(request->text, 128, stdin) == NULL)
                request->text[0] = '\0';
            // because fgets returns the newline, and wo dont wat it, wo look for it and turn it into a null truncatn gthe input if found.
            // it should be the last char if it is thre
            len = strlen(request->text);
            if (len > 0 && request->text[len-1] == '\n')
                request->text[len-1] = '\0';
            break;
        case REQ_WRITE:
            puts(request->text);
            break;
        default:
            break;
        }
        if (request->synchronous)
        {
            status = pthread_mutex_lock(&tty_sever.mutex);
            if (status != 0)
                err_abort(status, "lock server mutex");
            request->done_flag = 1;
            status = pthread_cond_signal(&request->done);
            if (status != 0)
                err_abort(status, "signal server condition");
            status = pthread_mutex_unlock(&tty_sever.mutex);
            if (status != 0)
                err_abort(status, "unlock server mutex");
        }
        else
        {
            free(request);

        }
        if (operation == REQ_QUIT)
            break;
    }
    return NULL;
}



// request an opertion
void tty_server_request(int operation, int sync, const char* prompt, char *string)
{
    request_t *request;
    int status;
    // lock the server mutex to do one time initialisation....
    status = pthread_mutex_lock(&tty_sever.mutex);
    if (status != 0)
        err_abort(status, "lock server mutex");
    if (!tty_sever.running)
    {
        pthread_t thread;
        pthread_attr_t deatched_attr;
        status = pthread_attr_init(&deatched_attr);
        if (status != 0)
            err_abort(status, "initi thread attr object");
        status = pthread_attr_setdetachstate(&deatched_attr, PTHREAD_CREATE_DETACHED);
        if (status != 0)
            err_abort(status, "Set deatch state");
        tty_sever.running = 1;
        status = pthread_create(&thread, &deatched_attr, tty_server_routine, NULL);
        if (status != 0)
            err_abort(status, "Creater server");
        // ignore an error in destroying the attribute object. It is unlikely to fail...thre is nothing useful we can do about it, and its not worth aborting hte program.
        pthread_attr_destroy(&deatched_attr);
    }

    // create and initialize a request structure;
    request = (request_t *)malloc(sizeof(request_t));
    if (request == NULL)
        err_abort(1, "ALLOCATE REQUEST");
    request->next = NULL;
    request->operation = operation;
    request->synchronous = sync;
    if (sync)
    {

        request->done_flag = 0;
        status = pthread_cond_init(&request->done, NULL);
        if (status != 0)
            err_abort(status, "Init request condition");
    }
    if (prompt != NULL)
        strncpy(request->prompt, prompt, 32);
    else
        request->prompt[0] = '\0';
    if (operation == REQ_WRITE && string != NULL)
        strncpy(request->text, string, 128);
    else
        request->text[0] = '\0';
    // add the request to the queue , maintainig the first and last pointers
    if (tty_sever.first == NULL)
    {
        tty_sever.first = request;
        tty_sever.last = request;
    }
    else
    {
        (tty_sever.last)->next = request;
        tty_sever.last = request;
    }
    // tell the server that a request is avalible
    status = pthread_cond_signal(&tty_sever.request);
    if (status != 0)
        err_abort(status, "wake server");
    // if request is synchrous, then wait for a reply
    if (sync)
    {
        while (!request->done_flag)
        {
            status = pthread_cond_wait(&request->done, &tty_sever.mutex);
            if (status != 0)
                err_abort(status, "wait for sync request");
        }
        if (operation == REQ_READ)
        {
            if (strlen(request->text) > 0)
                strcpy(string, request->text);
            else
                string[0] = '\0';
        }
        status = pthread_cond_destroy(&request->done);
        if (status != 0)
            err_abort(status, "destroy requestion condition");
        free(request);
    }
    status = pthread_mutex_unlock(&tty_sever.mutex);
    if (status != 0)
        err_abort(status, "unlock mutex");

}

// client routine---multiple copies will request server
void *client_routine(void* arg)
{
    int my_number = (int)arg, loops;
    char prompt[32];
    char string[128], formatted[128];
    int status;
    sprintf(prompt, "Holy Client %d> ", my_number);
    while (1)
    {
      //  printf("promp is %s \n", prompt);
        tty_server_request(REQ_READ, 1, prompt, string);
     //   printf("read result is %s\n", string);
        if (strlen(string) == 0)
            break;
        for (loops = 0; loops < 4; loops++)
        {
            sprintf(formatted, "(%d#%d) %s", my_number, loops, string);
            tty_server_request(REQ_WRITE, 0, NULL, formatted);
            sleep(1);
        }
    }
    status = pthread_mutex_lock(&client_mutex);
    if (status != 0)
        err_abort(status, "lock client mutex");
    client_threads--;
    if (client_threads <= 0)
    {
        status = pthread_cond_signal(&clients_done);
        if (status != 0)
            err_abort(status, "signal client done");
    }
    status = pthread_mutex_unlock(&client_mutex);
    if (status != 0)
        err_abort(status, "unlock client mutex");
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t thread;
    int count;
    int status;
    // create client threads clients
    client_threads = CLIENT_THREADS;
    for (count = 0; count < client_threads; count++)
    {
        status = pthread_create(&thread, NULL, client_routine, (void*)count);
        if (status != 0)
            err_abort(status, "create client thread");
    }
    status = pthread_mutex_lock(&client_mutex);
    if (status != 0)
        err_abort(status, "lock cclient mutex");
    while (client_threads > 0)
    {
        status = pthread_cond_wait(&clients_done, &client_mutex);
        if (status != 0)
            err_abort(status, "wait for client threads to finish");
    }
    status = pthread_mutex_unlock(&client_mutex);
    if (status != 0)
        err_abort(status, "unlock for client threads mutex");
    printf("all clients done\n");
    tty_server_request(REQ_QUIT, 1, NULL, NULL);
    return 0;

}

















