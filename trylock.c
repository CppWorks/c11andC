#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SPIN 210000000
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long counter;
time_t end_time;

void* counter_thread(void *arg)
{
	int status;
	int spin;
	while (time(NULL) < end_time)
	{
		status = pthread_mutex_lock(&mutex);
		if (status != 0)
		{
			printf("lock failed");
		}
		for (spin = 0; spin < SPIN; spin++)
		{
			counter++;
		}
		status = pthread_mutex_unlock(&mutex);
		if (status != 0)
		{
			printf("unlock failed");
		}
		sleep(1);
	}
	printf("Counter is %#lx\n", counter);
	return NULL;
}

void* monitor_thread(void* arg)
{
	int status;	int misses  = 0;
	while (time(NULL) < end_time)
	{
		sleep(3);
		status = pthread_mutex_trylock(&mutex);
		if (status != EBUSY)
		{
			if (status != 0)
			{
				printf("lock failed in monitor");
			}				
			printf("Counter is %ld\n", counter/SPIN);
			status = pthread_mutex_unlock(&mutex);
			if (status != 0)
			{
				printf("monitorf unlock failed");
			}
			
		}
		else
		{
			misses++;
		}		   
	}
	printf("Monitor htread missed update %d times.\n", misses);
	return NULL;
}


int main(int argc, char *argv[])
{
	int status;
	pthread_t counter_thread_id;
	pthread_t monitor_thread_id;
	end_time = time(NULL) + 60; // run for one minute;
	status = pthread_create(&counter_thread_id, NULL, counter_thread, NULL);
	if (status != 0)
		printf("counter thread failed to create");
	status = pthread_create(&monitor_thread_id, NULL, monitor_thread, NULL);
	if (status != 0)
		printf("monitor thread create faild");
	status = pthread_join(counter_thread_id, NULL);
	if (status != 0)
		printf("error in join counter thread");
	status = pthread_join(monitor_thread_id, NULL);
	if (status != 0)
		printf("error in join monitor thread");
	return 0;
}









































