/******************************************************************************
* FILE: hello.c
* DESCRIPTION:
*   A "hello world" Pthreads program.  Demonstrates thread creation and
*   termination.
* AUTHOR: Blaise Barney
* LAST REVISED: 08/09/11
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS	5

// http://www.c4learn.com/c-programming/c-void-pointers/

void *PrintHello(void *threadid)
{
    long tid;
    tid = (long)threadid;
    printf("Hello World! It's me, thread #%ld!\n", tid);
    pthread_exit(NULL);
}

void *thread(void *vargp)
{
    pthread_exit((void*)42);
}

int main(int argc, char *argv[])
{

    int i = 30;
    pthread_t tid;
    pthread_create(&tid, NULL, thread, NULL);
    pthread_join(tid, (void**)&i);
    printf("%d\n",i);

    void* ptr = &i;
    void** xptr = &ptr;
    void** xptr2 = (void**)ptr;
    printf("xptr address is %p\n", *xptr);
    printf("xptr2 address is %d\p", *((int*)*xptr));
    printf("ptr address is %p\n", ptr);
    return 0;
}