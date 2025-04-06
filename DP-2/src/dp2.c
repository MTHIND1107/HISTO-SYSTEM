#include "../inc/dp2.h"
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

int main(int argc, char *argv[]) {
    pid_t dc_pid;
    pid_t parent_pid = getppid();  /* DP-1's PID */
    pid_t my_pid = getpid();       /* DP-2's PID */
    char shmid_str[16];
    char dp1_pid_str[16];
    char dp2_pid_str[16];
    
    /* Set up signal handler */
    signal(SIGINT, sigint_handler);
    
    /* Check arguments */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <shmid>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    /* Get shared memory ID from command line */
    shmid = atoi(argv[1]);
    if (shmid <= 0) {
        fprintf(stderr, "Invalid shared memory ID\n");
        return EXIT_FAILURE;
    }
    
    /* Create semaphore */
    semid = create_semaphore();
    if (semid == -1) {
        fprintf(stderr, "Failed to create semaphore\n");
        return EXIT_FAILURE;
    }
    
    /* Convert IDs to strings for passing to DC */
    snprintf(shmid_str, sizeof(shmid_str), "%d", shmid);
    snprintf(dp1_pid_str, sizeof(dp1_pid_str), "%d", parent_pid);
    snprintf(dp2_pid_str, sizeof(dp2_pid_str), "%d", my_pid);
    
    /* Fork DC process */
    dc_pid = fork();
    if (dc_pid < 0) {
        /* Fork failed */
        perror("fork");
        return EXIT_FAILURE;
    } else if (dc_pid == 0) {
        /* Child process (DC) */
        execl("../DC/bin/DC", "DC", shmid_str, dp1_pid_str, dp2_pid_str, NULL);
        
        /* If exec fails */
        perror("execl");
        exit(EXIT_FAILURE);
    }
    
    /* Attach to shared memory */
    if (attach_shared_memory(shmid, &shm) != 0) {
        fprintf(stderr, "Failed to attach to shared memory\n");
        kill(dc_pid, SIGINT);
        waitpid(dc_pid, NULL, 0);
        return EXIT_FAILURE;
    }
    
    /* Main loop */
    while (running) {
        char letter = generate_random_letter();
        
        /* Acquire semaphore */
        semaphore_wait(semid);
        
        /* Write single letter to buffer */
        write_to_buffer(shm, letter);
        
        /* Release semaphore */
        semaphore_signal(semid);
        
        /* Sleep for 1/20 of a second (50 ms) */
        usleep(50000);
    }
    
    /* Clean up */
    detach_shared_memory(shm);
    
    /* Wait for child to terminate */
    waitpid(dc_pid, NULL, 0);
    
    return EXIT_SUCCESS;
}