//
// Created by Peer de Bakker on 6/18/22.
//


#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/ptrace.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/user.h>
#include "strace.h"
#include <string.h>

int execute_child(char **argv) {
	int		res;
	char	**cmds;

	cmds = argv + 1;
	dprintf(2, "lets go execvp(%s, cmds)\n", argv[1]);
	res = execvp(cmds[0], cmds);
	if (res == -1)
		perror("execvp");
	return (res);
}

static int	init_tracing()
{
	int status,
		pid = getpid();
	long ret;

	signal(SIGINT, &catch_sigint);
	ret = ptrace(PTRACE_SEIZE, pid, NULL, NULL);
	if (ret == -1)
		perror("PTRACE_SEIZE");
	ret = ptrace(PTRACE_INTERRUPT, pid, NULL, NULL);
	if (ret == -1)
		perror("PTRACE_INTERRUPT");
	if (waitpid(pid, &status, WUNTRACED) == -1)
	{
		dprintf(2, "ft_strace: waitpid fail\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}


int start_tracing(int child_pid) {
	struct user_regs_struct regs;
	siginfo_t	siginfo;
	long	res;
	int		status;

	if (init_tracing())
		return (EXIT_FAILURE);

	close(child_pid);
	return (EXIT_SUCCESS);
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
	dprintf(2, "lets go start_tracing\n");
	return (start_tracing(child));
}
