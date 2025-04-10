/*
 * FILE: dp2.h
 * PROJECT: HISTOGRAM-SYSTEM
 * PROGRAMMER: Manreet Thind
 * FIRST VERSION: 08-04-2025
 * DESCRIPTION:
 * This header file contains declarations for the DP-2 (Data Producer 2) component.
 * It includes the SIGINT signal handler declaration and the global variables required
 * for managing the shared memory and semaphore attachment during runtime.
 * REFERENCES:
 * https://docs.oracle.com/cd/E19683-01/806-6867/6jfpgdcnj/index.html 
 * https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_shared_memory.htm
 * https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/w/waitpid.html 
 */
#ifndef DP2_H
#define DP2_H

#include <signal.h>

// Signal handler for SIGINT 
void sigint_handler(int signum);

//Global variables
extern int running;  // Controls DP-2 main loop, set to 0 on SIGINT
extern int shmid;  // Shared memory ID passed from DP-1 to DC
extern int semid; // Semaphore ID used during letter writes

#endif