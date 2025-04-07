#ifndef COMMON_H
#define COMMON_H

/* Constants */
#define MIN_LETTER 'A'
#define MAX_LETTER 'T'
#define LETTER_RANGE (MAX_LETTER - MIN_LETTER + 1)

/* Random letter generation function */
char generate_random_letter(void);

#endif /* COMMON_H */