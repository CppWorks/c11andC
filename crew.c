#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "errors.h"

#define CREW_SIZE 5
// QUEUED items of work for the crew. One is queued by crew_start, and each worker may queue addtional items
typedef struct work_tag{
	struct work_tag* next;
	// next work item
	char *path;
	// Directory or file
	char *string;
	// search string
} work_t, *work_p;

// one of these is initialised for each worker thread in the crew. It contains identify of each worker.
typedef struct worker_tag
{
	int index;
	// threads index
	pthread_t thread;
	// thread for stage
	struct crew_tag *crew;
	// pointer to crew;
}worker_t, *worker_p;

// crew contains worers..and work count and first work pointer...which can point to next work pointer.
// The external hanle for a work crew. Contains crew sync state and staging area
typedef struct crew_tag 
{
	int crew_size;
	// size of array;
	worker_t crew[CREW_SIZE];
	// crew members;
	long work_count;
	// count of work items;
	work_t   *first, *last;
	// first and last work item
	pthread_mutex_t mutex;
	// mutex for crew data
	pthread_cond_t done;
	// wait for crew done
	pthread_cond_t go;
	// wait for work;
	long path_max;
	// filesystem path limitl
	long name_max;
	// filesystem name limite
}crew_t, *crew_p;

// THe thread start routine for crew threads,Wait until go command, process work items until requested to shut down.
void* worker_routine(void* arg)
{
	// worker_t caontins crew index, its own thread.....and pointer to the whole crew
	worker_p mine = (worker_t*)arg;
	crew_p crew = mine->crew;
	work_p work, new_work;
   // work = (work_p)malloc(sizeof(work_t));
   // new_work = (work_p)malloc(sizeof(work_t));

	struct stat filestat; // file sats
	struct dirent *entry;
	int status;
	status = pthread_mutex_lock(&crew->mutex); // lock the whole crew mutex
	if (status!=0)
		printf("lock crw mutex");
	// THere wont be any work when the crw is crated. so wait until something is put on the queue
	// if there is nothing to do in the crwe work ount, just wait for crew go command..
	while (crew->work_count ==0)
	{
		//we are done
		status = pthread_cond_wait(&crew->go, &crew->mutex);
		if (status !=0)
			printf("wait for go ");
	}
	status = pthread_mutex_unlock(&crew->mutex);
	if (status!=0)
		printf("unlock mutex fail");
	//now we start doing work..the wcrew index is 
	DPRINTF(("Crew %d starting \n", mine->index));
	
	// struct dirent is funny because poxis does not require the definiation to be more than a header for a variable buffer. THuSSSSSSSALLOCATE
	// A "BIG CHUNK" of memory and ue it as a buffer
	// allowcate memerty for a file.
	entry = (struct dirent*)malloc(sizeof(struct dirent) + crew->name_max);
	if (entry==NULL)
			printf("allocating dirent");
	// now as long sa there is work, keep doing it
	while (1)
	{
		// keep going
		// wait while there is nothing to do, and the hope of something coming along later. If 
		//CREW->FIRSTIS NULL, there is no work. BUt if crew->work_count goes to zero, we are done
		status = pthread_mutex_lock(&crew->mutex);
		if (status != 0)
			printf("lock crew MUTEX");
	    DPRINTF(("crew %d top: first is %#x, count is %ld\n", mine->index,  crew->first, crew->work_count));
		while (crew->first == NULL)
		{
			// no work to do.....slightly diffferent
            printf("try to get work\n");
			status = pthread_cond_wait(&crew->go, &crew->mutex);
			// wait will release the lock
			if (status !=0)
				printf("wait for work");
		}
		// crwe-first is not null....who ever gets the change to lock the mutex, it will process the job
		DPRINTF(("crew %ld woke:%#lx,  %ld\n", (long)mine->index, crew->first, (long)crew->work_count));
		// remove and process a work item
		// move the pointer...very typicaly
		work = crew->first;
		crew->first = work->next;
		if (crew->first == NULL)
			crew->last = NULL;
	
		DPRINTF(("crew %ld took %#1x, leaves first %#lx, last %#lx\n", (long)mine->index, work, crew->first, crew->last)) ;

        printf("i AM TAKING WORK\n");
		// once I took the work, I am happy re lease the lock
		status = pthread_mutex_unlock(&crew->mutex);
		if (status!=0)
			err_abort(status, "UnlOCK UTEX");
		//WE HAVE A WORK ITEM, PROCESS IT , WHICH MAY INVLOVE QUEUING NEW WORK ITEMS
		status = lstat(work->path, &filestat);
		if (S_ISLNK(filestat.st_mode))
			printf("thread %d: %s is a link, skipping.\n", (int)mine->index, work->path); // dont care about symbolic link
		else if (S_ISDIR(filestat.st_mode))
		{
			printf("Oddo this is a folder\n");
            DIR *directory;
			struct dirent *result;
			// if file is a directory, search it and place all files onto the quene as new work items
			directory = opendir(work->path);
			if (directory == NULL)
			{
				fprintf(stderr, "unable to open directory %s: %d(%s)\n", work->path, errno, strerror(errno));
				continue;
			}

			while (1)
			{
				status = readdir_r(directory, entry, &result);
				if (status != 0)
				{
					fprintf(stderr, "unable to erad directory %s: %d (%s) \n", work->path, status, strerror(status));
					break;
				}
				if (result == NULL)
					break;
				// end of directory
				// ignore . and .. entries
				if (strcmp(entry->d_name, ".") == 0) continue;
				if (strcmp(entry->d_name, "..") == 0) continue;
				new_work = (work_p)malloc(sizeof(work_t));
				if (new_work == NULL)
					printf("unable to allocate space");
				new_work->path = (char*) malloc(40);
				if (new_work->path == NULL)
					printf("Unable to allocate path");
				strcpy(new_work->path, work->path);
				strcat(new_work->path,"/");
				strcat(new_work->path, entry->d_name);
				new_work->string = work->string;
                printf("Oddo new work string is \%s\n",new_work->string );
                printf("Oddo new work path is \%s\n", new_work->path);
				new_work->next = NULL;
				status = pthread_mutex_lock(&crew->mutex);
				if (status !=0)
					err_abort(status, "LOck mutex");
				if (crew->first == NULL)
				{
					crew->first = new_work;
					crew->last = new_work;
				}
				else
				{
					crew->last->next = new_work;  // link the work to the last item in the queue
					crew->last = new_work; // move the last pointer to the real last one for query in the future.
				}
				crew->work_count++;
				DPRINTF(("crew %ld: add %#lx, first %#lx, last %#lx, %ld\n", (long)mine->index, new_work, crew->first, crew->last, (long)crew->work_count));
				status = pthread_cond_signal(&crew->go);
				status = pthread_mutex_unlock(&crew->mutex);
				if (status !=0)
					err_abort(status,"Unlock mutex");
            };
			closedir(directory);
            printf("oddo close directory\n");
        }
		else if (S_ISREG(filestat.st_mode)) {
			printf("Oddo this is a file %s\n", work->path);
            FILE *search;
			char buffer[256], *bufptr, *search_ptr;
			 //if this is a file, not a directory, then search it for the string
            printf("Oddo opeen a file\n");
			 search = fopen(work->path, "r");
             printf("Oddo ok to open a file\n");
			if (search == NULL)
				fprintf(stderr, "Unable to open %s: %d (%s) \n", work->path, errno, strerror(errno));
			else
			{
				while (1)
				{
					
                    bufptr = fgets(buffer, sizeof(buffer), search);
					if (bufptr == NULL)
					{
						if (feof(search))
							break;
						if (ferror(search))
						{
						 	fprintf(stderr, "unable to read %s: %d (%s) \n", work->path, errno, strerror(errno));
							break;
						}
					}
					search_ptr = strstr(buffer, work->string);
					if (search_ptr != NULL)
					{
						printf("THread %d found \"%s\" in %s\n", mine->index, work->string, work->path);
						break;
					}
				}
				fclose(search);
			}
		}
		else fprintf(stderr, "Thread %d: %s IS TYPE %o(%s))\n", 
					 mine->index, work->path, filestat.st_mode & S_IFMT, 
					 (S_ISFIFO(filestat.st_mode) ? "FIFO" : (S_ISCHR(filestat.st_mode) ? "CHR" : 
															 (S_ISBLK(filestat.st_mode) ? "BLK" : 
															  (S_ISSOCK(filestat.st_mode) ? "SOCK" : "unknown")))));
        printf("free work path\n");
		//free(work->path);
		// freee path buffer
		//free(work);
      //  printf("free work");
		// we are done with this and move on the whilte loop to get newe work
		// Dcrement count of outstanding work items and wake waiters (trying to collect results or start a new calculatoin) if the crew is now idle
		// It is important that the count be decremented after prcessing the current work item. THaTTTTTTTTENSURES HE CONT WONT GO TO 0 until we are really done
		status = pthread_mutex_lock(&crew->mutex);
		if (status != 0)
			err_abort(status, "LOck crew mutex");
		crew->work_count--;
		DPRINTF(("oddo Crew %d decremented work to %d\n", mine->index, crew->work_count));
		if (crew->work_count <= 0)
		{
			DPRINTF(("Crew thread %d done\n", mine->index));
         //   printf("oddo break out of while loop");
			status = pthread_cond_broadcast(&crew->done);
			if (status != 0)
				err_abort(status, "Wake waiters");
          //  printf("oddo unlock");
			status = pthread_mutex_unlock(&crew->mutex);
			if (status != 0)
				err_abort(status, "Unlock mutex");
         //   printf("oddo break out of while loop");
			break;
		}
		status = pthread_mutex_unlock(&crew->mutex);
		if (status != 0)
			err_abort(status, "Unlock mutex");
        //free(entry);
	}
	printf("thread %d:, oddo break out of while loop\n", mine->index);
	return NULL;	
}

