#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include "errors.h"
#include <sys/wait.h>
#include <sys/types.h>

pid_t self_pid; // pid of current process

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// this rouite will be called priori to executing the fork within the parent process

void fork_prepare(void)
{
    int status;
    // lock the mutex in the parent befoere creating the child
    // to ensure that no other thread can lock it or change any associated shared state) until after the fork completes.
    printf("im calling prepare\n");
    status = pthread_mutex_lock(&mutex);
    if (status != 0)
        err_abort(status, "lock in prepare handler");

}


// this routine will be called after executing the fork, within the parent proess
  
void fork_parent(void)
{
    printf("im calling fork parent\n");
    int status;
    // unlock the mutex in the parent after the child has been created
    status = pthread_mutex_unlock(&mutex);
    if (status != 0)
        err_abort(status, "unlock  in parent handler");
}


// this routine will be called after executing the fork, with in the child process
void fork_child(void)
{
   
    int status;
    // update the file scope self_pid within the child processs and unlock the mutex;
    self_pid = getpid();
     printf("im calling fork child pid %d\n", self_pid);
     // so mutex is actually copied...if I dont unlock, I will hang in the child process.
  status = pthread_mutex_unlock(&mutex);
  //if (status != 0)
    //  err_abort(status, "unlock  in childer handler");
}

// so there are so many things happen at the fork call
// prepare lock the mutex and copy it to the child process
// fork chile will unlock the copied mutex so it will continue
// fork parent will do same so the parent process can continue.
// thread start routine , which will fork a new child proc
void* thread_routine(void* arg)
{
    pid_t child_pid;
    int status;
    printf("im going to call fork\n");
    child_pid = fork();
    if (child_pid == (pid_t) - 1)
        err_abort(child_pid, "fork");
    // lock the mutex -- without the atfork handlers, the mutex will remain locked in the child process and this lock attempt will hang(or fail with edeadlk) in the child
    printf("back to normal\n");
    status = pthread_mutex_lock(&mutex);
    if (status != 0)
        err_abort(status, "lock  in child");
     printf("after me fork: %d (%d)\n", child_pid, self_pid);
    status = pthread_mutex_unlock(&mutex);
    if (status != 0)
        err_abort(status, "unlock  in child");

    if (child_pid != 0)
    {
        if ((pid_t) - 1 == waitpid(child_pid, (int*)0, 0))
            err_abort(1, "wait for child");
    }
    return NULL;

}
int main (int argc, char *argv[])
{
    pthread_t fork_thread;
    int atfork_flag = 1;
    int status;
    if (argc > 1)
        atfork_flag = atoi(argv[1]);
    if (atfork_flag)
    {
        // without fork handler, child will hang coz mutex is locked when copied (default)............fork handler utilise mutex so it will wait until mutex is free, then lock it and copy it over.
        // wiout fork handler.....the parent will be ok..but the child will hang.
        status = pthread_atfork(fork_prepare, fork_parent, fork_child);
        if (status != 0)
            err_abort(status, "register fork handler");
    }
    self_pid = getpid();
    status = pthread_mutex_lock(&mutex);
    if (status != 0)
        err_abort(status, "lock mutex");
    // create a thread while the mutex is locked, it will fork a process, which (without atfork handlers) will run with the mutex locked
    status = pthread_create(&fork_thread, NULL, thread_routine, NULL);
    if (status != 0)
        err_abort(status, "CREATE THREAD");
    sleep(4);

    // This unlock will unlock the fork call (prepare handler).
    status = pthread_mutex_unlock(&mutex);
    if (status != 0)
        err_abort(status, "unlock mutex");
    status = pthread_join(fork_thread, NULL);
    if (status != 0)
        err_abort(status, "join thread");
    return 0;
}
// gcc -Wno-implicit alarm.c -o hello -lpthread

