#include "../inc/semaphore_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int create_semaphore() {
    int semid;
    union semun arg;
    
    /* Try to create semaphore */
    semid = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semid != -1) {
        /* Semaphore created, initialize to 1 */
        arg.val = 1;
        if (semctl(semid, 0, SETVAL, arg) == -1) {
            perror("semctl");
            return -1;
        }
    } else if (errno == EEXIST) {
        /* Semaphore already exists, get the ID */
        semid = semget(SEM_KEY, 1, 0666);
    } else {
        /* Error creating semaphore */
        perror("semget");
        return -1;
    }
    
    return semid;
}

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