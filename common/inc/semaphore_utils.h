/*
 * FILE: semaphore_utils.h
 * PROJECT: HISTOGRAM-SYSTEM
 * PROGRAMMER: Manreet Thind
 * FIRST VERSION: 08-04-2025
 * DESCRIPTION:
 * This header file provides semaphore utility functions for attaching, waiting, signaling,
 * and removing semaphores. It also defines the required union for semaphore control operations.
 */
#ifndef SEMAPHORE_UTILS_H
#define SEMAPHORE_UTILS_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/* Union for semaphore operations */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

/* Functions */
int attach_semaphore();
void semaphore_wait(int semid);
void semaphore_signal(int semid);
void remove_semaphore(int semid);

#endif /* SEMAPHORE_UTILS_H */