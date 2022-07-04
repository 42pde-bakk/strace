//
// Created by Peer de Bakker on 7/4/22.
//

#ifndef FT_STRACE_USER_REGS_STRUCTS_H
#define FT_STRACE_USER_REGS_STRUCTS_H

typedef struct
{
	__extension__ unsigned long long int r15;
	__extension__ unsigned long long int r14;
	__extension__ unsigned long long int r13;
	__extension__ unsigned long long int r12;
	__extension__ unsigned long long int rbp;
	__extension__ unsigned long long int rbx;
	__extension__ unsigned long long int r11;
	__extension__ unsigned long long int r10;
	__extension__ unsigned long long int r9;
	__extension__ unsigned long long int r8;
	__extension__ unsigned long long int rax;
	__extension__ unsigned long long int rcx;
	__extension__ unsigned long long int rdx;
	__extension__ unsigned long long int rsi;
	__extension__ unsigned long long int rdi;
	__extension__ unsigned long long int orig_rax;
	__extension__ unsigned long long int rip;
	__extension__ unsigned long long int cs;
	__extension__ unsigned long long int eflags;
	__extension__ unsigned long long int rsp;
	__extension__ unsigned long long int ss;
	__extension__ unsigned long long int fs_base;
	__extension__ unsigned long long int gs_base;
	__extension__ unsigned long long int ds;
	__extension__ unsigned long long int es;
	__extension__ unsigned long long int fs;
	__extension__ unsigned long long int gs;
}	t_user_regs_struct_64;

typedef struct
{
	long int ebx;
	long int ecx;
	long int edx;
	long int esi;
	long int edi;
	long int ebp;
	long int eax;
	long int xds;
	long int xes;
	long int xfs;
	long int xgs;
	long int orig_eax;
	long int eip;
	long int xcs;
	long int eflags;
	long int esp;
	long int xss;
}	t_user_regs_struct_32;

typedef struct {
	unsigned long long int	syscall;
	unsigned long long int	rax;
	unsigned long long int	regs[6];
}	t_user_regs;

t_user_regs_struct_32	convert_to_32(const t_user_regs_struct_64 *in);
t_user_regs				convert_to_regs(const t_user_regs_struct_64 *in);

#endif //FT_STRACE_USER_REGS_STRUCTS_H
