/*
 * FILE: dp1.c
 * PROJECT: HISTO-SYSTEM
 * PROGRAMMER: Manreet Thind
 * FIRST VERSION: 08-04-2025
 * DESCRIPTION:
 * This file contains the main logic for DP-1 (Data Producer 1). It is responsible for creating and initializing
 * shared memory and semaphores. DP-1 generates 20 random letters every 2 seconds and writes them to the shared
 * circular buffer. It also forks and launches the DP-2 process and handles cleanup on SIGINT.
 */
#include "../inc/dp1.h"

#include "../../common/inc/shared_memory.h"
#include "../../common/inc/semaphore_utils.h"
#include "../../common/inc/circular_buffer.h"
#include "../../common/inc/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>

#define PATH_MAX 4096

// Global variables
int running = 1;
int shmid = -1;
int semid = -1;
shared_memory_t *shm = NULL;

/*
 * Name    : sigint_handler
 * Purpose : Signal handler to stop DP-1 on SIGINT
 * Input   : Signal number (int)
 * Outputs : Sets the global running flag to 0
 * Returns : None
 */
void sigint_handler(int signum) {
    running = 0;
}

/*
 * Name    : generate_and_write_letters
 * Purpose : Generates and writes 20 random letters to shared memory
 * Input   : None
 * Outputs : Writes to the circular buffer in shared memory
 * Returns : None
 */
void generate_and_write_letters() {
    char letters[20];
    
    // Generate 20 random letters
    for (int i = 0; i < 20; i++) {
        letters[i] = generate_random_letter();
    }
    
    // Acquire semaphore
    semaphore_wait(semid);
    
    // Write letters to buffer
    (void)bulk_write_to_buffer(shm, letters, 20); //(void) silences unused warnings
    
    // Release semaphore 
    semaphore_signal(semid);
}

/*
 * Name    : main
 * Purpose : Entry point of DP-1, sets up IPC, launches DP-2, and loops writing letters
 * Input   : None
 * Outputs : Launches DP-2 and writes to shared memory
 * Returns : EXIT_SUCCESS on clean exit, EXIT_FAILURE on error
 */
int main() {
    pid_t dp2_pid;
    char shmid_str[16];
    char path[PATH_MAX];
    
    // Set up signal handler
    signal(SIGINT, sigint_handler);
    
    // Create shared memory
    shmid = create_shared_memory();
    if (shmid == -1) {
        fprintf(stderr, "Failed to create shared memory\n");
        return EXIT_FAILURE;
    }
    
    // Attach to shared memory
    if (attach_shared_memory(shmid, &shm) != 0) {
        fprintf(stderr, "Failed to attach to shared memory\n");
        remove_shared_memory(shmid);
        return EXIT_FAILURE;
    }
    
    // Initialize shared memory
    init_shared_memory(shm);
    
    // Create semaphore  (initialize once)
    semid = semget(0x1234, 1, IPC_CREAT | 0666); //Fixed key permissions
    if (semid == -1) {
        perror("DP-1: Segmet failed");
        return EXIT_FAILURE;
    }
    
    // Convert shmid to string for passing to DP-2
    snprintf(path, sizeof(path), "%s/DP-2/bin/DP-2", getenv("PWD"));
    snprintf(shmid_str, sizeof(shmid_str), "%d", shmid);
    
    // Fork DP-2 process
    dp2_pid = fork();
    if (dp2_pid < 0) {
        // Fork failed
        perror("fork");
        detach_shared_memory(shm);
        remove_shared_memory(shmid);
        remove_semaphore(semid);
        return EXIT_FAILURE;
    } else if (dp2_pid == 0) {       
        execl(path, "DP-2", shmid_str, NULL);
        // If exec fails */
        perror("execl");
        exit(EXIT_FAILURE);
    }
    
    // Parent process (DP-1) continues here
    // Main loop */
    while (running) {
        // Generate and write letters
        generate_and_write_letters();
        
        // Sleep for 2 seconds
        sleep(2);
    }
    
    // Clean up */
    detach_shared_memory(shm);
    
    // Wait for child to terminate
    waitpid(dp2_pid, NULL, 0);
    
    return EXIT_SUCCESS;
}