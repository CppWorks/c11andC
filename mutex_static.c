#include <pthread.h>


// This is static way....
typedef struct my_struct_tag{
	pthread_mutex_t mutex;
	int value;
} my_struct_t;

// call PTHREAD_MUTEX_INITIALIZER for static 
my_struct_t data = {PTHREAD_MUTEX_INITIALIZER, 0};

int main (int argc, char *argv[])
{
	return 0;
}