//
// Created by Peer de Bakker on 6/18/22.
//

#include <stdio.h>
#include <stdlib.h>
#include "strace.h"
#include <string.h>
#include <errno.h>

int execute_child(char **argv, char *abspath) {
	int		res;
	char	**cmds;

	cmds = argv + 1;
	cmds[0] = abspath;
	res = execvp(cmds[0], cmds);
	if (res == -1)
		perror("execvp");
	return (res);
}

int main(int argc, char **argv, const char **envp) {
	pid_t	child;
	char	*abspath;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s program_name [arguments]\n", argv[0]);
		return (EXIT_FAILURE);
	}
	abspath = get_absolute_path(argv[1], envp);
	if (abspath == NULL) {
		fprintf(stderr, "ft_strace: Can't stat '%s': %s\n", argv[1], strerror(errno));
		return (EXIT_FAILURE);
	}
	child = fork();
	if (child == -1) {
		perror("fork");
		return (EXIT_FAILURE);
	}
	if (child == 0) {
		return (execute_child(argv, abspath));
	}
	free(abspath);
	return (start_tracing(child));
}
