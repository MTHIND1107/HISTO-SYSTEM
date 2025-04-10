/*
 * FILE: dp1.h
 * PROJECT: HISTO-SYSTEM
 * PROGRAMMER: Manreet Thind
 * FIRST VERSION: 08-04-2025
 * DESCRIPTION:
 * This header file declares the global variables and function prototypes
 * used by the DP-1 (Data Producer 1) component. It also defines the semaphore key
 * and declares the signal handler for SIGINT used to gracefully shut down DP-1.
 * REFERENCES:
 * https://docs.oracle.com/cd/E19683-01/806-6867/6jfpgdcnj/index.html 
 * https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_shared_memory.htm
 * https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/w/waitpid.html 
 */
#ifndef DP1_H
#define DP1_H

#include <signal.h>
#define SEM_KEY 0x1234

//Signal handler for SIGINT
void sigint_handler(int signum);

//Function to generate and write 20 random letters
void generate_and_write_letters();

//Global variables
extern int running; // Controls DP-1 loop, modified by SIGINT handler
extern int shmid;   // Stores shared memory ID for setup and cleanup
extern int semid; // Stores semaphore ID used to synchronize writes

#endif