//
// Created by Peer de Bakker on 6/20/22.
//

#ifndef STRACE_SYSCALLS_H
#define STRACE_SYSCALLS_H
//#include <sys/syscall.h>
#include <sys/user.h>
#define MAX_SYSCALL_NB 60

typedef enum syscall_type {
	NONE,
	INT,
	UNSIGNED_INT,
	SIZE_T,
	POINTER,
	STRUCT,
	STRING,
	SSIZE_T,
	UNSIGNED_LONG_INT,
	VOID,
	LONG_INT,
	FLAGS,
	VOID_POINTER,
	STRING_ARRAY
}	e_syscall_type;

typedef struct s_syscall {
	long int				nb64;
	long int				nb32;
	e_syscall_type			return_value;
	char*					name;
	union {
		e_syscall_type			registers[6];
		e_syscall_type			rdi;		// first pa
		e_syscall_type			rsi;		// seconde param
		e_syscall_type			rdx;		// third param
		e_syscall_type			r10;		// four param
		e_syscall_type			r8;			// five param
		e_syscall_type			r9;			// six param
	};
}	t_syscall;

extern t_syscall syscalls[];

/*
 * srcs/print_syscall.c
 */
/*
 * srcs/print_syscall.c
 */
void	print_syscall(struct user_regs_struct *regs);
void	print_syscall_return_value(struct user_regs_struct *regs, unsigned long long int syscallNb);

#endif //STRACE_SYSCALLS_H
