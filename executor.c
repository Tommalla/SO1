/* Tomasz Zakrzewski, tz336079
 * Pierwsze zadanie zaliczeniowe z SO
 */
#include "common.h"
#include <stdio.h>

char input[INPUT_SIZE];
char output[INPUT_SIZE];

int main() {
	while (1) {
		gets(input);
		if (input[0] == '#') {
			puts("#");
			return 0;
		}

		calc(input, output);
		puts(output);
	}

	return 0;
}