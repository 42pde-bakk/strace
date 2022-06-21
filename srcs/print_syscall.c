//
// Created by Peer de Bakker on 6/21/22.
//

#include "strace.h"
#include "syscalls.h"
#include <sys/user.h>
#include <stdio.h>

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
		case VOID_POINTER:
		case STRUCT:
		case POINTER:
			fprintf(stderr, "%p", (void *)regval);
			break;
		case STRING:
			fprintf(stderr, "%s", (char *)regval);
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

void	print_syscall(struct user_regs_struct *regs) {
	unsigned long long int syscall_nb = regs->orig_rax;

	const unsigned long long int registers[6] = {
			regs->rdi, regs->rsi, regs->rdx,
			regs->r10, regs->r8, regs->r9
	};

	if (syscall_nb >= 10 || 1) {
		fprintf(stderr, "raw regs values:\t%llu(%llu, %llu, %llu, %llu, %llu, %llu)\n",
				syscall_nb,
				regs->rdi, regs->rsi, regs->rdx,
				regs->r10, regs->r8, regs->r9);
	}
	if (syscall_nb < 10) {
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
}

void	print_syscall_return_value(struct user_regs_struct *regs, const unsigned long long int syscallNb) {
	const t_syscall syscall = syscalls[syscallNb];

	fprintf(stderr, " = ");
	if (syscallNb < 10) {
		switch_case(syscall.return_value, regs->rax);
	} else {
		fprintf(stderr, "%llu", regs->rax);
	}
	fprintf(stderr, "\n");
//	fprintf(stderr, "rax = %llu, orig_rax = %llu\n", regs->rax, regs->orig_rax);
}
