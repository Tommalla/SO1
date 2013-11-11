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
int res[100000];

int main(const int argc, const char** argv) {
	int n, qty, num, s, t, u, m;
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
	int pipe_dsc[2][2];
	int i;

	if (pipe(pipe_dsc[1]) == -1)
		syserr("Error in pipe for manager\n");

	if (dup2(pipe_dsc[1][1], STDOUT_FILENO) == -1)
		syserr("Error in dup stdout for manager\n");

	for (i = 0; i < n; ++i) {
		pipe_dsc[0][0] = pipe_dsc[1][0];
		pipe_dsc[0][1] = pipe_dsc[1][1];

		if (pipe(pipe_dsc[1]) == -1)
			syserr("Error in pipe for executor #%d\n", i);

		switch (fork()) {
			case -1:
				syserr("Error in fork for executor #%d\n", i);
				break;
			case 0:
				if (dup2(pipe_dsc[0][0], STDIN_FILENO) == -1)
					syserr("Error in dup stdin for executor %d\n", i);

				if (dup2(pipe_dsc[1][1], STDOUT_FILENO) == -1)
					syserr("Error in dup stdout for executor %d\n", i);

				execl("executor", "executor", NULL);
				syserr("Error in execl for executor #%d\n", i);
				break;
		}
	}

	if (dup2(pipe_dsc[1][0], STDIN_FILENO) == -1)
		syserr("Error in dup stdin for manager\n");

	fscanf(inFile, "%d\n", &num);
	qty = 0;
	fprintf(stderr, "num: %d\n", num);

	for (i = 0; i < num || qty > 0;) {
		while (qty >= n || (i >= num && qty > 0)) {	//already n data chunks in the ring
			readInput(input);
			s = strlen(input);

			//if a chunk is finished
			m = 1;
			u = 0;
			for (t = s - 2; t >= 0; --t)
				if (input[t] < '0' || input[t] > '9') {
					if ((input[t] == ' ' && input[t - 1] == ':') || input[t] == '-') {
						input[t-1] = '\0';
						res[atoi(input)] = u * (input[t] == '-' ? -1 : 1);
						--qty;
						fprintf(stderr, "Written!\n");
					} else
						write(STDOUT_FILENO, input, s);

					break;
				} else {
					u += (input[t] - '0') * m;
					m *= 10;
				}
		}

		if (i < num) {
			fgets(input, INPUT_SIZE, inFile);
			sprintf(output, "%d: %s\n", i, input);
			write(STDOUT_FILENO, output, strlen(output) - 1);
			++qty;
		}

		if (i < num)
			++i;
	}

	write(STDOUT_FILENO, "#\n", 2);

	for (i = 0; i < num; ++i)
		fprintf(outFile, "%d: %d\n", i + 1, res[i]);

	//close everything
	fclose(inFile);
	fclose(outFile);

	for (i = 0; i < n; ++i)
		wait(0);
	return 0;
}