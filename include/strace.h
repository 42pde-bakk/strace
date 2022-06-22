//
// Created by Peer de Bakker on 6/19/22.
//

#ifndef STRACE_STRACE_H
#define STRACE_STRACE_H

#include <unistd.h>
#include <sys/user.h>
#include <sys/types.h>

/*
 * srcs/signal.c
 */
void	catch_sigint(int signal);

/*
 * srcs/tracing.c
 */
int	init_tracing(pid_t child_pid);
int start_tracing(pid_t child_pid);

/*
 * srcs/path_management.c
 */
char	*get_absolute_path(const char* arg, const char **envp);


#endif //STRACE_STRACE_H
