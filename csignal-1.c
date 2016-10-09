#include <pthread.h>
#include <signal.h>
#include "errors.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#define MAX_i 10000
#define MAX_j 20000
#define MAX_SECOND (2)
void INThandler(int);
void ALARMhandler(int);
int SECOND, i, j;


void INThandler(int sig)
{
    char c;
    signal(SIGINT, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
    printf("INT handler: i = %d and j = %d\n", i, j);
    printf("INT handler: want to quit [y/n]?");
    c = tolower(getchar());
    if (c == 'y') {
        printf("INT handler: done"); exit(0);
    }
    signal(SIGINT, INThandler);
    signal(SIGALRM, ALARMhandler);
    alarm(SECOND);
} 

void ALARMhandler(int sig)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
    printf("ALARM handler: alarm signal received\n");
    printf("ALARM handler: i = %d and j = %d\n", i, j);
    alarm(SECOND);
    signal(SIGINT, INThandler);
    signal(SIGALRM, ALARMhandler);
}


void main(int argc, char *argv[])
{
    long sum;
    SECOND = abs(atoi(argv[1]));
    signal(SIGINT, INThandler);
    signal(SIGALRM, ALARMhandler);
    alarm(SECOND);
    for (i = 1; i <= MAX_i; i++) {
        sum = 0;
        for (j = 1; j <= MAX_j; j++)
            sum += j;
    }
        while(1) 
           pause();
    printf("Computation is  done");
}
//int main (int argc, char *argv[])
//{
//
//    if (signal(SIGINT, SIG_IGN) != SIG_IGN)
//    {
//       printf("cant ignore it\n");
//        signal(SIGINT, INThandler);
//    }
//
//    // A long long wait so that we can easily issue a signal to this process
//    while(1) 
//        pause();
//    return 0;
//  //pthread_t threads[THREAD_COUNT];
//  //pthread_attr_t detach;
//  //int status;
//  //void* result;
//  //int i;
//  //status = pthread_attr_init(&detach);
//  //if(status != 0 )
//  //    err_abort(status, "init attr");
//  // status = pthread_attr_setdetachstate(&detach, PTHREAD_CREATE_DETACHED);
//  // if(status != 0 )
//  //     err_abort(status, "set detach");
//  // for (i=0; i < THREAD_COUNT; i++)
//  // {
//  //     status = pthread_create(&threads[i], &detach, thread_routine, (void*)i);
//  //     if(status != 0 )
//  //         err_abort(status, "create threaed");
//  // }
//  // sleep(2);
//
//
//  // for (i = 0; i < THREAD_COUNT/2; i++)
//  // {
//  //     printf("Suspending thread %d.\n", i);
//  //     status = thd_suspend(threads[i]);
//  //     if (status != 0)
//  //         err_abort(status, "suspend threaed");
//  // }
//
//  // printf("sleeping.,...\n");
//  // sleep(2);
//  // for (i = 0 ; i < THREAD_COUNT/2; i++)
//  // {
//  //     printf("continuing thread %d \n",i);
//  //     status = thd_continue(threads[i]);
//  //     if (status != 0)
//  //         err_abort(status, "continus threaed"); 
//  // }
//
//  // for (i = THREAD_COUNT/2; i < THREAD_COUNT; i++)
//  // {
//  //     printf("Suspending thread %d.\n", i);
//  //     status = thd_suspend(threads[i]);
//  //     if (status != 0)
//  //         err_abort(status, "suspend threaed");
//  // }
//
//  // printf("sleeping.,...\n");
//  // sleep(2);
//  // for (i = THREAD_COUNT/2 ; i < THREAD_COUNT; i++)
//  // {
//  //     printf("continue thread %d \n",i);
//  //     status = thd_continue(threads[i]);
//  //     if (status != 0)
//  //         err_abort(status, "continus threaed"); 
//  // }
//   pthread_exit(NULL);
//}
// gcc -Wno-implicit alarm.c -o hello -lpthread

