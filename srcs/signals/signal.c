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

extern bool should_detach;
extern int signal_received;

void	print_siginfo_t(const siginfo_t *siginfo) {
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

static int handle_exited(const int status, const unsigned int flags) {
	if (flags & FLAG_SUMMARY_VALUE) {
		print_summary();
	}
	else {
		fprintf(stderr, " = ?\n");
		// Do I need to do check_and_print_errno ?
		fprintf(stderr, "+++ exited with %d +++\n", WEXITSTATUS(status));
	}
	return (BREAK);
}

static int handle_signaled(const int status, const unsigned int flags) {
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

static int handle_stopped(const int status, const unsigned int flags) {
	siginfo_t siginfo;

	bzero(&siginfo, sizeof(siginfo));
	if ((ptrace(PTRACE_GETSIGINFO, g_childpid, NULL, &siginfo) != -1 && !(siginfo.si_signo == SIGTRAP && siginfo.si_code != 0))) {
		const int sig = WSTOPSIG(status);
		if (sig == SIGINT || sig == SIGQUIT) {
			should_detach = true;
			signal_received = sig;
			return (NO_ACTION);
		}
		const t_errorsignal *error_signal = get_error_signal(sig);
		if (!(flags & FLAG_SUMMARY_VALUE)) {
			print_siginfo_t(&siginfo);
		}
		if (error_signal) {
			if (flags & FLAG_SUMMARY_VALUE) {
				print_summary();
			}
			else {
				fprintf(stderr, "+++ killed by %s", error_signal->name);
				if (WCOREDUMP(status))
					fprintf(stderr, " (core dumped)");
				fprintf(stderr, " +++\n");
			}
			fprintf(stderr, "%s", error_signal->exit_msg);
			exit(error_signal->exit_status);
		}
		return (CONTINUE);
	}
	return (NO_ACTION);
}

int	check_child_state(const int status, const unsigned int flags) {
	if (WIFEXITED(status)) {
		return (handle_exited(status, flags));
	}
	if (WIFSIGNALED(status)) {
		return (handle_signaled(status, flags));
	}
	if (WIFSTOPPED(status)) {
		return (handle_stopped(status, flags));
	}
	return (NO_ACTION);
}
