/* Tomasz Zakrzewski, tz336079
 * Pierwsze zadanie zaliczeniowe z SO
 */
#include "common.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

char input[INPUT_SIZE];
char output[INPUT_SIZE];

int main() {
	while (1) {
		readInput(input);
		if (input[0] == '#') {
			while (write(STDOUT_FILENO, "#\n", 2) == -1);
			return 0;
		}

		fprintf(stderr, "Read: %s\n", input);
		calc(input, output);
		write(STDOUT_FILENO, output, strlen(output));
	}

	close (STDIN_FILENO);

	return 0;
}