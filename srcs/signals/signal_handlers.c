//
// Created by Peer de Bakker on 7/5/22.
//
#include <stdbool.h>
#include <signal.h>

extern pid_t g_childpid;
bool	should_detach;
int		signal_received;

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
