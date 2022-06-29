//
// Created by peerdb on 29-6-22.
//

#include "syscalls.h"
#include <string.h>
#include <stdio.h>

static size_t	count_used_syscalls() {
	size_t amount = 0;

	for (size_t i = 0; i <= MAX_SYSCALL_NB; i++) {
		if (syscalls[i].summary.calls > 0)
			amount++;
	}
	return (amount);
}

static void	populate_list(t_summary *summaries[]) {
	size_t n = 0;
	for (size_t i = 0; i < MAX_SYSCALL_NB + 1; i++) {
		if (syscalls[i].summary.calls > 0) {
			summaries[n] = &syscalls[i].summary;
			n++;
		}
	}
}

void	print_header() {
	fprintf(stderr, "%% time\tseconds\tusecs/call\tcalls\terrors\tsyscalls\n");
	fprintf(stderr, "------ ----------- ----------- --------- --------- ----------------\n");
}

void	print_summary_syscall(const t_summary *summary, const double total_seconds) {
	fprintf(stderr, "%f\t%f\t%zu\t%zu\t%zu\t%s\n",
			summary->seconds / total_seconds * 100,
			summary->seconds,
			(size_t)(summary->seconds * 100000 / summary->calls),
			summary->calls,
			summary->errors,
			syscalls[summary->syscallNb].name);
}

double	get_total_seconds(t_summary *summaries[]) {
	double	total_seconds = 0;

	for (size_t i = 0; summaries[i]; i++) {
		total_seconds += summaries[i]->seconds;
	}
	return (total_seconds);
}

void	print_footer(t_summary *summaries[]) {
	size_t	total_calls = 0;
	size_t	total_errors = 0;
	double	total_seconds = 0;

	for (size_t i = 0; summaries[i]; i++) {
		total_calls += summaries[i]->calls;
		total_errors += summaries[i]->errors;
		total_seconds += summaries[i]->seconds;
	}

	fprintf(stderr, "------ ----------- ----------- --------- --------- ----------------\n");
	fprintf(stderr, "100.00\t%f\t\t%zu\t%zu\ttotal\n", total_seconds, total_calls, total_errors);
}

void	init_summary_structs() {
	static int init = 0;

	if (!init) {
		for (size_t i = 0; i <= MAX_SYSCALL_NB; i++) {
			syscalls[i].summary.syscallNb = syscalls[i].nb64;
		}
		init = 1;
	}
}

void	print_summary() {
	const size_t	amount = count_used_syscalls();
	double	total_seconds;
	t_summary *summaries[amount + 1];

	fprintf(stderr, "amount of used syscalls = %zu\n", amount);
	init_summary_structs();

	bzero(summaries, sizeof(t_summary *) * (amount + 1));
	populate_list(summaries);
	total_seconds = get_total_seconds(summaries);

	print_header();
	for (size_t i = 0; summaries[i]; i++) {
//		fprintf(stderr, "summaries[%zu] = %p\n", i, (void *)summaries[i]);
		print_summary_syscall(summaries[i], total_seconds);
	}
	print_footer(summaries);
}
