//
// Created by Peer de Bakker on 6/19/22.
//

#ifndef STRACE_STRACE_H
#define STRACE_STRACE_H

#include <unistd.h>
#include <sys/user.h>
#include <sys/types.h>

#define FLAG_SUMMARY_CHAR 'c'
#define FLAG_OUTPUT_REDIR_CHAR 'o'
#define FLAG_SUMMARY_VALUE 0b01
#define FLAG_OUTPUT_REDIR_VALUE 0b10

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
void check_detached(const struct user_regs_struct *regs, unsigned int flags);

/*
 * srcs/tracing.c
 */
int	init_tracing();
int start_tracing(unsigned int flags);

/*
 * srcs/path_management.c
 */
char	*get_absolute_path(const char* arg, const char **envp);

/*
 * srcs/parse_flags.c
 */
unsigned int parse_flags(int argc, char **argv, int *error);

/*
 * srcs/utils.c
 */
double	measure_elapsed_time(clock_t start);

/*
 * srcs/summary.c
 */

void	print_summary();

#endif //STRACE_STRACE_H
