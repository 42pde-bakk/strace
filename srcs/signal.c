//
// Created by Peer de Bakker on 6/19/22.
//

#include <signal.h>
#include <unistd.h>

void	catch_sigint(int signal)
{
	(void)signal;
	kill(getpid(), SIGINT);
	// detach ?
}
