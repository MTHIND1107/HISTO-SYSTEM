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