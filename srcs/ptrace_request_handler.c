#include <sys/ptrace.h>
#include <stdio.h>

void	enum__ptrace_request_handler(const unsigned long long int regval) {
	switch (regval) {
		case PTRACE_TRACEME:
			fprintf(stderr, "%s", "PTRACE_TRACEME");
			break;
		case PTRACE_PEEKTEXT:
			fprintf(stderr, "%s", "PTRACE_PEEKTEXT");
			break;
		case PTRACE_PEEKDATA:
			fprintf(stderr, "%s", "PTRACE_PEEKDATA");
			break;
		case PTRACE_PEEKUSER:
			fprintf(stderr, "%s", "PTRACE_PEEKUSER");
			break;
		case PTRACE_POKETEXT:
			fprintf(stderr, "%s", "PTRACE_POKETEXT");
			break;
		case PTRACE_POKEDATA:
			fprintf(stderr, "%s", "PTRACE_POKEDATA");
			break;
		case PTRACE_POKEUSER:
			fprintf(stderr, "%s", "PTRACE_POKEUSER");
			break;
		case PTRACE_CONT:
			fprintf(stderr, "%s", "PTRACE_CONT");
			break;
		case PTRACE_KILL:
			fprintf(stderr, "%s", "PTRACE_KILL");
			break;
		case PTRACE_SINGLESTEP:
			fprintf(stderr, "%s", "PTRACE_SINGLESTEP");
			break;
		case PTRACE_GETREGS:
			fprintf(stderr, "%s", "PTRACE_GETREGS");
			break;
		case PTRACE_SETREGS:
			fprintf(stderr, "%s", "PTRACE_SETREGS");
			break;
		case PTRACE_GETFPREGS:
			fprintf(stderr, "%s", "PTRACE_GETFPREGS");
			break;
		case PTRACE_SETFPREGS:
			fprintf(stderr, "%s", "PTRACE_SETFPREGS");
			break;
		case PTRACE_ATTACH:
			fprintf(stderr, "%s", "PTRACE_ATTACH");
			break;
		case PTRACE_DETACH:
			fprintf(stderr, "%s", "PTRACE_DETACH");
			break;
		case PTRACE_GETFPXREGS:
			fprintf(stderr, "%s", "PTRACE_GETFPXREGS");
			break;
		case PTRACE_SETFPXREGS:
			fprintf(stderr, "%s", "PTRACE_SETFPXREGS");
			break;
		case PTRACE_SYSCALL:
			fprintf(stderr, "%s", "PTRACE_SYSCALL");
			break;
		case PTRACE_GET_THREAD_AREA:
			fprintf(stderr, "%s", "PTRACE_GET_THREAD_AREA");
			break;
		case PTRACE_SET_THREAD_AREA:
			fprintf(stderr, "%s", "PTRACE_SET_THREAD_AREA");
			break;
		case PTRACE_ARCH_PRCTL:
			fprintf(stderr, "%s", "PTRACE_ARCH_PRCTL");
			break;
		case PTRACE_SYSEMU:
			fprintf(stderr, "%s", "PTRACE_SYSEMU");
			break;
		case PTRACE_SYSEMU_SINGLESTEP:
			fprintf(stderr, "%s", "PTRACE_SYSEMU_SINGLESTEP");
			break;
		case PTRACE_SINGLEBLOCK:
			fprintf(stderr, "%s", "PTRACE_SINGLEBLOCK");
			break;
		case PTRACE_SETOPTIONS:
			fprintf(stderr, "%s", "PTRACE_SETOPTIONS");
			break;
		case PTRACE_GETEVENTMSG:
			fprintf(stderr, "%s", "PTRACE_GETEVENTMSG");
			break;
		case PTRACE_GETSIGINFO:
			fprintf(stderr, "%s", "PTRACE_GETSIGINFO");
			break;
		case PTRACE_SETSIGINFO:
			fprintf(stderr, "%s", "PTRACE_SETSIGINFO");
			break;
		case PTRACE_GETREGSET:
			fprintf(stderr, "%s", "PTRACE_GETREGSET");
			break;
		case PTRACE_SETREGSET:
			fprintf(stderr, "%s", "PTRACE_SETREGSET");
			break;
		case PTRACE_SEIZE:
			fprintf(stderr, "%s", "PTRACE_SEIZE");
			break;
		case PTRACE_INTERRUPT:
			fprintf(stderr, "%s", "PTRACE_INTERRUPT");
			break;
		case PTRACE_LISTEN:
			fprintf(stderr, "%s", "PTRACE_LISTEN");
			break;
		case PTRACE_PEEKSIGINFO:
			fprintf(stderr, "%s", "PTRACE_PEEKSIGINFO");
			break;
		case PTRACE_GETSIGMASK:
			fprintf(stderr, "%s", "PTRACE_GETSIGMASK");
			break;
		case PTRACE_SETSIGMASK:
			fprintf(stderr, "%s", "PTRACE_SETSIGMASK");
			break;
		case PTRACE_SECCOMP_GET_FILTER:
			fprintf(stderr, "%s", "PTRACE_SECCOMP_GET_FILTER");
			break;
		case PTRACE_SECCOMP_GET_METADATA:
			fprintf(stderr, "%s", "PTRACE_SECCOMP_GET_METADATA");
			break;
		case PTRACE_GET_SYSCALL_INFO:
			fprintf(stderr, "%s", "PTRACE_GET_SYSCALL_INFO");
			break;
		default:
			fprintf(stderr, "%llu", regval);
			break;
	}
}
