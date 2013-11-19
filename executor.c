/* Tomasz Zakrzewski, tz336079
 * SO2013 - First project
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "err.h"
#include "common.h"

char input[INPUT_SIZE];
char output[INPUT_SIZE];

int main() {
	while (1) {
		//read the input or hang on pipe waiting
		readInput(input);
		//if it's time to end
		if (input[0] == '#') {
			if (write(STDOUT_FILENO, "#\n", 2) == -1)
				syserr("Error while passing the kill command [executor %d]\n", getpid());
			break;
		}

		if (debug)
			fprintf(stderr, "Executor %d: Read: %s\n", getpid(), input);

		//parse, calculate, pass
		calc(input, output);
		if (write(STDOUT_FILENO, output, strlen(output)) == -1)
			syserr("Error while passing the results through pipe [executor %d]\n", getpid());
	}

	return 0;
}