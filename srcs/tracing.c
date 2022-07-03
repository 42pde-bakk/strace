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

int	wait_child() {
	int status;
	const sigset_t	*empty = get_empty_sigset();
	const sigset_t	*blocked = get_blocked_sigset();

	if ((sigprocmask(SIG_SETMASK, empty, NULL) == -1)) {
		perror("sigprocmask(SIG_SETMASK)");
	}
	if ((waitpid(g_childpid, &status, WUNTRACED) == -1)) {
		perror("waitpid");
	}
	if ((sigprocmask(SIG_BLOCK, blocked, get_empty_sigset()) == -1)) {
		perror("sigprocmask(SIG_BLOCK)");
	}
	return (status);
}

static int	init_tracing() {
	int status;
	long ret;

	ret = ptrace(PTRACE_SEIZE, g_childpid, NULL, NULL);
	if (ret == -1)
		perror("PTRACE_SEIZE");
	ret = ptrace(PTRACE_INTERRUPT, g_childpid, NULL, NULL);
	if (ret == -1)
		perror("PTRACE_INTERRUPT");
	if ((waitpid(g_childpid, &status, WUNTRACED) == -1)) {
		perror("waitpid (init_tracing)");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	next_syscall(const pid_t child_pid, const unsigned int flags) {
	struct user_regs_struct regs;
	int	status;
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

		status = wait_child();
		if (check_child_state(status, flags)) {

			continue ;
		}
		check_detached(&regs, flags);

		ret = ptrace(PTRACE_GETREGSET, child_pid, NT_PRSTATUS, &iov);
		if (ret) {
			perror("PTRACE_GETREGS1");
			return (EXIT_FAILURE);
		}
//		fprintf(stderr,"1orig_rax=%llu ", regs.orig_rax);

		if (!(flags & FLAG_SUMMARY_VALUE)) {
			handle_syscall(&regs, child_pid);
		}

		ret = ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
		if (ret) {
			perror("PTRACE_SYSCALL2");
			return (EXIT_FAILURE);
		}

		const clock_t start_time = clock();
		status = wait_child();
		if (check_child_state(status, flags)) {
			isAlive = false;
			break ;
		}
		check_detached(&regs, flags);

		t_summary *summary = NULL;
		if (regs.orig_rax <= MAX_SYSCALL_NB) {
			summary = &syscalls[regs.orig_rax].summary;
			double elapsed_time = measure_elapsed_time(start_time);
			summary->calls++;
			summary->seconds += elapsed_time;
		}

		ptrace(PTRACE_GETREGSET, child_pid, NT_PRSTATUS, &iov);
//		fprintf(stderr,"2orig_rax=%llu ", regs.orig_rax);

		if ((int)regs.rax < 0 && summary != NULL) {
			summary->errors++;
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
