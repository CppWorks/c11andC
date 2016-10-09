#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "errors.h"
//internal structure describinh a stage in the piple line. One for each tread, plus a result stage where the final thread can stash the value
typedef struct stage_tag {
	pthread_mutex_t mutex;
	//if we dont have data, we tell we are free.
	pthread_cond_t avail;
	// once we finish data, we become busy
	pthread_cond_t ready;
	// read for get data
	int data_ready;
	long data;
	// thread for stage
	pthread_t thread;
	// next stage
	struct stage_tag *next;
    int stageIndex;
} stage_t;

// external structure representing the entire pipline
typedef struct pipe_tag
{
	pthread_mutex_t mutex;
	// mutext to protect the pipe
	// first stage;
	stage_t *head;
	// final stage
	stage_t * tail;
	// number of stages
	int stages;
	int active; // active data elements;
}pipe_t;

long queue[10];

// we use two states of data_ready to do the work...ready = 0, 
// two conds, two signals.
// one thread, wait for next thread to become free (by sending a signal),after free, the thread waits for a busy work from previous thread.
//internal function to send a mesage to the specified pipe stage. THreads use this to pass along modified data
// in c true is represented by any value nto equal to 0.
int pipe_send(stage_t* stage, long data)
{
	int status;
     printf("try lock stage is %d\n ", stage->stageIndex);
	status = pthread_mutex_lock(&stage->mutex);
    printf(" ok lock stage is %d\n ", stage->stageIndex);
	if (status != 0 )
		printf("send lock fail");
	// if there is data inthe pip stage..wait for it to consumed
   // printf("stage ready is %d\n ", stage->data_ready);
	while (stage->data_ready != 0) // data_read y= 0 means false; data_ready = 1 means we ar eprocessing so we wait for it to be free data_ready = 0
	{
       // printf("stage not ready is stage is %d\n ", stage->stageIndex);
        // if this is stage 0, we queue the numbers.
        if (stage->stageIndex == 0)
        {
            size_t nextPosition;
            for (size_t f = 0; f < 10; f++)
            {
                if (queue[f] == 0)
                {
                    nextPosition = f;
                    break;
                }
            }
            queue[nextPosition] = data;
            // dont hang on stage 0
            printf("stage 0 is busy, queue number for now\n");
        }
      //  printf("stage waaaaaait");
        status = pthread_cond_wait(&stage->ready, &stage->mutex);
      //  printf("free staget ready is stage is %d\n ", stage->stageIndex);
		if (status !=0 )
		{
			printf("send wait fail");
			return status;
		}

	}

	
	// send the next data;
	stage->data = data;
	stage->data_ready = 1; // 1 means we are processing the data?
    printf("next stage index  %d with ready %d\n", stage->stageIndex, stage->data_ready);
	// there is no thread to process the final stage.
	status = pthread_cond_signal(&stage->avail);
	if (status != 0)
	{
		pthread_mutex_unlock(&stage->mutex);
		printf("avil cond failed");
		return status;
	}
	if (stage->next == NULL)
	    printf("oddo call final\n");
	status = pthread_mutex_unlock(&stage->mutex);
	return status;
}

// the read start routine fo pipe stage threads
// each will wait for a data item passed from the caller or the previous stage, modify the data and pass it along to thenext stage
void *pipe_stage(void* arg)
{
	stage_t *stage = (stage_t*)arg;
//	printf("stage data is %d\n", stage->data);
	stage_t *next_stage = stage->next;
	int status;
	status = pthread_mutex_lock(&stage->mutex);
	if (status != 0)
		err_abort(status,"pipstage failed");
	while (1)
	{
		// no data just wait
		while (stage->data_ready != 1)
		{
			printf("unlock %d\n", stage->stageIndex);
            status = pthread_cond_wait(&stage->avail, &stage->mutex);
            printf("relock %d\n", stage->stageIndex);
			if (status != 0)
				printf("wait for previous stage failed");
		}
		// have data, so now process
        printf("send from stage %d with ready value %d\n", stage->stageIndex, stage->data_ready);
		pipe_send(next_stage,stage->data + 1);
        // if we dont return from send, the mutex is still locked...previous stage cannot even lock this mutex
        // it will just hang in there..
        printf("back from send for stage %d\n", stage->stageIndex);
		stage->data_ready = 0;
		status = pthread_cond_signal(&stage->ready);
		if (status !=0 )
			printf("wake next stauge failed");
	}
	// note the routie never unlock stage_mutex..the call to cond-wait implicit unlock the mutex while the thread is waiting..so we dont need to unlock again
	// because the loop never stop, no need to unlock explicitly.
			  
}

		
// external interface to crate a pipline

