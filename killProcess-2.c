#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void main(void)
{
    pid_t pid, *ShmPTR;
    key_t MyKey;
    int ShmID;
    char c;
    MyKey = ftok("./", 'a');
    ShmID = shmget(MyKey, sizeof(pid_t), 0666);  // return the identifier for the process
    ShmPTR = (pid_t *) shmat(ShmID, NULL, 0); //  shmat() attaches the System V shared memory segment identified by shmid to the address space of the calling process.
    pid = *ShmPTR;
    shmdt(ShmPTR); /* see next page */  // attach the address to this calling process...
    while (1) {
        printf("(i for interrupt or k for kill)? ");
        c = getchar();
        if (c == 'i' || c == 'I') {
            kill(pid, SIGINT);
            printf("A SIGKILL signal has been sent\n");
        }
        else if (c == 'k' || c == 'K') {
            printf("About to sent a SIGQUIT signal\n");
            kill(pid, SIGQUIT);
            exit(0);
        }
        else
            printf("Wrong keypress (%c). Try again!\n", c);
    }
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

