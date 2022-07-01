//
// Created by Peer de Bakker on 6/19/22.
//

#ifndef STRACE_STRACE_H
#define STRACE_STRACE_H

#include <unistd.h>
#include <sys/user.h>
#include <sys/types.h>
#include <stdbool.h>

#define FLAG_SUMMARY_CHAR 'c'
#define FLAG_SUMMARY_VALUE 0b01

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
void	check_detached(const struct user_regs_struct *regs, unsigned int flags);

/*
 * srcs/tracing.c
 */
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
char	*ft_str3join(const char *a, const char *b, const char *c);
void	free_array(char **arr);

/*
 * srcs/summary.c
 */
void	print_summary();

/*
 * srcs/print_strings.c
 */
char	*read_string(unsigned long long regval, bool *dots, size_t read_amount);
char	*buffering(char *str);
void	read_string_array(unsigned long long regval);

#endif //STRACE_STRACE_H
