/*
 * FILE: common.h
 * PROJECT: HISTOGRAM-SYSTEM
 * PROGRAMMER: Manreet Thind
 * FIRST VERSION: 08-04-2025
 * DESCRIPTION:
 * This file contains global constants and utility functions that are common to all components.
 * It defines valid letter boundaries and includes a helper function to generate random letters.
 */
#ifndef COMMON_H
#define COMMON_H

/* Constants */
#define MIN_LETTER 'A'
#define MAX_LETTER 'T'
#define LETTER_RANGE (MAX_LETTER - MIN_LETTER + 1)

/* Random letter generation function */
char generate_random_letter(void);

#endif /* COMMON_H */