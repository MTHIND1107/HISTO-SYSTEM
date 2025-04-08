#include "../inc/semaphore_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define SEM_KEY 0x1234

void semaphore_wait(int semid) {
    struct sembuf operation;
    
    operation.sem_num = 0;    /* First (and only) semaphore */
    operation.sem_op = -1;    /* Decrement by 1 (wait) */
    operation.sem_flg = 0;    /* Block until operation can be performed */
    
    if (semop(semid, &operation, 1) == -1) {
        perror("semop wait");
        exit(EXIT_FAILURE);
    }
}

void semaphore_signal(int semid) {
    struct sembuf operation;
    
    operation.sem_num = 0;    /* First (and only) semaphore */
    operation.sem_op = 1;     /* Increment by 1 (signal) */
    operation.sem_flg = 0;    /* Operation flags */
    
    if (semop(semid, &operation, 1) == -1) {
        perror("semop signal");
        exit(EXIT_FAILURE);
    }
}

void remove_semaphore(int semid) {
    semctl(semid, 0, IPC_RMID);
}