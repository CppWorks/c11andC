#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include "errors.h"
#include <sys/wait.h>
#include <sys/types.h>

// This function writes a string (the function's arg) to stdout, by locking the file stream and using putchar_unlocked to write each character individually
// this is the correct way
void* lock_routine(void* arg)
{
    printf("in lock routine %s\n", (char*)arg);
    char *pointer;
    flockfile(stdout);
    for (pointer = arg; *pointer != '\0'; pointer++)
    {
        putchar_unlocked(*pointer);
      //  sleep(1);
    }
     sleep(1);
    funlockfile(stdout);
    return NULL;
}

// THIS FUNCtion writes a string to stdout, by using putchar to write each character individually. Although the internal locking of putchar prevents filestream corruption
// the writes of various threads may be interleaved
void *unlock_routine(void* arg)
{
    char* pointer;
    for (pointer = arg; *pointer != '\0'; pointer++)
    {
        putchar(*pointer);
        sleep(1);
    }
}

int main (int argc, char *argv[])
{
  pthread_t thread1, thread2, thread3;
  int flock_flag = 1;
  void *(*thread_func)(void*);
  void* string;
  int status;

  if (argc > 1)
      flock_flag = atoi(argv[1]);
  if (flock_flag)
      thread_func = lock_routine;
  else
      thread_func = unlock_routine;

  status = pthread_create(&thread1, NULL, thread_func, "This is thread 1>\n");
  if(status != 0 )
      err_abort(status, "create thread");
  status = pthread_create(&thread2, NULL, thread_func, "this is Thread 2>\n");
  if(status != 0 )
      err_abort(status, "create thread");
  status = pthread_create(&thread3, NULL, thread_func, "this Thread 3>\n");
  if(status != 0 )
      err_abort(status, "create thread");


  if(status != 0 )
      err_abort(status, "join thread");

  pthread_exit(NULL);
  return 0;

}
// gcc -Wno-implicit alarm.c -o hello -lpthread

