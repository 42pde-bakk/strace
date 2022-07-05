//
// Created by Peer de Bakker on 7/1/22.
//

#include "strace.h"
#include "syscalls.h"
#include <signal.h>

static const t_signal	g_sig[] = {
				{ 1, "SIGHUP"},
				{ 2, "SIGINT"},
				{ 3, "SIGQUIT"},
				{ 4, "SIGILL"},
				{ 5, "SIGTRAP"},
				{ 6, "SIGABRT"},
				{ 7, "SIGBUS"},
				{ 8, "SIGFPE"},
				{ 9, "SIGKILL"},
				{ 10, "SIGUSR1"},
				{ 11, "SIGSEGV"},
				{ 12, "SIGUSR2"},
				{ 13, "SIGPIPE"},
				{ 14, "SIGALRM"},
				{ 15, "SIGTERM"},
				{ 16, "SIGSTKFLT"},
				{ 17, "SIGCHLD"},
				{ 18, "SIGCONT"},
				{ 19, "SIGSTOP"},
				{ 20, "SIGTSTP"},
				{ 21, "SIGTTIN"},
				{ 22, "SIGTTOU"},
				{ 23, "SIGURG"},
				{ 24, "SIGXCPU"},
				{ 25, "SIGXFSZ"},
				{ 26, "SIGVTALRM"},
				{ 27, "SIGPROF"},
				{ 28, "SIGWINCH"},
				{ 29, "SIGIO"},
				{ 30, "SIGPWR"},
				{ 31, "SIGSYS"},
				{ 34, "SIGRTMIN"},
				{ 35, "SIGRTMIN+1"},
				{ 36, "SIGRTMIN+2"},
				{ 37, "SIGRTMIN+3"},
				{ 38, "SIGRTMIN+4"},
				{ 39, "SIGRTMIN+5"},
				{ 40, "SIGRTMIN+6"},
				{ 41, "SIGRTMIN+7"},
				{ 42, "SIGRTMIN+8"},
				{ 43, "SIGRTMIN+9"},
				{ 44, "SIGRTMIN+10"},
				{ 45, "SIGRTMIN+11"},
				{ 46, "SIGRTMIN+12"},
				{ 47, "SIGRTMIN+13"},
				{ 48, "SIGRTMIN+14"},
				{ 49, "SIGRTMIN+15"},
				{ 50, "SIGRTMAX-14"},
				{ 51, "SIGRTMAX-13"},
				{ 52, "SIGRTMAX-12"},
				{ 53, "SIGRTMAX-11"},
				{ 54, "SIGRTMAX-10"},
				{ 55, "SIGRTMAX-9"},
				{ 56, "SIGRTMAX-8"},
				{ 57, "SIGRTMAX-7"},
				{ 58, "SIGRTMAX-6"},
				{ 59, "SIGRTMAX-5"},
				{ 60, "SIGRTMAX-4"},
				{ 61, "SIGRTMAX-3"},
				{ 62, "SIGRTMAX-2"},
				{ 63, "SIGRTMAX-1"},
				{ 64, "SIGRTMAX"},
				{ 65, NULL}
};

static const t_errorsignal error_signals[] = {
		{.nb = SIGILL, .name = "SIGILL", .exit_status = 132, .exit_msg = "Ended by SIGILL\n"},
		{.nb = SIGTRAP, .name = "SIGTRAP", .exit_status = 133, .exit_msg = "Ended by SIGTRAP\n"},
		{.nb = SIGABRT, .name = "SIGABRT", .exit_status = 134, .exit_msg = "Ended by SIGABRT\n"},
		{.nb = SIGFPE, .name = "SIGFPE", .exit_status = 136, .exit_msg = "Ended by SIGFPE\n"},
		{.nb = SIGBUS, .name = "SIGBUS", .exit_status = 138, .exit_msg = "Bus error\n"},
		{.nb = SIGSEGV, .name = "SIGSEGV", .exit_status = 139, .exit_msg = "Segmentation fault\n"},
		{.nb = -1, .name = NULL, .exit_status = -1, .exit_msg = NULL}
};

