#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
void SIGINT_handler(int);
void SIGQUIT_handler(int);
int ShmID;
pid_t *ShmPTR;

void SIGINT_handler(int sig)
{
    signal(sig, SIG_IGN);
    printf("From SIGINT: got a Ctrl-C signal %d\n", sig);
    signal(sig, SIGINT_handler);
}
void SIGQUIT_handler(int sig)
{
    signal(sig, SIG_IGN);
    printf("From SIGQUIT: got a Ctrl-\\ signal %d\n", sig);
    printf("From SIGQUIT: quitting\n");
    shmdt(ShmPTR);
    shmctl(ShmID, IPC_RMID, NULL);
    exit(0);
}
void main(void)
{
    int i;
    pid_t pid = getpid();
    key_t MyKey;
    signal(SIGINT, SIGINT_handler);
    signal(SIGQUIT, SIGQUIT_handler);
    MyKey = ftok("./", 'a');
    ShmID = shmget(MyKey, sizeof(pid_t), IPC_CREAT|0666);
    ShmPTR = (pid_t *) shmat(ShmID, NULL, 0);
    *ShmPTR = pid;
    for (i = 0; ; i++) {
        printf("From process %d: %d\n", pid, i);
        sleep(1);
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

