#ifndef DC_H
#define DC_H

#include <signal.h>

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