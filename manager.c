/* Tomasz Zakrzewski, tz336079
 * Pierwsze zadanie zaliczeniowe z SO
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "common.h"
#include "err.h"

const char* usage = "Usage: manager <number of processes> <input file> <output file>";

char input[INPUT_SIZE];
char output[INPUT_SIZE];

int main(const int argc, const char** argv) {
	int n;
	FILE* inFile;
	FILE* outFile;

	if (argc != 4)
		syserr("Wrong number of arguments!\n%s", usage);

	n = atoi(argv[1]);

	strcpy(input, "DATA/");
	if ((inFile = fopen(strcat(input, argv[2]), "r")) == NULL)
		syserr("Error opening input file %s!\n", input);

	strcpy(input, "DATA/");
	if ((outFile = fopen(strcat(input, argv[3]), "w")) == NULL)
		syserr("Error opening output file %s\n", input);


	//TODO create the ring
	int pipe_dsc[2];
	int i;

	if (pipe(pipe_dsc) == -1)
		syserr("Error in pipe for manager\n");

	if (dup2(pipe_dsc[0], STDOUT_FILENO) == -1)
		syserr("Error in dup stdout for manager\n");


	for (i = 0; i < n; ++i) {
		switch (fork()) {
			case -1:
				syserr("Error in fork for executor #%d\n", i);
				break;
			case 0:
				if (dup2(pipe_dsc[1], STDIN_FILENO) == -1)
					syserr("Error in dup stdin for executor %d\n", i);

				if (pipe(pipe_dsc) == -1)
					syserr("Error in pipe for executor #%d\n", i);

				if (dup2(pipe_dsc[0], STDOUT_FILENO) == -1)
					syserr("Error in dup stdout for executor %d\n", i);

				execl("executor", "executor", NULL);
				syserr("Error in execl for executor #%d\n", i);
				break;
		}
	}

	if (dup2(pipe_dsc[1], STDIN_FILENO) == -1)
		syserr("Error in dup stdin for manager\n");
	//TODO read the file line by line and calculate stuff

	//close everything
	fclose(inFile);
	fclose(outFile);

	//for (i = 0; i < n; ++i)
	//	wait(0);
	return 0;
}