//
// Created by Peer de Bakker on 6/19/22.
//

#include <signal.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <stdbool.h>
#include <stdlib.h>
#include "syscalls.h"

#define DETACH_EXIT_CODE 130

extern pid_t g_childpid;
static bool	should_detach;
static int	signal_received;

static void	sigint_handler(int sig) {
	(void)sig;
	signal_received = SIGINT;
	kill(g_childpid, SIGINT);
	// detach ?
	should_detach = true;
}

static void	sigquit_handler(int sig) {
	(void)sig;
	signal_received = SIGQUIT;
	kill(g_childpid, SIGQUIT);
	// detach ?
	should_detach = true;
}

void	setup_sighandlers() {
	signal(SIGINT, &sigint_handler);
	signal(SIGQUIT, &sigquit_handler);
}

static void	detach(const struct user_regs_struct *regs) {
	fprintf(stderr, "detached!\n");
	ptrace(PTRACE_DETACH, g_childpid, 0, 0);
	fprintf(stderr, " = ?");
	check_and_print_errno(regs);
	fprintf(stderr, "\nft_strace: Process %d detached\n <detached ...>\n", g_childpid);
	if (signal_received == SIGQUIT)
		fprintf(stderr, "Quit");
	fprintf(stderr, "\n");
	exit(DETACH_EXIT_CODE);
}

void check_detached(const struct user_regs_struct *regs) {
	if (should_detach)
		detach(regs);
}
