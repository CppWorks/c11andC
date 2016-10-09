#include <pthread.h>
#include "errors.h"

pthread_cond_t cond;

int main (int argc, char *argv[])
{
    pthread_condattr_t cond_attr;;
    int status;
    status = pthread_condattr_init(&cond_attr);
    if (status != 0)
        err_abort(status, "Create attr");
#ifdef _POSIX_THREAD_PROCESS_SHARED
    printf("wow");
    status = pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
    if (status != 0)
        err_abort(status, "set pshared");
#endif
    status = pthread_cond_init(&cond, &cond_attr);
    if (status != 0)
        err_abort(status, "init cond");

}
// gcc -Wno-implicit alarm.c -o hello -lpthread
