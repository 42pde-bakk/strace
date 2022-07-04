//
// Created by Peer de Bakker on 6/20/22.
//

#ifndef STRACE_SYSCALLS_H
#define STRACE_SYSCALLS_H
//#include <sys/syscall.h>
#include <sys/user.h>
#include <stdlib.h>
#include <summary.h>
#include "user_regs_structs.h"

#define MAX_SYSCALL_NB_64 334
#define MAX_SYSCALL_NB_32 386

typedef enum {
	ARCH_32,
	ARCH_64,
	INVALID
}	e_arch;
extern e_arch arch;
extern size_t max_syscall_nb;

typedef enum syscall_type {
	NONE,
	INT,
	UNSIGNED_INT,
	SIZE_T,
	POINTER,
	STRUCT,
	STRING,
	BUFFER,
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

typedef void (*handler_func)(const t_user_regs *);

typedef struct s_syscall {
	const e_syscall_type		return_value;
	const char*					name;
	const union {
		e_syscall_type			registers[6];
		e_syscall_type			rdi;		// first parameter
		e_syscall_type			rsi;		// second parameter
		e_syscall_type			rdx;		// third parameter
		e_syscall_type			r10;		// fourth parameter
		e_syscall_type			r8;			// fifth parameter
		e_syscall_type			r9;			// sixth parameter
	};
	const handler_func handler;
	t_summary summary;
}	t_syscall;

extern t_syscall syscalls_64[];
extern t_syscall syscalls_32[];
extern t_syscall *syscalls;

/*
 * srcs/print_syscall.c
 */
int		check_and_print_errno(const t_user_regs *regs);
void	handle_syscall(const t_user_regs *regs, pid_t child_pid);
void	print_syscall(const t_user_regs *regs);
void	ptrace_handler(const t_user_regs *regs);
void	mmap_handler(const t_user_regs *regs);
void	print_syscall_return_value(const t_user_regs *regs);

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
