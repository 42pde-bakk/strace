//
// Created by Peer de Bakker on 6/19/22.
//

#ifndef STRACE_STRACE_H
#define STRACE_STRACE_H

#include <unistd.h>
#include <sys/user.h>
#include <sys/types.h>

#define MAX_ERRNO_NB 131
typedef struct s_errno {
	int nb;
	char* code;
	char* description;
}	t_errno;
extern t_errno	errnoTable[];

extern pid_t g_childpid;
/*
 * srcs/signal.c
 */
void	setup_sighandlers();
void check_detached(const struct user_regs_struct *regs);

/*
 * srcs/tracing.c
 */
int	init_tracing();
int start_tracing();

/*
 * srcs/path_management.c
 */
char	*get_absolute_path(const char* arg, const char **envp);


#endif //STRACE_STRACE_H
