//
// Created by Peer de Bakker on 7/5/22.
//

#include "strace.h"
#include "syscalls.h"
#include <sys/ptrace.h>
#include <stdio.h>
#include <signal.h>
#define DETACH_EXIT_CODE 130

extern bool should_detach;
extern int signal_received;

static void	detach(const struct user_regs_struct *regs, const unsigned int flags) {
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

void check_detached(const struct user_regs_struct *regs, const unsigned int flags) {
	if (should_detach)
		detach(regs, flags);
}
