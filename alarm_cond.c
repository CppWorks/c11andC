#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>


typedef struct alarm_tag {
	struct alarm_tag *link;
	int seconds;
	time_t time;
	char message[64];
} alarm_t;


pthread_mutex_t alarm_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t alarm_cond = PTHREAD_COND_INITIALIZER;


alarm_t* alarm_list = NULL;
time_t current_alarm = 0;

void alarm_insert(alarm_t* alarm)
{
	int status;
	alarm_t **last, *next;
	last = &alarm_list;
	next = *last;
	while (next != NULL)
	{
		if (next->time >= alarm->time)
		{
			alarm->link = next;
			*last = alarm;
			break;
		}
		last = &next->link;
		next = next->link;
	}
	if (next == NULL)
	{
		*last = alarm;
		alarm->link = NULL;
	}
	printf("[list:");
	for (next = alarm_list; next != NULL; next = next->link)
		printf("%d(%d)[\"%s\"\"]", (int)next->time, (int)(next->time - time(NULL)), next->message);
	printf("]\n");
	
	// wake the alarm thread if it is not busy (that is if current_alarm is 0 , signifying that it is waiting for work), or the new alarm comes before the one on which alarm thread is waing
	if (current_alarm == 0 || alarm-> time < current_alarm)
	{
		//rest current_alarm time
		current_alarm = alarm->time;
		status = pthread_cond_signal(&alarm_cond);
		if (status != 0)
			printf("signal cond fail");
	}

}

void * alarm_thread(void* arg)
{
	alarm_t *alarm;
	struct timespec cond_time;
	time_t now;
	int status, expired;
	status = pthread_mutex_lock(&alarm_mutex);
	if (status != 0 )
		printf("alram thread lock fail");
	while (1)
	{
		// if alarm list is empty, wait utial an alarm is added
		// setting current alarm to 0 informs the insert route that the thread is not busy
		current_alarm = 0;
		while (alarm_list == NULL)
		{
			printf("no nask\n");
			status = pthread_cond_wait(&alarm_cond, &alarm_mutex);
			if (status != 0)
				printf("wait on failed");
			printf("have work now\n");
		}
		alarm = alarm_list;
		alarm_list = alarm->link;
		now = time(NULL);
		expired = 0;
		if (alarm->time > now)
		{
			printf("[waiting:%d(%d)\"%s\"]\n", (int)alarm->time, (int)(alarm->time - time(NULL)), alarm->message);
	         if (alarm_list ==NULL)
				 printf("no alam in the list\n");
			cond_time.tv_sec = alarm->time;
			cond_time.tv_nsec = 0;
			current_alarm = alarm->time;
			while (current_alarm == alarm->time)
			{
				status = pthread_cond_timedwait(&alarm_cond, &alarm_mutex, &cond_time);
				if (status == ETIMEDOUT){
					expired  = 1;
					break;
				}
				if (status != 0)
					printf("cond timeoutfail");
			}
			if (!expired)
                // the mutex is still locked due to user input
				alarm_insert(alarm);
		
		} else
			expired = 1;
		if (expired)
		{
			printf("(%d) %s\n", alarm->seconds, alarm->message);
			free(alarm);
		}
	}
}

int main (int argc, char *argv[])
{
		int  status;
	char line[128];
	alarm_t *alarm, **last,  *next;
	
	
	pthread_t thread;
	
	status = pthread_create(&thread, NULL, alarm_thread, NULL);
    while (1)
	{
		printf("alarm> ");
		if (fgets(line, sizeof(line), stdin) == NULL) 
		{
			printf("oh no");
			exit(0);
		}
		//printf("%s", line);
		if (strlen(line) <=1) continue;
		alarm = (alarm_t*) malloc(sizeof(alarm_t));
		
		if (alarm == NULL)
			printf("fail construct alarm");
		alarm->seconds = 0;
		if (sscanf(line, "%d %64[^\n]", &(alarm->seconds), alarm->message) < 2)
		{
			fprintf(stderr, "bad command\n");
			free (alarm);
	
		}
		else
		{
			status = pthread_mutex_lock(&alarm_mutex);
			if (status != 0)
				printf("error lock mutex");
			alarm->time = time(NULL) + alarm->seconds;
			// insert the new aralm into the list
			// sorted by expiration time
			alarm_insert(alarm);
			status = pthread_mutex_unlock(&alarm_mutex);
			if (status != 0)
				printf("fails unlock main");
				
		}
	}

	
	
}