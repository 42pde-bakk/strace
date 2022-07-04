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
#include "user_regs_structs.h"

e_arch arch;
t_syscall *syscalls;
size_t		max_syscall_nb;

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
	if ((sigprocmask(SIG_BLOCK, blocked, NULL) == -1)) {
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

void	get_arch(const struct iovec *iov) {
	static int done = false;
	static const size_t user_regs_struct_size_32 = 136;
//	static const size_t user_regs_struct_size_64 = 216;
//	Found in /usr/include/x86_64-linux-gnu/sys/user.h

	if (done)
		return ;
	done = true;
	fprintf(stderr, "sizeof = %zu\n", iov->iov_len);
	if (iov->iov_len == user_regs_struct_size_32) {
		arch = ARCH_32;
		max_syscall_nb = MAX_SYSCALL_NB_32;
		syscalls = syscalls_32;
		fprintf(stderr, "32 bits bro\n");
		exit(1);
	}
	else {
		arch = ARCH_64;
		max_syscall_nb = MAX_SYSCALL_NB_64;
		syscalls = syscalls_64;
	}
}

static int	next_syscall(const pid_t child_pid, const unsigned int flags) {
	t_user_regs_struct_64	regs64;
	t_user_regs				regs;
	int	status;
	long ret;
	int childstate_action;
	struct iovec iov = {
			.iov_base = &regs64,
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
		childstate_action = check_child_state(status, flags);
		if (childstate_action == CONTINUE)
			continue ;
		else if (childstate_action == BREAK)
			break ;

		check_detached(&regs, flags);

		ret = ptrace(PTRACE_GETREGSET, child_pid, NT_PRSTATUS, &iov);
		if (ret) {
			perror("PTRACE_GETREGS1");
			return (EXIT_FAILURE);
		}
		regs = convert_to_regs(&regs64);
		fprintf(stderr, "syscall = %llu\n", regs64.orig_rax);
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
		childstate_action = check_child_state(status, flags);
		if (childstate_action == CONTINUE)
			continue ;
		else if (childstate_action == BREAK)
			break ;

		check_detached(&regs, flags);

		t_summary *summary = NULL;
		if (regs64.orig_rax <= max_syscall_nb) {
			summary = &syscalls[regs64.orig_rax].summary;
			double elapsed_time = measure_elapsed_time(start_time);
			summary->calls++;
			summary->seconds += elapsed_time;
		}

		ptrace(PTRACE_GETREGSET, child_pid, NT_PRSTATUS, &iov);
		regs = convert_to_regs(&regs64);
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
