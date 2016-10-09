#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include "errors.h"
#include <sys/wait.h>
#include <sys/types.h>

// this routine writes a propt to stdout (passed as the thead's arg), and reads a response. All other I/O to std in and stdout is prevented by the file locks until both prompt and fgets are complete

void *prompt_routine(void*arg)
{
    char* prompt = (char*)arg;
    char* string;
    int len;
    string = (char*)malloc(128);
    if(string == NULL)
        err_abort(0, "alloc string");
    flockfile(stdin);
    flockfile(stdout);
    printf(prompt);
    if (fgets(string, 128, stdin) == NULL)
        string[0] = '\0';
    else
    {
        len = strlen(string);
        if (len > 0 && string[len-1] == '\n')
            string[len-1] = '\0';

    }
    funlockfile(stdout);
    funlockfile(stdin);
    printf("returning from %s \n",prompt);
    return (void*)string;

}


int main (int argc, char *argv[])
{
  pthread_t thread1, thread2, thread3;
  void* string;
  int status;
  status = pthread_create(&thread1, NULL, prompt_routine, "Thread 1>");
  if(status != 0 )
      err_abort(status, "create thread");
  status = pthread_create(&thread2, NULL, prompt_routine, "Thread 2>");
  if(status != 0 )
      err_abort(status, "create thread");
  status = pthread_create(&thread3, NULL, prompt_routine, "Thread 3>");
  if(status != 0 )
      err_abort(status, "create thread");
   printf("waiting 3\n");
  status = pthread_join(thread3, &string);
  if(status != 0 )
      err_abort(status, "join thread");
  printf("Thread 3:  \"%s\"\n", (char*)string);
  free(string);

  printf("waiting 1\n");
  status = pthread_join(thread1, &string);
  if(status != 0 )
      err_abort(status, "join thread");
  printf("Thread 1:  \"%s\"\n", (char*)string);
  free(string);
  printf("waiting 2\n");
  status = pthread_join(thread2, &string);
  if(status != 0 )
      err_abort(status, "join thread");
  printf("Thread 2:  \"%s\"\n", (char*)string);
  free(string);

  return 0;

}
// gcc -Wno-implicit alarm.c -o hello -lpthread