int pipe_create(pipe_t *pipe, int stages)
{
	int pipe_index;
	stage_t **link = &pipe->head, *new_stage, *stage;
	int status;
	status = pthread_mutex_init(&pipe->mutex, NULL);
	if (status != 0)
		printf("craete mutex init fails");
	pipe->stages = stages;
	pipe->active =0;
    pthread_mutexattr_t mutex_attr;
	for (pipe_index = 0; pipe_index <= stages; pipe_index++)
	{
		new_stage = (stage_t*)malloc(sizeof(stage_t));

        new_stage->stageIndex = pipe_index;
		if (new_stage == NULL)
		printf("allocate fail");

        if (index == 0)
        {
            status = pthread_mutexattr_init(&mutex_attr);
            if (status != 0)
                printf("init mutex attr failed");
            status = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK);
            if (status != 0)
                printf("set attr failed");
            status = pthread_mutex_init(&new_stage->mutex, &mutex_attr);
            if (status != 0)
                printf("init stame mutex failed");
        }
        else

        {
            status = pthread_mutex_init(&new_stage->mutex, NULL);
            if (status != 0)
                printf("init stame mutex failed");

        }

		status = pthread_cond_init(&new_stage->avail, NULL);
		if (status !=0)
			printf("init avail");
		status = pthread_cond_init(&new_stage->ready, NULL);
		if (status !=0 )
			printf("eady cond");
		new_stage-> data_ready = 0; //free
		// connect to the new stage
		*link = new_stage;
		link = &new_stage->next;
	}
	*link = (stage_t*)NULL;
	pipe->tail = new_stage;

	// create the thread for th pipe
	// it wont create a thread for the final stage. we check it manually.
	for (stage = pipe->head; stage->next != NULL; stage = stage->next)
	{
		status = pthread_create(&stage->thread, NULL, pipe_stage, (void*) stage);
		if (status != 0 )
			printf("crate pip stage");
	}
	
	return 0;
}

// extrenal interface to start a piple by passing data to the first tage. the routine return while pipline process in parallel. Call pip_result return to collect the final stage values
// note the piple will stall when each stage fills until the result is collected
int pipe_start(pipe_t *pipe, long value)
{
	int status;
	status = pthread_mutex_lock(&pipe->mutex);
	if (status !=0 )
		printf("lock pipe");
	pipe->active++;
	status = pthread_mutex_unlock(&pipe->mutex);
	if (status !=0)
		printf("unlock pipe");
  //  printf("start value is %d", value);
	pipe_send(pipe->head, value);
	return 0;
}
// collect result wait for a result if the pipline has not produced one
int pipe_result(pipe_t *pipe, long *result)
{
	stage_t *tail = pipe->tail;
	long value;
	int empty = 0;
	int status;
	status = pthread_mutex_lock(&pipe->mutex);
	if (status!=0)
		printf("lock pip mutex");
	if (pipe->active <= 0)
		empty = 1;
	else
		pipe->active--;
	status = pthread_mutex_unlock(&pipe->mutex);
	if (status!=0)
		printf("unlock pipe mutex");
	if (empty == 1)
		return 0;
	pthread_mutex_lock(&tail->mutex);
	while (tail->data_ready == 0)
		pthread_cond_wait(&tail->avail, &tail->mutex);
	*result = tail->data;
	tail->data_ready = 0;
	pthread_cond_signal(&tail->ready);
	pthread_mutex_unlock(&tail->mutex);
	return 1;
}
	
int main(int argc, char*argv[])
{
	pipe_t my_pipe;
	long value, result;
	int status;
	char line[128];
	pipe_create(&my_pipe, 3);
	printf("enter integer value, or \"=\" for next result\n");

    for (size_t index = 0 ; index < 10; index++)
    {
        queue[index] = 0;
    }
	while (1)
	{
		printf("Data>");
		if (fgets(line, sizeof(line), stdin) == NULL) 
		{
			printf("oh no");
			exit(0);
		}
		if (strlen(line) <=1) continue;
		if (strlen(line) <=2 && line[0] == '=')
		{
			if (pipe_result(&my_pipe, &result))
				printf("resutls is %ld\n", result);
			else
				printf("pipe is empty\n");
				
		}
		else
		{
			if (sscanf(line, "%ld", &value) < 1)
				fprintf(stderr, "enter an integer value\n");
			else
				pipe_start(&my_pipe,value);
		}
	}
}
			  
			  
			  
			  
			  
			  
			  
			  
			  
			  
			  
			 
			  
			  
			  
			  
			  
			  
			  
			  
			  
			  
			  
			  
