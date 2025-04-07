#include "../inc/dc.h"
#include "../../common/inc/shared_memory.h"
#include "../../common/inc/semaphore_utils.h"
#include "../../common/inc/circular_buffer.h"
#include "../../common/inc/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

/* Number of letters to read every 2 seconds */
#define READ_BATCH_SIZE 40

/* Global variables */
int running = 1;
int cleanup_mode = 0;
int shmid = -1;
int semid = -1;
pid_t dp1_pid = -1;
pid_t dp2_pid = -1;
shared_memory_t *shm = NULL;
int letter_counts[LETTER_RANGE] = {0};  /* Counts for letters A-T */
int histogram_timer = 0;  /* Counter for 10-second histogram display */

/**
 * Signal handler for SIGINT
 */
void sigint_handler(int signum) {
    /* Set cleanup mode */
    cleanup_mode = 1;
    
    /* Send SIGINT to producer processes */
    if (dp1_pid > 0) {
        kill(dp1_pid, SIGINT);
    }
    if (dp2_pid > 0) {
        kill(dp2_pid, SIGINT);
    }
}

/**
 * Signal handler for SIGALRM
 */
void sigalrm_handler(int signum) {
    char buffer[READ_BATCH_SIZE];
    int num_read;
    
    /* Acquire semaphore */
    semaphore_wait(semid);
    
    /* Read letters from the buffer */
    num_read = bulk_read_from_buffer(shm, buffer, READ_BATCH_SIZE);
    
    /* Release semaphore */
    semaphore_signal(semid);
    
    /* Update letter counts */
    for (int i = 0; i < num_read; i++) {
        if (buffer[i] >= MIN_LETTER && buffer[i] <= MAX_LETTER) {
            letter_counts[buffer[i] - MIN_LETTER]++;
        }
    }
    
    /* Update histogram timer */
    histogram_timer++;
    if (histogram_timer >= 5 || cleanup_mode) {  /* Display every 10 seconds (5 * 2s) */
        display_histogram();
        histogram_timer = 0;
    }
    
    /* If in cleanup mode and no more data to read, exit */
    if (cleanup_mode && num_read == 0) {
        running = 0;
        return;
    }
    
    /* Set alarm for next read */
    alarm(2);
}