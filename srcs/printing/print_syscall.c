//
// Created by Peer de Bakker on 6/21/22.
//

#include "strace.h"
#include "syscalls.h"
#include <sys/user.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static void	switch_case(const e_syscall_type eSyscallType, const unsigned long long int regval) {
#ifndef PRINT_STRINGS_AS_POINTERS
	char *str;
	bool dots = false;
#endif
	switch (eSyscallType) {
		case FLAGS:
		case INT:
			fprintf(stderr, "%d", (int)regval);
			break;
		case UNSIGNED_INT:
			fprintf(stderr, "%u", (unsigned int)regval);
			break;
		case SIZE_T:
			fprintf(stderr, "%zu", (size_t)regval);
			break;
#ifdef PRINT_STRINGS_AS_POINTERS
		case BUFFER:
		case STRING:
		case STRING_ARRAY:
#else
		case BUFFER:
			/* Pretty much the same as a string, but print newlines as '\n' */
			str = read_string(regval, &dots, 4);
			str = buffering(str);
			fprintf(stderr, "\"%s\"", str);
			if (dots)
				fprintf(stderr, "...");
			free(str);
			break ;
		case STRING:
			/*
			 * Even though the subject doesn't allow the use of PTRACE_PEEKDATA or PTRACE_PEEKTEXT,
			 * we need those functions to print strings.
			 * Otherwise, the best we can do is to only print their address.
			 */
			str = read_string(regval, &dots, 5);
			fprintf(stderr, "\"%s\"", str);
			free(str);
			break ;
		case STRING_ARRAY:
			read_string_array(regval);
			break ;
#endif
		case VOID_POINTER:
		case STRUCT:
		case POINTER:
			if (regval == 0)
				fprintf(stderr, "NULL");
			else
				fprintf(stderr, "%p", (void *)regval);
			break;
		case SSIZE_T:
			fprintf(stderr, "%zd", (ssize_t)regval);
			break;
		case UNSIGNED_LONG_INT:
			fprintf(stderr, "%lu", (unsigned long int)regval);
			break;
		case LONG_INT:
			fprintf(stderr, "%ld", (long int)regval);
			break;
		default:
			break;
	}
}

void	mmap_handler(const struct user_regs_struct *regs) {
	const t_syscall	syscal_mmap = syscalls[9];
	fprintf(stderr, "%s(", syscal_mmap.name);
	switch_case(syscal_mmap.rdi, regs->rdi);

	fprintf(stderr, ", %zu, ", (size_t)regs->rsi);
	mmap_handle_prots(regs->rdx);
	fprintf(stderr, ", ");
	mmap_handle_flags(regs->r10);
	fprintf(stderr, ", %d, %ld)", (int)regs->r8, (long int)regs->r9);
}

void	ptrace_handler(const struct user_regs_struct *regs) {
	const t_syscall syscall_ptrace = syscalls[101];
	fprintf(stderr, "%s(", syscall_ptrace.name);
	enum__ptrace_request_handler(regs->rdi);
	fprintf(stderr, ", %d, %p, %p)", (int)regs->rsi, (void *)regs->rdx, (void *)regs->r10);
}

void	print_syscall(const struct user_regs_struct *regs) {
	unsigned long long int syscall_nb = regs->orig_rax;

	const unsigned long long int registers[6] = {
			regs->rdi, regs->rsi, regs->rdx,
			regs->r10, regs->r8, regs->r9
	};
	if (syscall_nb > MAX_SYSCALL_NB) {
		fprintf(stderr, "syscall_%llubad(%llx, %llx, %llx, %llx, %llx, %llx)",
				syscall_nb,
				regs->rdi, regs->rsi, regs->rdx,
				regs->r10, regs->r8, regs->r9);
		return ;
	}
	const t_syscall syscall = syscalls[syscall_nb];
	// print name
	fprintf(stderr, "%s(", syscall.name);
	for (int i = 0; i < 6 && syscall.registers[i] != NONE; i++) {
		switch_case(syscall.registers[i], registers[i]);

		if (i < 5 && syscall.registers[i + 1] != NONE) {
			fprintf(stderr, ", ");
		}
	}
	fprintf(stderr, ")");
}

void	handle_syscall(const struct user_regs_struct *regs, const pid_t child_pid) {
	const unsigned long long int syscall_nb = regs->orig_rax;

	if (syscall_nb > MAX_SYSCALL_NB) {
		fprintf(stderr, "syscall_%#llxbad(%#llx, %#llx, %#llx, %#llx, %#llx, %#llx)",
				syscall_nb, regs->rdi, regs->rsi, regs->rdx, regs->r10, regs->r8, regs->r9);
	}
	const t_syscall syscall = syscalls[syscall_nb];

	g_childpid = child_pid;
	return syscall.handler(regs);
}

int	check_and_print_errno(const struct user_regs_struct *regs) {
	if (regs->orig_rax > MAX_SYSCALL_NB) {
		fprintf(stderr, "%d", (int)regs->rax);
		return (1);
	}
	const t_syscall syscall = syscalls[regs->orig_rax];
	const int child_errno_nb = -1 * (int)regs->rax;

	if ((int)regs->rax < 0 && (syscall.return_value != VOID && syscall.return_value != NONE) && child_errno_nb <= MAX_ERRNO_NB) {
		t_errno child_errno = errnoTable[child_errno_nb];
		fprintf(stderr, "%d %s (%s)", -1, child_errno.code, child_errno.description);
		return (1);
	}
	return (0);
}

void print_syscall_return_value(struct user_regs_struct *regs) {
	const unsigned long long int syscallNb = regs->orig_rax;
	if (syscallNb > MAX_SYSCALL_NB) {
		fprintf(stderr, " = %llu\n", regs->rax);
		return ;
	}
	const t_syscall syscall = syscalls[syscallNb];

	fprintf(stderr, " = ");
	if (check_and_print_errno(regs) == 0) {
		switch_case(syscall.return_value, regs->rax);
	}
	fprintf(stderr, "\n");
}
