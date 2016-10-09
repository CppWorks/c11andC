#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>

/* We must define union semun ourselves. */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};
/* Obtain a binary semaphore’s ID, allocating if necessary. */
int binary_semaphore_allocation (key_t key, int sem_flags)
{
    return semget (key, 1, sem_flags);
}
/* Deallocate a binary semaphore. All users must have finished their
use. Returns -1 on failure. */
int binary_semaphore_deallocate (int semid)
{
    union semun ignored_argument;
    return semctl (semid, 1, IPC_RMID, ignored_argument);
}

/* Initialize a binary semaphore with a value of 1. */
int binary_semaphore_initialize (int semid)
{
    union semun argument;
    unsigned short values[1];
    values[0] = 1;
    argument.array = values;
    return semctl (semid, 0, SETALL, argument);
}

/* Wait on a binary semaphore. Block until the semaphore value is positive, then
decrement it by 1. */
int binary_semaphore_wait (int semid)
{
    struct sembuf operations[1];
    /* Use the first (and only) semaphore. */
    operations[0].sem_num = 0;
    /* Decrement by 1. */
    operations[0].sem_op = -1;
    /* Permit undo’ing. */
    operations[0].sem_flg = SEM_UNDO;
    return semop (semid, operations, 1);
}
/* Post to a binary semaphore: increment its value by 1.
This returns immediately. */
int binary_semaphore_post (int semid)
{
    struct sembuf operations[1];
    /* Use the first (and only) semaphore. */
    operations[0].sem_num = 0;
    /* Increment by 1. */
    operations[0].sem_op = 1;
    /* Permit undo’ing. */
    operations[0].sem_flg = SEM_UNDO;
    return semop (semid, operations, 1);
}

int main ()
{
    int segment_id;
    char* shared_memory;
    struct shmid_ds shmbuffer;
    int segment_size;
    const int shared_segment_size = 0x6400;
    /* Allocate a shared memory segment. */
    segment_id = shmget (IPC_PRIVATE, shared_segment_size,
        IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    /* Attach the shared memory segment. */ // mapping
    shared_memory = (char*) shmat (segment_id, 0, 0);
    printf ("shared memory attached at address %p\n", shared_memory);
    /* Determine the segment’s size. */
    shmctl (segment_id, IPC_STAT, &shmbuffer);
    segment_size = shmbuffer.shm_segsz;
    printf ("segment size: %d\n", segment_size);
    /* Write a string to the shared memory segment. */
    sprintf (shared_memory, "Hello, world.");
    /* Detach the shared memory segment. */
    shmdt (shared_memory);
    /* Reattach the shared memory segment, at a different address. */
    shared_memory = (char*) shmat (segment_id, (void*) 0x5000000, 0);
    printf ("shared memory reattached at address %p\n", shared_memory);
    /* Print out the string from shared memory. */
    printf ("%s\n", shared_memory);
    /* Detach the shared memory segment. */
    shmdt (shared_memory);
    /* Deallocate the shared memory segment. */
    shmctl (segment_id, IPC_RMID, 0);
    return 0;
}