#ifndef DP2_H
#define DP2_H

#include <signal.h>

/* Signal handler for SIGINT */
void sigint_handler(int signum);

/* Global variables */
extern int running;
extern int shmid;
extern int semid;

#endif /* DP2_H */