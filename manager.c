/* Tomasz Zakrzewski, tz336079
 * Pierwsze zadanie zaliczeniowe z SO
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
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
	int pipe_dsc[2][2];
	int i;

	if (argc != 4)
		syserr("Wrong number of arguments!\n%s", usage);

	n = atoi(argv[1]);

	strcpy(input, "DATA/");
	if ((inFile = fopen(strcat(input, argv[2]), "r")) == NULL)
		syserr("Error opening input file %s!\n", input);

	//opening the outFile here, even though we're writing at the end because if
	//it's impossible to open the file, it should fail before the calculations
	strcpy(input, "DATA/");
	if ((outFile = fopen(strcat(input, argv[3]), "w")) == NULL)
		syserr("Error opening output file %s\n", input);


	//create the ring

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

	if (fscanf(inFile, "%d\n", &num) == 0)
		syserr("Error while reading number of expressions from the input file!\n");
	qty = 0;

	if (debug)
		fprintf(stderr, "Number of expressions to calculate: %d\n", num);

	for (i = 0; i < num || qty > 0;) {
		while (qty >= n || (i >= num && qty > 0)) {	//already n data chunks in the ring
			//hang up on reading (wait for the next chunk)
			readInput(input);
			s = strlen(input);

			//analyze the input data:
			for (t = s - 2, m = 1, u = 0; t >= 0; --t)
				if (input[t] < '0' || input[t] > '9') {
					if ((input[t] == ' ' && input[t - 1] == ':') || input[t] == '-') {
						//if a chunk is finished
						input[t-1] = '\0';
						m = atoi(input);
						res[m] = u * (input[t] == '-' ? -1 : 1);	//save the result
						--qty;
						if (debug)
							fprintf(stderr, "Manager: Saved the result of %d expr: %d\n", m, res[m]);
					} else
						if (write(STDOUT_FILENO, input, s) == -1)
							syserr("Error while writing to the first pipe [manager]!\n");

					break;
				} else {
					u += (input[t] - '0') * m;
					m *= 10;
				}
		}

		if (i < num) {
			if (fgets(input, INPUT_SIZE, inFile) == NULL)
				syserr("Error while reading line %d [manager]\n", i);
			sprintf(output, "%d: %s\n", i, input);	//FIXME remove the space
			if (write(STDOUT_FILENO, output, strlen(output) - 1) == -1)
				syserr("Error while writing the line %d to the pipe [manager]\n", i);
			++qty;
		}

		if (i < num)
			++i;
	}

	//tell the ring to close
	if (write(STDOUT_FILENO, "#\n", 2) == -1)
		syserr("Error while writing kill command to the pipe [manager]\n");

	//write the results to file
	for (i = 0; i < num; ++i)
		fprintf(outFile, "%d: %d\n", i + 1, res[i]);

	//close everything, wait for the children to end etc
	fclose(inFile);
	fclose(outFile);

	for (i = 0; i < n; ++i)
		wait(0);

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	return 0;
}