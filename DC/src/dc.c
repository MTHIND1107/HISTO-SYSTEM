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

/**
 * Display histogram of letter counts
 */
void display_histogram() {
    /* Clear screen */
    printf("\033[2J\033[H");
    
    /* Display histogram for each letter */
    for (int i = 0; i < LETTER_RANGE; i++) {
        char letter = MIN_LETTER + i;
        int count = letter_counts[i];
        
        /* Display letter and count */
        printf("%c-%03d ", letter, count);
        
        /* Display histogram bars */
        int hundreds = count / 100;
        int tens = (count % 100) / 10;
        int ones = count % 10;
        
        /* Display hundreds as '*' */
        for (int h = 0; h < hundreds; h++) {
            printf("*");
        }
        
        /* Display tens as '+' */
        for (int t = 0; t < tens; t++) {
            printf("+");
        }
        
        /* Display ones as '-' */
        for (int o = 0; o < ones; o++) {
            printf("-");
        }
        
        printf("\n");
    }
    
    fflush(stdout);
}

/**
 * Clean up and exit
 */
void cleanup_and_exit() {
    /* Display final histogram */
    display_histogram();
    
    /* Clean up IPC resources if we're the last to use them */
    detach_shared_memory(shm);
    remove_shared_memory(shmid);
    remove_semaphore(semid);
    
    /* Exit message */
    printf("Shazam !!\n");
}

int main(int argc, char *argv[]) {
    /* Check arguments */
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <shmid> <dp1_pid> <dp2_pid>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    /* Get command line arguments */
    shmid = atoi(argv[1]);
    dp1_pid = atoi(argv[2]);
    dp2_pid = atoi(argv[3]);
    
    /* Verify arguments */
    if (shmid <= 0 || dp1_pid <= 0 || dp2_pid <= 0) {
        fprintf(stderr, "Invalid arguments\n");
        return EXIT_FAILURE;
    }
    
    /* Attach to shared memory */
    if (attach_shared_memory(shmid, &shm) != 0) {
        fprintf(stderr, "Failed to attach to shared memory\n");
        return EXIT_FAILURE;
    }
    
    /* Create semaphore */
    semid = create_semaphore();
    if (semid == -1) {
        fprintf(stderr, "Failed to create semaphore\n");
        detach_shared_memory(shm);
        return EXIT_FAILURE;
    }
    
    /* Set up signal handlers */
    signal(SIGINT, sigint_handler);
    signal(SIGALRM, sigalrm_handler);
    
    /* Start the 2-second alarm for reading data */
    alarm(2);
    
    /* Main loop */
    while (running) {
        pause();  /* Wait for signals */
    }
    
    /* Clean up and exit */
    cleanup_and_exit();
    
    return EXIT_SUCCESS;
}