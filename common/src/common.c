#include "../inc/common.h"
#include <stdlib.h>
#include <time.h>

/* Initialize random number generator - call this in main */
__attribute__((constructor))
static void init_random(void) {
    srand(time(NULL));
}

char generate_random_letter(void) {
    return 'A' + (rand() % ('T' - 'A' + 1));
}