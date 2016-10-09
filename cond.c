#include <pthread.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct my_struct_tag {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int value; // access protected by mutex
} my_struct_t;

my_struct_t data = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 0};

int hibernation = 1; // default to 1 second
// Thread start route. It will set the main thread's predicate and signal the condition variable
void* wait_thread(void* arg)
{
	int status;
	sleep(hibernation);
	status = pthread_mutex_lock(&data.mutex);
	if (status != 0)
		printf("lock failed");
	data.value = 1; // set predicate;
	status = pthread_cond_signal(&data.cond);
	if (status != 0)
		printf("signal failed");
	status = pthread_mutex_unlock(&data.mutex);
	if (status != 0)
		printf("unlock wait falied");;
	return NULL;
		
};

int main (int argc, char * argv[])
{
	int status;
	pthread_t wait_thread_id;
	struct timespec timeout;
	if (argc > 1)
		hibernation = atoi(argv[1]);
	status = pthread_create(&wait_thread_id, NULL, wait_thread, NULL);
	if (status != 0)
		printf("create thread failed");
	// wait on the condition variable for 2 seondds or until signaled by the wait thread
	timeout.tv_sec = time(NULL) + 2;
	timeout.tv_nsec = 0;
	status = pthread_mutex_lock(&data.mutex);
	if (status !=0)
		printf("lock mutex failed main");
	while (data.value == 0)
	{
		status = pthread_cond_timedwait(&data.cond, &data.mutex, &timeout);
		if (status == ETIMEDOUT)
		{
			printf("conditoin wait timed out\n");
			break;
		}
		else if (status != 0)
			printf("wait failed");
	}
		if (data.value != 0)
			printf("condition was signaled.\n");
			status = pthread_mutex_unlock(&data.mutex);
		if (status !=0)
			printf("unlock main fail");
}
			
			