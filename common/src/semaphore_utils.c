/*
 * FILE: semaphore_utils.c
 * PROJECT: HISTOGRAM-SYSTEM
 * PROGRAMMER: Manreet Thind
 * FIRST VERSION: 08-04-2025
 * DESCRIPTION:
 * This file contains semaphore utility functions to synchronize access
 * to shared memory by producers and consumers.
 */
#include "../inc/semaphore_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define SEM_KEY 0x1234

/*
 * Name    : attach_semaphore
 * Purpose : Attaches to an existing semaphore
 * Input   : None
 * Outputs : None
 * Returns : Semaphore ID or -1 on error
 */
int attach_semaphore() {
    int semid = semget(SEM_KEY, 1, 0666); // Attach only (no IPC_CREAT)
    if (semid == -1) {
        perror("attach_semaphore: semget failed");
    }
    return semid;
}

/*
 * Name    : semaphore_wait
 * Purpose : Lock the semaphore (decrement)
 * Input   : Semaphore ID
 * Outputs : Blocks if resource is unavailable
 * Returns : None
 */
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

/*
 * Name    : semaphore_signal
 * Purpose : Unlock the semaphore (increment)
 * Input   : Semaphore ID
 * Outputs : Releases one blocked process if any
 * Returns : None
 */
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

/*
 * Name    : remove_semaphore
 * Purpose : Remove the semaphore set from the system
 * Input   : Semaphore ID
 * Outputs : Frees system resource
 * Returns : None
 */
void remove_semaphore(int semid) {
    semctl(semid, 0, IPC_RMID);
}