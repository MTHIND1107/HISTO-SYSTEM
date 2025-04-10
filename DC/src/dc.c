/*
 * FILE: dc.c
 * PROJECT: HISTOGRAM-SYSTEM
 * PROGRAMMER: Manreet Thind
 * FIRST VERSION: 08-04-2025
 * DESCRIPTION:
 * This file implements the DC (Data Consumer) component. It attaches to shared memory and semaphore,
 * reads letters written by the producers, and builds a histogram showing the frequency of letters A-T.
 * The histogram is displayed every 10 seconds. On receiving SIGINT, the DC process initiates cleanup,
 * signals the producers to stop, drains the remaining buffer data, displays the final histogram,
 * and prints "Shazam !!" before exiting.
 */
#define _POSIX_C_SOURCE 200809L

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
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>

// Number of letters to read every 2 seconds 
#define READ_BATCH_SIZE 40
#define SEM_KEY 0x1234

// Global variables 
volatile sig_atomic_t running = 1;
volatile sig_atomic_t cleanup_mode = 0;
int shmid = -1;
int semid = -1;
pid_t dp1_pid = -1;
pid_t dp2_pid = -1;
shared_memory_t *shm = NULL;
int letter_counts[LETTER_RANGE] = {0};  // Counts for letters A-T
time_t last_histogram_time = 0;  // Counter for 10-second histogram display

/*
 * Name    : sigint_handler
 * Purpose : Handles SIGINT for graceful shutdown
 * Input   : signum (signal number)
 * Outputs : Initiates cleanup and signals producers to stop
 * Returns : None
 */
void sigint_handler(int signum) {
    // Set cleanup mode 
    cleanup_mode = 1;
    
    // Send SIGINT to producer processes
    if (dp1_pid > 0) {
        kill(dp1_pid, SIGINT);
    }
    if (dp2_pid > 0) {
        kill(dp2_pid, SIGINT);
    }
}

/*
 * Name    : sigalrm_handler
 * Purpose : Triggered every 2 seconds to read data from buffer and update histogram
 * Input   : signum (signal number)
 * Outputs : Reads buffer, updates letter counts, optionally displays histogram
 * Returns : None
 */
void sigalrm_handler(int signum) {
    static int alarm_count = 0;
    alarm_count++;
    printf("SIGALRM triggered %d times\n", alarm_count);

    char buffer[READ_BATCH_SIZE];
    int num_read;
    
    // Acquire semaphore
    semaphore_wait(semid);
    
    // Read letters from the buffer
    num_read = bulk_read_from_buffer(shm, buffer, READ_BATCH_SIZE);
    
    // Release semaphore
    semaphore_signal(semid);
    
    // Update letter counts
    if (num_read > 0) {
        printf("Read %d letters from buffer.\n", num_read);
        printf("Letters read: ");
        for (int i = 0; i < num_read; i++) {
            printf("%c ", buffer[i]);
    
            if (buffer[i] >= MIN_LETTER && buffer[i] <= MAX_LETTER) {
                int index = buffer[i] - MIN_LETTER;
                letter_counts[index]++;
            }
        }
        printf("\n");
    }
    
    
    // Update histogram timer
    time_t current_time = time(NULL);
    printf("Time since last histogram: %ld seconds\n", current_time - last_histogram_time);

    if (current_time - last_histogram_time >= 10 || cleanup_mode) {
        printf("Displaying histogram...\n");
        display_histogram();
        last_histogram_time = current_time;
        usleep(5000);
    }

    
    // If in cleanup mode and no more data to read, exit
    if (cleanup_mode && num_read == 0) {
        running = 0;
        return;
    }
    
    // Set alarm for next read
    alarm(2);
}

/*
 * Name    : display_histogram
 * Purpose : Displays the histogram based on letter_counts[]
 * Input   : None
 * Outputs : Printed histogram on terminal
 * Returns : None
 */
void display_histogram() {
    // Clear screen
    printf("\033[2J\033[H");
    
    // Display histogram for each letter
    for (int i = 0; i < LETTER_RANGE; i++) {
        char letter = MIN_LETTER + i;
        int count = letter_counts[i];
        
        // Display letter and count
        printf("%c-%03d ", letter, count);
        
        // Display histogram bars
        int hundreds = count / 100;
        int tens = (count % 100) / 10;
        int ones = count % 10;
        
        // Display hundreds as '*' 
        for (int h = 0; h < hundreds; h++) {
            printf("*");
        }
        
        // Display tens as '+'
        for (int t = 0; t < tens; t++) {
            printf("+");
        }
        
        // Display ones as '-'
        for (int o = 0; o < ones; o++) {
            printf("-");
        }
        
        printf("\n");
    }
    
    fflush(stdout);
}

/*
 * Name    : cleanup_and_exit
 * Purpose : Final display and detach shared memory on shutdown
 * Input   : None
 * Outputs : Displays final histogram, prints "Shazam !!"
 * Returns : None
 */
void cleanup_and_exit() {
    printf("Final histogram before shutdown:\n");
    // Display final histogram
    display_histogram();

    // Exit message 
    printf("Shazam !!\n");
    
    fflush(stdout);

    // Clean up IPC resources if we're the last to use them 
    detach_shared_memory(shm);
}

/*
 * Name    : main
 * Purpose : Entry point for DC process
 * Input   : Command-line arguments <shmid> <dp1_pid> <dp2_pid>
 * Outputs : Attaches to IPC, sets alarms, loops until SIGINT
 * Returns : EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, 0);

    // Check arguments 
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <shmid> <dp1_pid> <dp2_pid>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    // Get command line arguments 
    shmid = atoi(argv[1]);
    dp1_pid = atoi(argv[2]);
    dp2_pid = atoi(argv[3]);
    
    // Verify arguments 
    if (shmid <= 0 || dp1_pid <= 0 || dp2_pid <= 0) {
        fprintf(stderr, "Invalid arguments\n");
        return EXIT_FAILURE;
    }
    
    // Attach to shared memory 
    if (attach_shared_memory(shmid, &shm) != 0) {
        fprintf(stderr, "Failed to attach to shared memory\n");
        return EXIT_FAILURE;
    }
    
    // Attach semaphore 
    semid = semget(SEM_KEY, 1, 0);
    if (semid == -1) {
        fprintf(stderr, "Failed to attach semaphore\n");
        detach_shared_memory(shm);
        return EXIT_FAILURE;
    }

    last_histogram_time = time(NULL);
    
    // Set up signal handlers
      struct sigaction sa;

      // SIGALRM
      memset(&sa, 0, sizeof(sa));
      sa.sa_handler = sigalrm_handler;
      sa.sa_flags = SA_RESTART;
      sigemptyset(&sa.sa_mask);
      if (sigaction(SIGALRM, &sa, NULL) == -1) {
          perror("sigaction(SIGALRM)");
          return EXIT_FAILURE;
      }
  
      // SIGINT
      if (signal(SIGINT, sigint_handler) == SIG_ERR) {
          perror("signal(SIGINT)");
          return EXIT_FAILURE;
      }
  
    
    // Start the 2-second alarm for reading data
    alarm(2);
    printf("DC: Setup complete, waiting for alarms...\n");
    
    // Main loop */
    while (running) {
        pause();  // Wait for signals
    }
    
    // Clean up and exit
    cleanup_and_exit();
    
    return EXIT_SUCCESS;
}