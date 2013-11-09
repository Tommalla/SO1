/* Tomasz Zakrzewski, tz336079
 * Pierwsze zadanie zaliczeniowe z SO
 */
#ifndef COMMON_H
#define COMMON_H

#define INPUT_SIZE 32768

/**
 * Reads the input and saves the result in output.
 * This method assumes that output is large enough to hold the string.
 */
void readInput(char* output);

/**
 * Performs one calculation and writes the result to output
 */
void calc(const char* input, char* output);

#endif