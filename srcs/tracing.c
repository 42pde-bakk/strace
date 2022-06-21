//
// Created by Peer de Bakker on 6/20/22.
//

#include "strace.h"
#include "syscalls.h"

#include <stdbool.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <string.h>
#include <unistd.h>

int	init_tracing(const pid_t child_pid) {
	int status;
	long ret;

	ret = ptrace(PTRACE_SEIZE, child_pid, NULL, NULL);
	if (ret == -1)
		perror("PTRACE_SEIZE");
	ret = ptrace(PTRACE_INTERRUPT, child_pid, NULL, NULL);
	if (ret == -1)
		perror("PTRACE_INTERRUPT");
	if (waitpid(child_pid, &status, WUNTRACED) == -1)
	{
		perror("waitpid");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	wait_and_check_status(const pid_t child_pid) {
	int wait_ret;
	int status;

	wait_ret = waitpid(child_pid, &status, WUNTRACED);
	if (wait_ret == -1) {
		perror("waitpid");
		return (1);
	}
	if (WIFEXITED(status)) {
		fprintf(stderr, "+++ exited with %d +++\n", WEXITSTATUS(status));
		return (1);
	}
	if (WIFSIGNALED(status)) {
		fprintf(stderr,"+++ killed by signal +++\n");
		return (1);
	}
//	if (WIFSTOPPED(status)) {
//		fprintf(stderr, "+++ stopped +++ \n");
//		return (1);
//	}

	return (0);
}
# include <sys/uio.h>
#include <elf.h>

int	next_syscall(const pid_t child_pid) {
	struct user_regs_struct regs;
	long ret;
	struct iovec iov = {
			.iov_base = &regs,
			.iov_len = sizeof(struct user_regs_struct)
	};
	bool isAlive = true;

	ptrace(PTRACE_SETOPTIONS, child_pid, 0, PTRACE_O_EXITKILL);

	while (isAlive) {
		ret = ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
		if (ret) {
			perror("PTRACE_SYSCALL1");
			return (EXIT_FAILURE);
		}

		if (wait_and_check_status(child_pid)) {
			break ;
		}
//		waitpid(child_pid, 0, 0);

//		ret = ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
		ret = ptrace(PTRACE_GETREGSET, child_pid, NT_PRSTATUS, &iov);
		if (ret) {
			perror("PTRACE_GETREGS1");
			return (EXIT_FAILURE);
		}

		print_syscall(&regs);

		unsigned long long int syscall = regs.orig_rax;


		ret = ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
		if (ret) {
			perror("PTRACE_SYSCALL2");
			return (EXIT_FAILURE);
		}
		if (wait_and_check_status(child_pid)) {
			break ;
		}
//		waitpid(child_pid, 0, 0);

		ret = ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
		if (ret) {
			perror("PTRACE_GETREGS2");
			return (EXIT_FAILURE);
		}

		print_syscall_return_value(&regs, syscall);
	}

	return (EXIT_SUCCESS);
}

int start_tracing(pid_t child_pid) {

	if (init_tracing(child_pid))
		return (EXIT_FAILURE);

	next_syscall(child_pid);
	close(child_pid);
	return (EXIT_SUCCESS);
}