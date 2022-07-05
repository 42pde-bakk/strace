//
// Created by Peer de Bakker on 7/5/22.
//
#include <signal.h>
#include <stdbool.h>

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
