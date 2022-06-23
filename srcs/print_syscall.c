//
// Created by Peer de Bakker on 6/21/22.
//

#include "strace.h"
#include "syscalls.h"
#include <sys/user.h>
#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <errno.h>

static int g_childpid = -1;
static void	read_string(unsigned long long regval) {
	char buf[1024];
	long int ret;
	size_t i = 0;

	memset(buf, 0, sizeof(buf));
	while (i < 5) {
		ret = ptrace(PTRACE_PEEKDATA, g_childpid, regval + (i * sizeof(ret)), 0);
		if (errno) {
			break ;
		}
		memcpy(buf + (i * sizeof(ret)), &ret, sizeof(ret));
		if (memchr(&ret, 0, sizeof(ret)) != NULL)
			break ;
		i++;
	}
	fprintf(stderr, "%s", buf);
}

static void	switch_case(const e_syscall_type eSyscallType, const unsigned long long int regval) {
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
		case STRING_ARRAY:
		case STRING:
			/*
			 * I'd love to actually print the string,
			 * But since we aren't allowed to use PTRACE_PEEKDATA or PTRACE_PEEKTEXT,
			 * Printing the address is the best I can do.
			 */
			read_string(regval);
			break ;
		case VOID_POINTER:
		case STRUCT:
		case POINTER:
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
	const t_syscall syscal_mmap = syscalls[9];
	fprintf(stderr, "\t prot=%llu, flags=%llu\t", regs->rdx, regs->r10);
	fprintf(stderr, "%s(%p, %zu, ", syscal_mmap.name, (void *)regs->rdi, (size_t)regs->rsi);
	mmap_handle_prots(regs->rdx);
	fprintf(stderr, ", ");
	mmap_handle_flags(regs->r10);
	fprintf(stderr, ", %d, %ld)", (int)regs->r8, (long int)regs->r9);
//	exit(1);
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
		fprintf(stderr, "%llu(%llu, %llu, %llu, %llu, %llu, %llu)",
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
	const t_syscall syscall = syscalls[syscall_nb];

	g_childpid = child_pid;
	return syscall.handler(regs);
}

void print_syscall_return_value(struct user_regs_struct *regs) {
	const unsigned long long int syscallNb = regs->orig_rax;
	if (syscallNb > MAX_SYSCALL_NB) {
		fprintf(stderr, " = %llu\n", regs->rax);
		return ;
	}
	const t_syscall syscall = syscalls[syscallNb];

	fprintf(stderr, " = ");
	const int child_errno_nb = -1 * (int)regs->rax;
	if ((int)regs->rax < 0 && syscall.return_value != VOID && child_errno_nb <= MAX_ERRNO_NB) {
		t_errno child_errno = errnoTable[child_errno_nb];
		fprintf(stderr, "%d %s %s", -1, child_errno.code, child_errno.description);
	} else {
		switch_case(syscall.return_value, regs->rax);
	}
	fprintf(stderr, "\n");
}
