/*
 * FILE: dp2.h
 * PROJECT: HISTOGRAM-SYSTEM
 * PROGRAMMER: Manreet Thind
 * FIRST VERSION: 08-04-2025
 * DESCRIPTION:
 * This header file contains declarations for the DP-2 (Data Producer 2) component.
 * It includes the SIGINT signal handler declaration and the global variables required
 * for managing the shared memory and semaphore attachment during runtime.
 */
#ifndef DP2_H
#define DP2_H

#include <signal.h>

// Signal handler for SIGINT 
void sigint_handler(int signum);

//Global variables
extern int running;
extern int shmid;
extern int semid;

#endif