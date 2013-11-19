/* Tomasz Zakrzewski, tz336079
 * SO2013 - First project
 */
#ifndef COMMON_H
#define COMMON_H

#define INPUT_SIZE 32768


#ifdef DEBUG
static const char debug = 1;
#else
static const char debug = 0;
#endif

/**
 * Reads the input and saves the result in output.
 * This method assumes that output is large enough to hold the string.
 */
void readInput(char* output);

/**
 * Performs one calculation and writes the result to output
 */
void calc(const char* input, char* output);

/**
 * Returns 1 if c is a digit and 0 otherwise
 */
int isDigit(const char c);

#endif