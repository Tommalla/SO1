/* Tomasz Zakrzewski, tz336079
 * Pierwsze zadanie zaliczeniowe z SO
 */
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "err.h"

const char* usage = "Usage: manager <number of processes> <input file> <output file>";

char input[INPUT_SIZE];
char output[INPUT_SIZE];

int main(const int argc, const char** argv) {
	if (argc != 4)
		syserr("Not enough arguments!\n%s", usage);

	int n = atoi(argv[1]);

	FILE* inFile;
	FILE* outFile;

	if (inFile = fopen(argv[2], "r") == NULL)
		syserr("Error opening input file DATA/%s!\n", argv[2]);
	if (outFile = fopen(argv[3], "w") == NULL)
		syserr("Error opening output file DATA/%s\n", argv[3]);

	//TODO create the ring
	//TODO read the file line by line and calculate stuff

	//close everything
	fclose(inFile);
	fclose(outFile);
	return 0;
}