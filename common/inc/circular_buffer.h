/*
 * FILE: circular_buffer.h
 * PROJECT: HISTOGRAM-SYSTEM
 * PROGRAMMER: Manreet Thind
 * FIRST VERSION: 08-04-2025
 * DESCRIPTION:
 * This header file declares the functions used to manage a circular buffer within the shared memory.
 * It includes functionality for writing and reading single or multiple characters while respecting
 * the buffer boundaries and synchronization with semaphores.
 */
#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include "shared_memory.h"

/* Functions */
int get_available_space(shared_memory_t *shm);
int write_to_buffer(shared_memory_t *shm, char letter);
int read_from_buffer(shared_memory_t *shm, char *letter);
int bulk_write_to_buffer(shared_memory_t *shm, char *letters, int count);
int bulk_read_from_buffer(shared_memory_t *shm, char *letters, int count);

#endif /* CIRCULAR_BUFFER_H */