#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct alarm_tag {
	struct alarm_tag *link;
	int seconds;
	time_t time;
	char message[64];
} alarm_t;

pthread_mutex_t alarm_mutex = PTHREAD_MUTEX_INITIALIZER;
alarm_t* alarm_list = NULL;

// START ROUTINE;
void* alarm_thread(void *arg){

	alarm_t *alarm;
	int sleep_time;
	time_t now;
	int status;
	// Loop forever processing cimmand.
	while (1) {
		//printf("running alarm thrad \n");
		status = pthread_mutex_lock(&alarm_mutex);
		if (status !=0 )
			printf("od error");
		alarm = alarm_list;
		if (alarm == NULL)
			sleep_time = 1;
		else {
			// alarm is the current alarm.
			// alarm list moves on to the next alarm.
			alarm_list = alarm->link;
			now = time(NULL);
			// if alarm time is less then now...don't need to sleep
			if (alarm->time <= now)
				sleep_time = 0;
			// otherwise sleeep time = arlam time - now
			else
				sleep_time = alarm->time - now;
			printf("[waiting:%d(%d)\"%s\"]\n", (int)alarm->time, sleep_time, alarm->message);			
		}
		status =  pthread_mutex_unlock(&alarm_mutex);
		if (sleep_time > 0)
			sleep(sleep_time);
		else
			sched_yield();
		
		if (alarm != NULL) {
			printf ("(%d) %s\n", alarm->seconds, alarm->message);
			free (alarm);
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
			// pinter to a pointer which points to the start of the alarm list
			last = &alarm_list;
			// def to get the first alarm.
			// draw a picture it is so clear.
			
			/*
			
					see linked list picture.
			
			*/
			next = *last;
			while (next != NULL)
			{
				// if this alarm time is > input alarm time, insert the alarm before next.
				if (next->time >= alarm->time)
				{
					alarm->link = next;  // new alarm link to next alarm.
					// last point to this alarm.
					*last = alarm;
					break;
				}
				// next is small.  move on to pointer pointing to the link pointer. def will get the next alarm pointer
				// last is really looking back.
				// we are going along the `next-link' pointer.
				last = &next->link;
				// next alarm is pointer to the next alarm.
				next = next->link;
			}
			// if we reach the endo f the list, insert the new alarm here
			if (next == NULL)
			{
				printf("new alarm");
				*last = alarm;
				alarm->link = NULL;
			}
			printf("[list:");
			for (next = alarm_list; next != NULL; next = next->link)
				printf("%d(%d)[\"%s\"\"]", (int)next->time, (int)(next->time - time(NULL)), next->message);
			printf("]\n");
			status = pthread_mutex_unlock(&alarm_mutex);
			if (status != 0) printf("error unlock");	
				
		}
	}
}






































