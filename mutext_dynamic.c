#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// This is static way....
typedef struct my_struct_tag{
	pthread_mutex_t mutex;
	int value;
} my_struct_t;


int main (int argc, char *argv[])
{
	my_struct_t *data;
	int status;
	data = malloc(sizeof(my_struct_t));

	if (data == NULL)
		printf("oh noooo");
	status = pthread_mutex_init(&data->mutex, NULL);
	// We need to release it
	status = pthread_mutex_destroy(&data->mutex);
	
	return 0;
}