static const t_signal SIGILL_codes[] = {
		{ILL_ILLOPC, "ILL_ILLOPC"},
		{ILL_ILLOPN, "ILL_ILLOPN"},
		{ILL_ILLADR, "ILL_ILLADR"},
		{ILL_ILLTRP, "ILL_ILLTRP"},
		{ILL_PRVOPC, "ILL_PRVOPC"},
		{ILL_PRVREG, "ILL_PRVREG"},
		{ILL_COPROC, "ILL_COPROC"},
		{ILL_BADSTK, "ILL_BADSTK"},
		{-1, NULL}
};

static const t_signal SIGFPE_codes[] = {
		{FPE_INTDIV, "FPE_INTDIV"},
		{FPE_INTOVF, "FPE_INTOVF"},
		{FPE_FLTDIV, "FPE_FLTDIV"},
		{FPE_FLTOVF, "FPE_FLTOVF"},
		{FPE_FLTUND, "FPE_FLTUND"},
		{FPE_FLTRES, "FPE_FLTRES"},
		{FPE_FLTINV, "FPE_FLTINV"},
		{FPE_FLTSUB, "FPE_FLTSUB"},
		{-1, NULL}
};

static const t_signal SIGSEGV_codes[] = {
		{SEGV_MAPERR, "SEGV_MAPERR"},
		{SEGV_ACCERR, "SEGV_ACCERR"},
		{-1, NULL}

};

static const t_signal SIGBUS_codes[] = {
		{BUS_ADRALN, "BUS_ADRALN"},
		{BUS_ADRERR, "BUS_ADRERR"},
		{BUS_OBJERR, "BUS_OBJERR"},
		{3, "BUS_OOMERR"},
		{-1, NULL}
};

static const t_signal SIGTRAP_codes[] = {
		{0, "TRAP_BRKPT"},
		{1, "BUS_ADRERR"},
		{2, "BUS_OBJERR"},
		{3, "BUS_OOMERR"},
		{-1, NULL}
};

static const t_signal SIGCHLD_codes[] = {
		{CLD_EXITED, "CLD_EXITED"},
		{CLD_KILLED, "CLD_KILLED"},
		{CLD_DUMPED, "CLD_DUMPED"},
		{CLD_TRAPPED, "CLD_TRAPPED"},
		{CLD_STOPPED, "CLD_STOPPED"},
		{CLD_CONTINUED, "CLD_CONTINUED"},
		{-1, NULL}
};

static const t_signal SIGPOLL_codes[] = {
		{POLL_IN, "POLL_IN"},
		{POLL_OUT, "POLL_OUT"},
		{POLL_MSG, "POLL_MSG"},
		{POLL_ERR, "POLL_ERR"},
		{POLL_PRI, "POLL_PRI"},
		{POLL_HUP, "POLL_HUP"},
		{6, "SI_NOINFO"},
		{-1, NULL}
};

const char	*get_signal_name(const int nb) {
	for (int i = 0; g_sig[i].name; i++) {
		if (nb == g_sig[i].nb) {
			return (g_sig[i].name);
		}
	}
	return (NULL);
}

const char	*get_signal_code_internal(const t_signal codes[], int code) {
	for (int i = 0; codes[i].name; i++) {
		if (code == codes[i].nb) {
			return (codes[i].name);
		}
	}
	return ("SI_NOINFO");
}

const char *get_signal_code(const int signal_nb, const int signal_code) {
	switch (signal_nb) {
		case SIGILL:
			return get_signal_code_internal(SIGILL_codes, signal_code);
		case SIGFPE:
			return get_signal_code_internal(SIGFPE_codes, signal_code);
		case SIGSEGV:
			return get_signal_code_internal(SIGSEGV_codes, signal_code);
		case SIGBUS:
			return get_signal_code_internal(SIGBUS_codes, signal_code);
		case SIGTRAP:
			return get_signal_code_internal(SIGTRAP_codes, signal_code);
		case SIGCHLD:
			return get_signal_code_internal(SIGCHLD_codes, signal_code);
		case SIGPOLL:
			return get_signal_code_internal(SIGPOLL_codes, signal_code);
		default:
			return ("SI_NOINFO2");
	}
}

const t_errorsignal	*get_error_signal(const int signal_nb) {
	for (unsigned int i = 0; error_signals[i].name; i++) {
		if (error_signals[i].nb == signal_nb) {
			return (&error_signals[i]);
		}
	}
	return (NULL);
}
