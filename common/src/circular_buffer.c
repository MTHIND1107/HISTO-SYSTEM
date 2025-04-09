/*
 * FILE: circular_buffer.c
 * PROJECT: HISTOGRAM-SYSTEM
 * PROGRAMMER: Manreet Thind
 * FIRST VERSION: 08-04-2025
 * DESCRIPTION:
 * This file implements the logic for interacting with a circular buffer
 * stored in shared memory. It supports both single and bulk read/write
 * operations while managing buffer space and avoiding overflow.
 */
#include "../inc/circular_buffer.h"

/*
 * Name    : get_available_space
 * Purpose : Calculate how much space is left in the circular buffer
 * Input   : Pointer to shared memory buffer
 * Outputs : None
 * Returns : Number of available spaces
 */
int get_available_space(shared_memory_t *shm) {
    int read_idx = shm->read_index;
    int write_idx = shm->write_index;
    
    if (read_idx <= write_idx) {
        /* Read index is before or at write index */
        return BUFFER_SIZE - (write_idx - read_idx) - 1;
    } else {
        /* Write index has wrapped around */
        return read_idx - write_idx - 1;
    }
}
/*
* Name    : write_to_buffer
* Purpose : Write a single character to the buffer if space is available
* Input   : Pointer to shared memory, letter to write
* Outputs : Updated buffer
* Returns : 1 if success, 0 if buffer is full
*/
int write_to_buffer(shared_memory_t *shm, char letter) {
    int next_write = (shm->write_index + 1) % BUFFER_SIZE;
    
    /* Check if buffer is full */
    if (next_write == shm->read_index) {
        return 0;  /* Buffer full */
    }
    
    /* Write letter and update write index */
    shm->buffer[shm->write_index] = letter;
    shm->write_index = next_write;
    
    return 1;
}


/*
 * Name    : read_from_buffer
 * Purpose : Read a single character from the buffer if data is available
 * Input   : Pointer to shared memory, pointer to output letter
 * Outputs : The character read
 * Returns : 1 if success, 0 if buffer is empty
 */
int read_from_buffer(shared_memory_t *shm, char *letter) {
    /* Check if buffer is empty */
    if (shm->read_index == shm->write_index) {
        return 0;  /* Buffer empty */
    }
    
    /* Read letter and update read index */
    *letter = shm->buffer[shm->read_index];
    shm->read_index = (shm->read_index + 1) % BUFFER_SIZE;
    
    return 1;
}

/*
 * Name    : bulk_write_to_buffer
 * Purpose : Write multiple letters into the buffer
 * Input   : Pointer to shared memory, letter array, number of letters
 * Outputs : Updated buffer
 * Returns : Number of letters actually written
 */
int bulk_write_to_buffer(shared_memory_t *shm, char *letters, int count) {
    int available = get_available_space(shm);
    int to_write = (count <= available) ? count : available;
    int written = 0;
    
    for (int i = 0; i < to_write; i++) {
        if (write_to_buffer(shm, letters[i])) {
            written++;
        } else {
            break;
        }
    }
    
    return written;
}


/*
 * Name    : bulk_read_from_buffer
 * Purpose : Read multiple letters from the buffer
 * Input   : Pointer to shared memory, letter array, max number to read
 * Outputs : Letter array filled with read data
 * Returns : Number of letters read
 */
int bulk_read_from_buffer(shared_memory_t *shm, char *letters, int count) {
    int read_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (read_from_buffer(shm, &letters[i])) {
            read_count++;
        } else {
            break;
        }
    }
    
    return read_count;
}