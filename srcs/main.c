//
// Created by Peer de Bakker on 6/18/22.
//

#include <stdio.h>
#include <stdlib.h>
#include "strace.h"

int execute_child(char **argv) {
	int		res;
	char	**cmds;

	cmds = argv + 1;
	res = execvp(cmds[0], cmds);
	if (res == -1)
		perror("execvp");
	return (res);
}

int main(int argc, char **argv) {
	pid_t	child;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s program_name [arguments]\n", argv[0]);
		return (EXIT_FAILURE);
	}
	child = fork();
	if (child == -1) {
		perror("fork");
		return (EXIT_FAILURE);
	}
	if (child == 0) {
		return (execute_child(argv));
	}
	return (start_tracing(child));
}
