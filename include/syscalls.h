//
// Created by Peer de Bakker on 6/20/22.
//

#ifndef STRACE_SYSCALLS_H
#define STRACE_SYSCALLS_H
//#include <sys/syscall.h>
#include <sys/user.h>
#define MAX_SYSCALL_NB 334

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
	STRING_ARRAY,
	E_PTRACE_REQUEST,
	UNSIGNED_LONG_LONG_INT,
	LONG_LONG_INT
}	e_syscall_type;

typedef void (*handler_func)(const struct user_regs_struct*);

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
	handler_func handler;
}	t_syscall;

extern const t_syscall syscalls[];

/*
 * srcs/print_syscall.c
 */
void	handle_syscall(const struct user_regs_struct *regs, pid_t child_pid);
void	print_syscall(const struct user_regs_struct *regs);
void	ptrace_handler(const struct user_regs_struct *regs);
void	mmap_handler(const struct user_regs_struct *regs);
void print_syscall_return_value(struct user_regs_struct *regs);

/*
 * srcs/ptrace_request_handler.c
 */
void	enum__ptrace_request_handler(unsigned long long int regval);

/*
 * srcs/mmap_options_handler.c
 */
void	mmap_handle_prots(unsigned long long regval);
void	mmap_handle_flags(unsigned long long regval);

#endif //STRACE_SYSCALLS_H
