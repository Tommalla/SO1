/* Tomasz Zakrzewski, tz336079
 * SO2013 - First project
 */
#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "common.h"
#include "err.h"

const char* usage = "Usage: manager <number of processes> <input file> <output file>";

char input[INPUT_SIZE];
char output[INPUT_SIZE];
int res[1000000];	//logical assumption - there won't be more input lines

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

	//opening the outFile here, even though we're writing at the end, because if
	//it's impossible to open the file, it should fail before the calculations
	strcpy(input, "DATA/");
	if ((outFile = fopen(strcat(input, argv[3]), "w")) == NULL)
		syserr("Error opening output file %s\n", input);


	//create the ring

	if (pipe2(pipe_dsc[1], O_CLOEXEC) == -1)
		syserr("Error in pipe for manager\n");

	if (dup2(pipe_dsc[1][1], STDOUT_FILENO) == -1)
		syserr("Error in dup stdout for manager\n");

	for (i = 0; i < n; ++i) {
		//'rotate' pipes
		pipe_dsc[0][0] = pipe_dsc[1][0];
		pipe_dsc[0][1] = pipe_dsc[1][1];

		//create the next pipe
		if (pipe2(pipe_dsc[1], O_CLOEXEC) == -1)
			syserr("Error in pipe for executor #%d\n", i);

		//create the i-th executor
		switch (fork()) {
			case -1:
				syserr("Error in fork for executor #%d\n", i);
				break;
			case 0:
				fclose(inFile);
				fclose(outFile);
				if (dup2(pipe_dsc[0][0], STDIN_FILENO) == -1)
					syserr("Error in dup stdin for executor %d\n", i);

				if (dup2(pipe_dsc[1][1], STDOUT_FILENO) == -1)
					syserr("Error in dup stdout for executor %d\n", i);

				execl("executor", "executor", NULL);
				syserr("Error in execl for executor #%d\n", i);
				break;
		}


		//close pipes we won't be using anymore (already copied by the last child)
		close(pipe_dsc[0][0]);
		close(pipe_dsc[0][1]);
	}

	//connect the n-1 -th executor to manager's input
	if (dup2(pipe_dsc[1][0], STDIN_FILENO) == -1)
		syserr("Error in dup stdin for manager\n");

	//close remaining unused pipes
	close(pipe_dsc[1][1]);
	close(pipe_dsc[1][0]);

	//start reading the input
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
				if (!isDigit(input[t])) {
					if ((input[t] == ' ' && input[t - 1] == ':') || (input[t] == '-' &&
						isDigit(input[t + 1])) ) {
						//if a chunk is finished
						input[t-1] = '\0';
						m = atoi(input);
						res[m] = u * (input[t] == '-' ? -1 : 1);	//save the result
						--qty;
						if (debug)
							fprintf(stderr, "Manager: Saved the result of %d expr: %d\n", m, res[m]);
					} else	//a chunk isn't finished - pass it to the executors
						if (write(STDOUT_FILENO, input, s) == -1)
							syserr("Error while writing to the first pipe [manager]!\n");

					break;	//either way - break
				} else {
					//still parsing digits
					u += (input[t] - '0') * m;
					m *= 10;
				}
		}

		//less than n data chunks, can write more to the ring
		if (i < num) {
			if (fgets(input, INPUT_SIZE, inFile) == NULL)
				syserr("Error while reading line %d [manager]\n", i);
			sprintf(output, "%d: %s\n", i, input);
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
	return 0;
}