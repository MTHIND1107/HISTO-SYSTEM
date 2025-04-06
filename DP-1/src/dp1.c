#include "../inc/dp1.h"
#include "../../common/inc/shared_memory.h"
#include "../../common/inc/semaphore_utils.h"
#include "../../common/inc/circular_buffer.h"
#include "../../common/inc/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

/* Global variables */
int running = 1;
int shmid = -1;
int semid = -1;
shared_memory_t *shm = NULL;

/**
 * Signal handler for SIGINT
 */
void sigint_handler(int signum) {
    running = 0;
}

/**
 * Generate and write 20 random letters to the buffer
 */
void generate_and_write_letters() {
    char letters[20];
    
    /* Generate 20 random letters */
    for (int i = 0; i < 20; i++) {
        letters[i] = generate_random_letter();
    }
    
    /* Acquire semaphore */
    semaphore_wait(semid);
    
    /* Write letters to buffer */
    int written = bulk_write_to_buffer(shm, letters, 20);
    
    /* Release semaphore */
    semaphore_signal(semid);
}

