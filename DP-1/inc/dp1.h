/*
 * FILE: dp1.h
 * PROJECT: HISTO-SYSTEM
 * PROGRAMMER: Manreet Thind
 * FIRST VERSION: 08-04-2025
 * DESCRIPTION:
 * This header file declares the global variables and function prototypes
 * used by the DP-1 (Data Producer 1) component. It also defines the semaphore key
 * and declares the signal handler for SIGINT used to gracefully shut down DP-1.
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
extern int running;
extern int shmid;
extern int semid;

#endif