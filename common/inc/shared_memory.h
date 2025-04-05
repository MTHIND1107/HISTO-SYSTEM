#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/* Constants */
#define BUFFER_SIZE 256
#define SHM_KEY 9876  /* Arbitrary key for shared memory */
#define SEM_KEY 5432  /* Arbitrary key for semaphore */

/* Shared memory structure */
typedef struct {
    char buffer[BUFFER_SIZE];  /* Circular buffer to hold letters A-T */
    int read_index;            /* Index where DC reads from */
    int write_index;           /* Index where DPs write to */
} shared_memory_t;

/* Functions */
int create_shared_memory();
int attach_shared_memory(int shmid, shared_memory_t **shm);
void detach_shared_memory(shared_memory_t *shm);
void remove_shared_memory(int shmid);
void init_shared_memory(shared_memory_t *shm);

#endif /* SHARED_MEMORY_H */