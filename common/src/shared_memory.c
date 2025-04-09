/*
 * FILE: shared_memory.c
 * PROJECT: HISTOGRAM-SYSTEM
 * PROGRAMMER: Manreet Thind
 * FIRST VERSION: 08-04-2025
 * DESCRIPTION:
 * Implements shared memory operations used for circular buffer communication
 * between producer and consumer processes.
 */
#include "../inc/shared_memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/*
 * Name    : create_shared_memory
 * Purpose : Creates shared memory or attaches to existing one
 * Input   : None
 * Outputs : None
 * Returns : Shared memory ID or -1 on error
 */
int create_shared_memory() {
    int shmid;
    
    /* Try to create shared memory */
    shmid = shmget(SHM_KEY, sizeof(shared_memory_t), IPC_CREAT | IPC_EXCL | 0666);
    if (shmid != -1) {
        /* Created new shared memory */
        return shmid;
    } else if (errno == EEXIST) {
        /* Shared memory already exists, get the ID */
        return shmget(SHM_KEY, sizeof(shared_memory_t), 0666);
    } else {
        /* Error creating shared memory */
        perror("shmget");
        return -1;
    }
}

/*
 * Name    : attach_shared_memory
 * Purpose : Attaches process to shared memory segment
 * Input   : Shared memory ID, double pointer to shared_memory_t
 * Outputs : shm pointer initialized
 * Returns : 0 on success, -1 on failure
 */
int attach_shared_memory(int shmid, shared_memory_t **shm) {
    *shm = (shared_memory_t *)shmat(shmid, NULL, 0);
    if (*shm == (shared_memory_t *)-1) {
        perror("shmat");
        return -1;
    }
    return 0;
}

/*
 * Name    : detach_shared_memory
 * Purpose : Detaches process from shared memory segment
 * Input   : Pointer to shared memory
 * Outputs : None
 * Returns : None
 */
void detach_shared_memory(shared_memory_t *shm) {
    if (shm != NULL) {
        shmdt(shm);
    }
}

/*
 * Name    : remove_shared_memory
 * Purpose : Destroys shared memory segment
 * Input   : Shared memory ID
 * Outputs : None
 * Returns : None
 */
void remove_shared_memory(int shmid) {
    shmctl(shmid, IPC_RMID, NULL);
}

/*
 * Name    : init_shared_memory
 * Purpose : Initializes the buffer and indices to default state
 * Input   : Pointer to shared memory
 * Outputs : Buffer zeroed, indices reset
 * Returns : None
 */
void init_shared_memory(shared_memory_t *shm) {
    memset(shm->buffer, 0, BUFFER_SIZE);
    shm->read_index = 0;
    shm->write_index = 0;
}  