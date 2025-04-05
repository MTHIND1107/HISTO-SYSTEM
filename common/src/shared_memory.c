#include "../inc/shared_memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int attach_shared_memory(int shmid, shared_memory_t **shm) {
    *shm = (shared_memory_t *)shmat(shmid, NULL, 0);
    if (*shm == (shared_memory_t *)-1) {
        perror("shmat");
        return -1;
    }
    return 0;
}