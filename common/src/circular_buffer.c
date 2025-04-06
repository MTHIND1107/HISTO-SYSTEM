#include "../inc/circular_buffer.h"

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