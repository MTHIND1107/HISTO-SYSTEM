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

int main() {
    pid_t dp2_pid;
    char shmid_str[16];
    
    /* Set up signal handler */
    signal(SIGINT, sigint_handler);
    
    /* Create shared memory */
    shmid = create_shared_memory();
    if (shmid == -1) {
        fprintf(stderr, "Failed to create shared memory\n");
        return EXIT_FAILURE;
    }
    
    /* Attach to shared memory */
    if (attach_shared_memory(shmid, &shm) != 0) {
        fprintf(stderr, "Failed to attach to shared memory\n");
        remove_shared_memory(shmid);
        return EXIT_FAILURE;
    }
    
    /* Initialize shared memory */
    init_shared_memory(shm);
    
    /* Create semaphore */
    semid = create_semaphore();
    if (semid == -1) {
        fprintf(stderr, "Failed to create semaphore\n");
        detach_shared_memory(shm);
        remove_shared_memory(shmid);
        return EXIT_FAILURE;
    }
    
    /* Convert shmid to string for passing to DP-2 */
    snprintf(shmid_str, sizeof(shmid_str), "%d", shmid);
    
    /* Fork DP-2 process */
    dp2_pid = fork();
    if (dp2_pid < 0) {
        /* Fork failed */
        perror("fork");
        detach_shared_memory(shm);
        remove_shared_memory(shmid);
        remove_semaphore(semid);
        return EXIT_FAILURE;
    } else if (dp2_pid == 0) {
        /* Child process (DP-2) */
        execl("../DP-2/bin/DP-2", "DP-2", shmid_str, NULL);
        
        /* If exec fails */
        perror("execl");
        exit(EXIT_FAILURE);
    }
    
    /* Parent process (DP-1) continues here */
    
    /* Main loop */
    while (running) {
        /* Generate and write letters */
        generate_and_write_letters();
        
        /* Sleep for 2 seconds */
        sleep(2);
    }
    
    /* Clean up */
    detach_shared_memory(shm);
    
    /* Wait for child to terminate */
    waitpid(dp2_pid, NULL, 0);
    
    return EXIT_SUCCESS;
}