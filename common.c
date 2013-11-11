#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "common.h"

void reread(char* buffer, size_t* size, int* id) {
	*size = read(STDIN_FILENO, buffer, BUFSIZ);
	*id = 0;
}

void readInput(char* output) {
	static char buffer[BUFSIZ];
	static int id = 0;
	static size_t size = 0;

	int outId;

	outId = 0;
	do {
		++id;
		if (id >= size)
			reread(buffer, &size, &id);

		output[outId++] = buffer[id];

	} while (buffer[id] != '\n');

	output[outId] = '\0';
}

void calc(const char* input, char* output) {
	char buffer[BUFSIZ];
	int n, i, j, k, m, mul;
	int args[2];
	char op;

	n = strlen(input);

	for (i = 0; i < n - 1; ++i)	//find the first operator
		if (('0' > input[i] || input[i] > '9') && input[i] != ' ' && input[i] != ':' &&
			!(input[i] == '-' && i + 1 < n && input[i + 1] != ' '))
			break;

	if (i == n - 1) {	//no operators left
		strcpy(output, input);
		return;
	}

	m = i;

	fprintf(stderr, "Analyzing: %s[length = %d]\n", input, n);
	fprintf(stderr, "First operator at: %d\n", i);

	//copy everything before and including ';'
	for (j = 0; j < n && input[j] != ':'; ++j)
		output[j] = input[j];

	output[j++] = input[j - 1]; //happy debugging
	output[j++] = input[j - 1];
	//now j points to the beginning of the expression

	for (k = 1; k >= 0; --k) {
		args[k] = 0;
		for (; i >= 0 && ('0' > input[i] || input[i] > '9'); --i);	//iterate to the first digit

		for (mul = 1; i >= 0 && ('0' <= input[i] && input[i] <= '9'); --i) {	//through all the digits
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

	fprintf(stderr, "%d %d %d %d, operands: %d %d\n", i, j, k, m, args[0], args[1]);

	sprintf(buffer, "%d", k);
	k = strlen(buffer);
	fprintf(stderr, "%c result: %s\n", input[m], buffer);
	//copy j - i, then buffer, then m + 1 - end

	fprintf(stderr, "%d %d %d\n", i, j, m);

	memcpy(output + j, input + j, i - j);
	memcpy(output + i, buffer, k);
	memcpy(output + i + k, input + m + 1, strlen(input) - m);
	fprintf(stderr, "output: %s\n", output);
}
