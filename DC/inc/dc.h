/*
 * FILE: dc.h
 * PROJECT: HISTOGRAM-SYSTEM
 * PROGRAMMER: Manreet Thind
 * FIRST VERSION: 08-04-2025
 * DESCRIPTION:
 * This header file declares the function prototypes and global variables
 * used by the DC (Data Consumer) component. It includes signal handlers,
 * histogram logic, and IPC-related shared variables.
 * REFERENCES:
 * https://medium.com/@razika28/signals-ad83f38f80b6 
 * https://www.tutorialspoint.com/c_standard_library/c_function_signal.htm 
 * https://beej.us/guide/bgc/html/split/signal-handling.html 
 * https://pubs.opengroup.org/onlinepubs/007904875/functions/sigaction.html#:~:text=The%20sigaction()%20function%20allows,h%3E.&text=Pointer%20to%20a%20signal%2Dcatching,the%20macros%20SIG_IGN%20or%20SIG_DFL. 
 */
#ifndef DC_H
#define DC_H

#include <signal.h>
#include <sys/types.h>

// Letter range constants
#define MIN_LETTER 'A'
#define MAX_LETTER 'T'
#define LETTER_RANGE (MAX_LETTER - MIN_LETTER + 1)


// Signal handlers
void sigint_handler(int signum);
void sigalrm_handler(int signum);

// Function to display histogram
void display_histogram(void);

// Function to clean up and exit
void cleanup_and_exit(void);

// Global variables
extern volatile sig_atomic_t running; // Used in signal handlers and main loop to control program flow
extern volatile sig_atomic_t cleanup_mode; // Indicates cleanup mode across signal handler and main logic
extern int shmid; // Shared memory ID needed in multiple functions
extern int semid;  // Semaphore ID accessed by reading and cleanup functions
extern pid_t dp1_pid;  // Required to send SIGINT to DP-1 during shutdown
extern pid_t dp2_pid; // Required to send SIGINT to DP-2 during shutdown
extern int letter_counts[LETTER_RANGE];  // Stores histogram data used by multiple functions

#endif /* DC_H */