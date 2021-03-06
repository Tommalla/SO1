/* Tomasz Zakrzewski, tz336079
 * SO2013 - First project
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "common.h"
#include "err.h"

void reread(char* buffer, size_t* size, int* id) {
	*size = read(STDIN_FILENO, buffer, BUFSIZ);
	if (*size == -1)
		syserr("Error on reading\n");
	if (*size == 0)
		syserr("Error on reading - one of the processes was closed prematurely\n");
	*id = 0;
}

void readInput(char* output) {
	static char buffer[BUFSIZ];
	static int id = 0;
	static size_t size = 0;

	int outId;

	//go through buffer (or read if it's the end of the buffer)
	//until you find '\n'
	outId = 0;
	do {
		++id;
		if (id >= size)
			reread(buffer, &size, &id);

		output[outId++] = buffer[id];

	} while (buffer[id] != '\n');

	output[outId] = '\0';	//null-terminate
}

void calc(const char* input, char* output) {
	char buffer[BUFSIZ];
	int n, i, j, k, m, mul;
	int args[2];

	n = strlen(input);

	if (debug)
		fprintf(stderr, "Analyzing: %s[length=%d]\n", input, n);

	for (i = 0; i < n - 1; ++i)	//find the first operator
		if (!isDigit(input[i]) && input[i] != ' ' && input[i] != ':' &&
			!(input[i] == '-' && i + 1 < n - 1 && input[i + 1] != ' '))
			break;

	if (i == n - 1) {	//no operators left
		if (debug)
			fprintf(stderr, "No operators found! i = %d, n - 1 = %d\n", i, n - 1);
		strcpy(output, input);
		return;
	}

	m = i;

	if (debug)
		fprintf(stderr, "First operator (%c) at: %d\n", input[m], m);

	//copy everything before and including ':' and one space after
	for (j = 0; j < n && input[j] != ':'; ++j)
		output[j] = input[j];

	for (k = 0; k < 2; ++k, ++j)
		output[j] = input[j];
	//now j points to the beginning of the expression

	for (k = 1; k >= 0; --k) {
		args[k] = 0;
		for (; i >= 0 && !isDigit(input[i]); --i);	//iterate to the first digit

		for (mul = 1; i >= 0 && isDigit(input[i]); --i) {	//through all the digits
			args[k] += (input[i] - '0') * mul;
			mul *= 10;
		}

		if (input[i] == '-') {
			args[k] *= -1;
			--i;
		}
	}

	++i;
	//i now points to the beginning of the first argument

	switch (input[m]) {
		case '+':
			k = args[0] + args[1];
			break;
		case '-':
			k = args[0] - args[1];
			break;
		case '*':
			k = args[0] * args[1];
			break;
		case '/':
			k = args[0] / args[1];
			break;
	}

	if (debug)
		fprintf(stderr, "i = %d j = %d k = %d m = %d, operands: %d %d\n", i, j, k, m, args[0], args[1]);

	sprintf(buffer, "%d", k);
	k = strlen(buffer);
	//the operation result is now in buffer

	if (debug) {
		fprintf(stderr, "result: %s\n", buffer);
		fprintf(stderr, "%d %d %d\n", i, j, m);
	}

	//copy everything between ':' and the beginning of the result
	memcpy(output + j, input + j, i - j);
	//copy the result
	memcpy(output + i, buffer, k);
	//copy the rest (with '\0' at the end!)
	memcpy(output + i + k, input + m + 1, strlen(input) - m);

	if (debug)
		fprintf(stderr, "output: %s\n", output);
}

int isDigit(const char c) {
	return '0' <= c && c <= '9';
}

