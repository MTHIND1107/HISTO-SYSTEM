#ifndef DC_H
#define DC_H

#include <signal.h>

/* Signal handlers */
void sigint_handler(int signum);
void sigalrm_handler(int signum);

/* Function to display histogram */
void display_histogram();

/* Function to clean up and exit */
void cleanup_and_exit();

/* Global variables */
extern int running;
extern int cleanup_mode;
extern int shmid;
extern int semid;
extern pid_t dp1_pid;
extern pid_t dp2_pid;
extern int letter_counts[20];  /* Counts for letters A-T */

#endif /* DC_H */