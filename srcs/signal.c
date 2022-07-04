//
// Created by Peer de Bakker on 6/19/22.
//

#include <signal.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "syscalls.h"
#include "strace.h"
#include "user_regs_structs.h"

#define DETACH_EXIT_CODE 130
#define SIGSEGV_EXIT_CODE 139

static bool	should_detach;
static int	signal_received;

sigset_t	*get_empty_sigset() {
	static bool init = false;
	static sigset_t	empty;

	if (!init) {
		sigemptyset(&empty);
		init = true;
	}
	return (&empty);
}

sigset_t	*get_blocked_sigset() {
	static bool init = false;
	static sigset_t	blocked;

	if (!init) {
		sigemptyset(&blocked);
		sigaddset(&blocked, SIGHUP);
		sigaddset(&blocked, SIGINT);
		sigaddset(&blocked, SIGQUIT);
		sigaddset(&blocked, SIGPIPE);
		sigaddset(&blocked, SIGTERM);
		sigaddset(&blocked, SIGTTIN);
		init = true;
	}
	return (&blocked);
}

static void	sigint_handler(int sig) {
	(void)sig;
	signal_received = SIGINT;
	kill(g_childpid, SIGINT);
	should_detach = true;
}

static void	sigquit_handler(int sig) {
	(void)sig;
	signal_received = SIGQUIT;
	kill(g_childpid, SIGQUIT);
	should_detach = true;
}

void	setup_sighandlers() {
	signal(SIGINT, &sigint_handler);
	signal(SIGQUIT, &sigquit_handler);
}

static void	detach(const t_user_regs *regs, const unsigned int flags) {
	ptrace(PTRACE_DETACH, g_childpid, 0, 0);
	if (flags & FLAG_SUMMARY_VALUE) {
		fprintf(stderr, "ft_strace: Process %d detached\n", g_childpid);
		print_summary();
	}
	else {
		fprintf(stderr, " = ?");
		check_and_print_errno(regs);
		fprintf(stderr, "\nft_strace: Process %d detached\n <detached ...>\n", g_childpid);
		if (signal_received == SIGQUIT)
			fprintf(stderr, "Quit");
		fprintf(stderr, "\n");
	}
	exit(DETACH_EXIT_CODE);
}

void check_detached(const t_user_regs *regs, const unsigned int flags) {
	if (should_detach)
		detach(regs, flags);
}

void	print_siginfo_t(const siginfo_t *siginfo, int status) {
	int sig = WSTOPSIG(status);
	(void)sig;

	fprintf(stderr, "--- %s {si_signo=%s, si_code=%s, si_pid=%d, si_uid=%d, si_status=%d, si_utime=%ld, si_stime=%ld} ---\n",
			get_signal_name(siginfo->si_signo),
			get_signal_name(siginfo->si_signo),
			get_signal_code(siginfo->si_signo, siginfo->si_code),
			siginfo->si_pid,
			siginfo->si_uid,
			siginfo->si_status,
			siginfo->si_utime,
			siginfo->si_stime
	);
}

int	check_child_state(const int status, const unsigned int flags) {
	if (WIFEXITED(status)) {
		if (flags & FLAG_SUMMARY_VALUE) {
			print_summary();
		}
		else {
			fprintf(stderr, " = ?\n");
			// Do I need to do check_and_print_errno
			fprintf(stderr, "+++ exited with %d +++\n", WEXITSTATUS(status));
		}
		return (BREAK);
	}
	if (WIFSIGNALED(status)) {
		if (flags & FLAG_SUMMARY_VALUE) {
			print_summary();
		}
		else {
			fprintf(stderr, "+++ killed by %s", get_signal_name(WTERMSIG(status)));
			if (WCOREDUMP(status)) {
				fprintf(stderr, " (core dumped)");
			}
			fprintf(stderr, " +++\n");
		}
		return (BREAK);
	}
	if (WIFSTOPPED(status)) {
		siginfo_t siginfo;

		bzero(&siginfo, sizeof(siginfo));
		if ((ptrace(PTRACE_GETSIGINFO, g_childpid, NULL, &siginfo) != -1 && !(siginfo.si_signo == SIGTRAP && siginfo.si_code != 0))) {
			print_siginfo_t(&siginfo, status);
			if (WSTOPSIG(status) == SIGSEGV) {
				fprintf(stderr, "+++ killed by %s (core dumped) +++\n", get_signal_name(WSTOPSIG(status)));
				exit(SIGSEGV_EXIT_CODE);
			}
			return (CONTINUE);
		}
	}
	return (NO_ACTION);
}