int crew_create (crew_t *crew, int crew_size)
{
	int crew_index;
	int status;
	// we cont create more than crew size
	if (crew_size > CREW_SIZE)
		return EINVAL;
	crew->crew_size = crew_size;
	crew->work_count = 0;
	crew->first = NULL;
	crew->last = NULL;
	// INIT synchoronisation objects;
	status  = pthread_mutex_init(&crew->mutex, NULL);
	if (status != 0)
		return status;
	status = pthread_cond_init(&crew->done, NULL);
	if (status != 0)
		return status;
		status = pthread_cond_init(&crew->go, NULL);
	if (status != 0)
		return status;
	// Create the worker thread
	for (crew_index = 0; crew_index < CREW_SIZE; crew_index++)
	{
		crew->crew[crew_index].index  = crew_index;
		crew->crew[crew_index].crew = crew;
		status = pthread_create(&crew->crew[crew_index].thread, NULL, worker_routine, (void*)&crew->crew[crew_index]);
		if (status != 0)
			err_abort(status, "Create worker");
	}
	return 0;
}


// pass a file path to a work crew previously created.....
// using crew create
int crew_start( crew_p crew, char *filepath, char *search)
{
//	printf("file path is %s\n", filepath);
    work_p request;
	int status;
	status = pthread_mutex_lock(&crew->mutex);
    if (status != 0)
        return status;
    // if the crew is busy, wait for them to finish
    while (crew->work_count > 0)
    {
        status = pthread_cond_wait(&crew->done, &crew->mutex);
        if (status != 0)
        {
            pthread_mutex_unlock(&crew->mutex);
            return status;
        }
    }
    errno = 0;
    crew->path_max = pathconf(filepath, _PC_PATH_MAX);
    if (crew->path_max == -1)
    {
        if (errno == 0)
            crew->path_max = 1024; // no limist
        else
            err_abort(1, "Unable to get path max");
    }
    errno = 0;
    crew->name_max = pathconf(filepath, _PC_NAME_MAX);
    if (crew->name_max == -1)
    {
        if (errno == 0)
            crew->name_max = 256; // no limit;
        else
            err_abort(1, "unable to get name max");
    }
    DPRINTF(("path_max for %s is %ld, name_max IS %ld\n", filepath, crew->path_max, crew->name_max));
    crew->path_max++; // add null byte
    crew->name_max++; // add null byte
    printf("oddo im before request\n");
    request = (work_p)malloc(sizeof(work_t));
    if (request == NULL)
        err_abort(1,"unable to allocate request");

  //   request->path = (char *) malloc(crew->path_max);
      request->path = (char *) malloc(40);
    if (request->path = NULL)
        err_abort(1,"Unable to allocate path");
    DPRINTF(("Oddo Requesting %s\n", filepath));  
    request->path = filepath;
   // strcpy(request->path, filepath);
    printf("oddo im after request->path %s\n", request->path);
    request->string = (char*) malloc(crew->name_max);
    strcpy(request->string, search);
     printf("oddo im after request->string %s\n", request->string);
 //   request->string = search;
    request->next = NULL;
    if (crew->first == NULL)
    {
        crew->first = request;
        crew->last = request;  
    }
    else
    {
        crew->last->next = request;
        crew->last = request;

    }
    crew->work_count++;
    status = pthread_cond_signal(&crew->go);
    if (status != 0)
    {

        free(crew->first);
        crew->first = NULL;
        crew->work_count = 0;
        pthread_mutex_unlock(&crew->mutex);
        return status;
    }
    while (crew->work_count > 0)
    {
        status = pthread_cond_wait(&crew->done, &crew->mutex);
        printf("oddo i am finished\n");
        if (status != 0)
            err_abort(status, "waiting for crew to fnish");
    }
    status = pthread_mutex_unlock(&crew->mutex);
    if (status != 0)
        err_abort(status, "unlock crew mutex");
    return 0;

}

// the main to drive the crew
// argv 1 is file path
// arg 2 is serach string
// eg. crew, cat, ~
int main(int argc, char* argv[])
{
    crew_t my_crew;
    char line[128], *next;
    int status;
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s string path \n", argv[0]);
        return -1;
    }
    status = crew_create(&my_crew, CREW_SIZE);
    if (status != 0)
        err_abort(status, "create crew");
    status = crew_start(&my_crew, argv[2], argv[1]);
    if (status != 0)
        err_abort(status, "start crew;");
    printf("im returned to main\n");
    sleep(2000);
    return 0;

}



























