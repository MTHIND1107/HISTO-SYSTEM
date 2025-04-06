#include "../inc/common.h"
#include <stdlib.h>
#include <time.h>

/* Initialize random number generator - call this in main */
__attribute__((constructor))
static void init_random() {
    srandom(time(NULL));
}