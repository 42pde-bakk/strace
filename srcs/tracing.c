//
// Created by Peer de Bakker on 6/20/22.
//

#include "strace.h"
#include "syscalls.h"

#include <stdbool.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <linux/ptrace.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <string.h>
#include <unistd.h>
#include <sys/uio.h>
#include <elf.h>
#include <time.h>
#include <assert.h>

int	init_tracing() {
	int status;
	long ret;

	ret = ptrace(PTRACE_SEIZE, g_childpid, NULL, NULL);
	if (ret == -1)
		perror("PTRACE_SEIZE");
	ret = ptrace(PTRACE_INTERRUPT, g_childpid, NULL, NULL);
	if (ret == -1)
		perror("PTRACE_INTERRUPT");
	if (waitpid(g_childpid, &status, WUNTRACED) == -1)
	{
		perror("waitpid");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	print_status(int status) {
	if (WIFEXITED(status)) {
		fprintf(stderr, "+++ exited with %d +++\n", WEXITSTATUS(status));
		return (1);
	}
	if (WIFSIGNALED(status)) {
		fprintf(stderr,"+++ killed by signal +++\n");
		return (1);
	}
	return (0);
}

static int	next_syscall(const pid_t child_pid, const unsigned int flags) {
	struct user_regs_struct regs;
	int	status = 0;
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

		if (waitpid(child_pid, &status, WUNTRACED) == -1) {
			break ;
		}

		ret = ptrace(PTRACE_GETREGSET, child_pid, NT_PRSTATUS, &iov);
		if (ret) {
			perror("PTRACE_GETREGS1");
			return (EXIT_FAILURE);
		}

		if (!(flags & FLAG_SUMMARY_VALUE)) {
			handle_syscall(&regs, child_pid);
		}

		ret = ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
		if (ret) {
			perror("PTRACE_SYSCALL2");
			return (EXIT_FAILURE);
		}
		const clock_t start_time = clock();
		if (waitpid(child_pid, &status, WUNTRACED) == -1) {
			break ;
		}
		check_detached(&regs);

		t_summary *summary = &syscalls[regs.orig_rax].summary;
		double elapsed_time = measure_elapsed_time(start_time);
		summary->calls++;
		summary->seconds += elapsed_time;

		ptrace(PTRACE_GETREGSET, child_pid, NT_PRSTATUS, &iov);
		if ((int)regs.rax < 0) {
			summary->errors++;
		}

		if (WIFSIGNALED(status) || WIFEXITED(status)) {
			if (flags & FLAG_SUMMARY_VALUE) {
				print_summary();
			} else {
				fprintf(stderr, " = ?\n");
				check_and_print_errno(&regs);
				print_status(status);
			}
			break ;
		}


		if (!(flags & FLAG_SUMMARY_VALUE)) {
			print_syscall_return_value(&regs);
		}
	}
	return (EXIT_SUCCESS);
}

int start_tracing(const unsigned int flags) {
	if (init_tracing())
		return (EXIT_FAILURE);

	next_syscall(g_childpid, flags);
	close(g_childpid);
	return (EXIT_SUCCESS);
}
