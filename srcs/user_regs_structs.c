//
// Created by Peer de Bakker on 7/4/22.
//
#include <string.h>
#include "user_regs_structs.h"
#include "syscalls.h"

t_user_regs_struct_32	convert_to_32(const t_user_regs_struct_64 *in) {
	t_user_regs_struct_32 out;

	bzero(&out, sizeof(out));
	memcpy(&out, &in, sizeof(out));
	return (out);
}
typedef unsigned long long int UINT64;

t_user_regs convert_32_to_regs(const t_user_regs_struct_32 *in) {
	const t_user_regs out = {
			.syscall = (UINT64)in->orig_eax,
			.rax = (UINT64)in->eax,
//			.regs = {in->edi, in->esi, in->edx, in->ecx, in->ebx, in->eax}
			.regs = {(UINT64)in->ecx, (UINT64)in->edx, (UINT64)in->esi, (UINT64)in->edi, (UINT64)in->ebp, (UINT64)in->eax}
	};
	return (out);
}

t_user_regs convert_to_regs(const t_user_regs_struct_64 *in) {
	if (arch == ARCH_32) {
		t_user_regs_struct_32 regs32 = convert_to_32(in);
		return (convert_32_to_regs(&regs32));
	}
	const t_user_regs out = {
			.syscall = in->orig_rax,
			.rax = in->rax,
			.regs = {in->rdi, in->rsi, in->rdx, in->r10, in->r8, in->r9}
	};
	return (out);
}

