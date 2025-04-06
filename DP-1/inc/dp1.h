#ifndef DP1_H
#define DP1_H

#include <signal.h>

/* Signal handler for SIGINT */
void sigint_handler(int signum);

/* Function to generate and write 20 random letters */
void generate_and_write_letters();

/* Global variables */
extern int running;
extern int shmid;
extern int semid;

#endif /* DP1_H */