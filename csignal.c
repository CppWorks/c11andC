#include <pthread.h>
#include <signal.h>
#include "errors.h"

void INThandler(int sig)
{
    char c;
    if (signal(SIGINT, SIG_IGN) != SIG_IGN)
    {
        printf("cant ignore it\n");
    }
    else
    {
        printf("we can ignore it\n");
    }
  //  signal(sig, SIG_IGN); // ignore the signal first
    printf("ouch did you hit ctrl-c?\n, do you rally want to quit [y/n]?");
    c = getchar();
    if ( c== 'Y' || c== 'y')
        exit(0);
    else
        signal(SIGINT, INThandler); //reinstall
}

int main (int argc, char *argv[])
{

    if (signal(SIGINT, SIG_IGN) != SIG_IGN)
    {
       printf("cant ignore it\n");
        signal(SIGINT, INThandler);
    }

    // A long long wait so that we can easily issue a signal to this process
    while(1) 
        pause();
    return 0;
  //pthread_t threads[THREAD_COUNT];
  //pthread_attr_t detach;
  //int status;
  //void* result;
  //int i;
  //status = pthread_attr_init(&detach);
  //if(status != 0 )
  //    err_abort(status, "init attr");
  // status = pthread_attr_setdetachstate(&detach, PTHREAD_CREATE_DETACHED);
  // if(status != 0 )
  //     err_abort(status, "set detach");
  // for (i=0; i < THREAD_COUNT; i++)
  // {
  //     status = pthread_create(&threads[i], &detach, thread_routine, (void*)i);
  //     if(status != 0 )
  //         err_abort(status, "create threaed");
  // }
  // sleep(2);


  // for (i = 0; i < THREAD_COUNT/2; i++)
  // {
  //     printf("Suspending thread %d.\n", i);
  //     status = thd_suspend(threads[i]);
  //     if (status != 0)
  //         err_abort(status, "suspend threaed");
  // }

  // printf("sleeping.,...\n");
  // sleep(2);
  // for (i = 0 ; i < THREAD_COUNT/2; i++)
  // {
  //     printf("continuing thread %d \n",i);
  //     status = thd_continue(threads[i]);
  //     if (status != 0)
  //         err_abort(status, "continus threaed"); 
  // }

  // for (i = THREAD_COUNT/2; i < THREAD_COUNT; i++)
  // {
  //     printf("Suspending thread %d.\n", i);
  //     status = thd_suspend(threads[i]);
  //     if (status != 0)
  //         err_abort(status, "suspend threaed");
  // }

  // printf("sleeping.,...\n");
  // sleep(2);
  // for (i = THREAD_COUNT/2 ; i < THREAD_COUNT; i++)
  // {
  //     printf("continue thread %d \n",i);
  //     status = thd_continue(threads[i]);
  //     if (status != 0)
  //         err_abort(status, "continus threaed"); 
  // }
   pthread_exit(NULL);
}
// gcc -Wno-implicit alarm.c -o hello -lpthread

