/*
 * FILE: common.c
 * PROJECT: HISTOGRAM-SYSTEM
 * PROGRAMMER: Manreet Thind
 * FIRST VERSION: 08-04-2025
 * DESCRIPTION:
 * Contains utility functions used across the system.
 * Currently includes random letter generation between 'A' and 'T'.
 */
#include "../inc/common.h"
#include <stdlib.h>
#include <time.h>

/*
 * Name    : init_random
 * Purpose : Seed the random number generator using current time
 * Input   : None
 * Outputs : Seeds rand()
 * Returns : None
 */
void init_random(void) {
    srand(time(NULL));
}

/*
 * Name    : generate_random_letter
 * Purpose : Generate a random uppercase letter from A to T
 * Input   : None
 * Outputs : None
 * Returns : Random character between 'A' and 'T'
 */
char generate_random_letter(void) {
    return 'A' + (rand() % ('T' - 'A' + 1));
}