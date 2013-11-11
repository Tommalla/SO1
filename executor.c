/* Tomasz Zakrzewski, tz336079
 * Pierwsze zadanie zaliczeniowe z SO
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "err.h"
#include "common.h"

char input[INPUT_SIZE];
char output[INPUT_SIZE];

int main() {
	while (1) {
		readInput(input);
		if (input[0] == '#') {
			if (write(STDOUT_FILENO, "#\n", 2) == -1)
				syserr("Error while passing the kill command [executor]\n");
			return 0;
		}

		if (debug)
			fprintf(stderr, "Executor: Read: %s\n", input);

		calc(input, output);
		if (write(STDOUT_FILENO, output, strlen(output)) == -1)
			syserr("Error while passing the results through pipe [executor]\n");
	}

	close (STDIN_FILENO);

	return 0;
}