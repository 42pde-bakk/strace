//
// Created by Peer de Bakker on 6/18/22.
//

#include <stdio.h>
#include <stdlib.h>
#include "strace.h"
#include <string.h>
#include <errno.h>

pid_t	g_childpid = -1;

static int execute_child(char **argv, char *abspath) {
	int		res;
	char	**cmds;

	cmds = argv;
	cmds[0] = abspath;
	res = execvp(cmds[0], cmds);
	if (res == -1)
		perror("execvp");
	return (res);
}

static int	get_path_nb(int argc, char **argv) {
	int i = 1;

	while (i < argc) {
		if (argv[i][0] != '-') {
			return (i);
		}
		i++;
	}
	return (-1);
}

int main(int argc, char **argv, const char **envp) {
	unsigned int	flags;
	int				error = 0;
	int				path_nb;
	char	*abspath;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s program_name [arguments]\n", argv[0]);
		return (EXIT_FAILURE);
	}

	flags = parse_flags(argc, argv, &error);
	if (error) {
		fprintf(stderr, "ft_strace: Invalid option\n");
		return (EXIT_FAILURE);
	}

	path_nb = get_path_nb(argc, argv);
	if (path_nb == -1) {
		fprintf(stderr, "ft_strace: Please provide a valid executable\n");
		return (EXIT_FAILURE);
	}

	abspath = get_absolute_path(argv[path_nb], envp);
	if (abspath == NULL) {
		fprintf(stderr, "ft_strace: Can't stat '%s': %s\n", argv[path_nb], strerror(errno));
		return (EXIT_FAILURE);
	}
	g_childpid = fork();
	if (g_childpid == -1) {
		perror("fork");
		return (EXIT_FAILURE);
	}
	if (g_childpid == 0) {
		return (execute_child(&argv[path_nb], abspath));
	}
	setup_sighandlers();
	free(abspath);
	return (start_tracing(flags));
}
