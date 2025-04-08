/*
 * FILE: dc.h
 * PROJECT: HISTOGRAM-SYSTEM
 * PROGRAMMER: Manreet Thind
 * FIRST VERSION: 08-04-2025
 * DESCRIPTION:
 * This header file declares the function prototypes and global variables
 * used by the DC (Data Consumer) component. It includes signal handlers,
 * histogram logic, and IPC-related shared variables.
 */
#ifndef DC_H
#define DC_H

#include <signal.h>
#include <sys/types.h>

/* Letter range constants */
#define MIN_LETTER 'A'
#define MAX_LETTER 'T'
#define LETTER_RANGE (MAX_LETTER - MIN_LETTER + 1)


/* Signal handlers */
void sigint_handler(int signum);
void sigalrm_handler(int signum);

/* Function to display histogram */
void display_histogram(void);

/* Function to clean up and exit */
void cleanup_and_exit(void);

/* Global variables */
extern volatile sig_atomic_t running;
extern volatile sig_atomic_t cleanup_mode;
extern int shmid;
extern int semid;
extern pid_t dp1_pid;
extern pid_t dp2_pid;
extern int letter_counts[LETTER_RANGE];  /* Counts for letters A-T */

#endif /* DC_H */