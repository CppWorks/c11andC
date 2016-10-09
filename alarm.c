#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main (int argc, char *argv[])
{
    int seconds;
	char line[128];
	char message[64];
	while (1)
	{
		printf("alarm> ");
		if (fgets(line, sizeof(line), stdin) == NULL) 
		{
			printf("oh no");
			exit(0);
		}
		if (sscanf(line, "%d %64[^\n]", &seconds, message) < 2)
		{
			fprintf(stderr, "bad command\n");
		}
		else
		{
			sleep(seconds);
			printf("(%d)  %s\n", seconds, message);
		}
	}
}

// gcc -Wno-implicit alarm.c -o hello -lpthread
