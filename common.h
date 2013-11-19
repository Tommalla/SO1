/* Tomasz Zakrzewski, tz336079
 * Pierwsze zadanie zaliczeniowe z SO
 */
#ifndef COMMON_H
#define COMMON_H
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>

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

int isDigit(const char c);

static inline void countfd() {
	struct rlimit rlim;
	int count, i, res;
	res = 0;
	//count = getrlimit(RLIMIT_NOFILE, &rlim);
	for (i = 0; i < 30; ++i)
		if (fcntl(i, F_GETFD) != -1) {
			++res;
			fprintf(stderr, "Open: %d\n", i);
		}

		fprintf(stderr, "Open descriptors: %d\n", res);
}

#endif