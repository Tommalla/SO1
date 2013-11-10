/* Tomasz Zakrzewski, tz336079
 * Pierwsze zadanie zaliczeniowe z SO
 */
#include "common.h"
#include <stdio.h>
#include <unistd.h>

char input[INPUT_SIZE];
char output[INPUT_SIZE];

int main() {
	while (1) {
		//TODO: fix this to readInput
		if (input[0] == '#') {
			puts("#");
			return 0;
		}

		calc(input, output);
		puts(output);
	}

	close (STDIN_FILENO);

	return 0;
}