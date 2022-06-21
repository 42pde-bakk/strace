//
// Created by Peer de Bakker on 6/19/22.
//

#ifndef STRACE_STRACE_H
#define STRACE_STRACE_H

#include <unistd.h>
#include <sys/user.h>

/*
 * srcs/signal.c
 */
void	catch_sigint(int signal);

/*
 * srcs/tracing.c
 */
int	init_tracing(pid_t child_pid);
int start_tracing(pid_t child_pid);


#endif //STRACE_STRACE_H
