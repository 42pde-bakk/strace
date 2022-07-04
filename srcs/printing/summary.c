//
// Created by peerdb on 29-6-22.
//

#include "syscalls.h"
#include <string.h>
#include <stdio.h>
#include <locale.h>

typedef enum {
	TIME_PERCENTAGE = 0,
	TOTALSECONDS = 1,
	USECS_PER_CALL = 2,
	CALLS = 3,
	ERRORS = 4,
	SYSCALL_NAME = 5
}	e_column;

static const char *sep = "------------------------------------------------";
static const char *headers[] = {
		"% time", "seconds", "usecs/call", "calls", "errors", "syscall"
};
static const char *footers[] = {
		"100.00", "total"
};
static size_t	column_widths[] = {
		6, 11, 11, 9, 9, 7
};

static size_t	max(const size_t a, const size_t b) {
	return ((a > b) ? a : b);
}

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
			column_widths[SYSCALL_NAME] = max(column_widths[SYSCALL_NAME], strlen(syscalls[i].name));
			n++;
		}
	}
}

static void	print_header() {
	fprintf(stderr, "%*s %*s %*s %*s %*s %-*s\n",
			(int)column_widths[TIME_PERCENTAGE], headers[0],
			(int)column_widths[TOTALSECONDS], headers[1],
			(int)column_widths[USECS_PER_CALL], headers[2],
			(int)column_widths[CALLS], headers[3],
			(int)column_widths[ERRORS], headers[4],
			(int)column_widths[SYSCALL_NAME], headers[5]
	);
	fprintf(stderr, "%.*s %.*s %.*s %.*s %.*s %.*s\n",
			(int)column_widths[TIME_PERCENTAGE], sep,
			(int)column_widths[TOTALSECONDS], sep,
			(int)column_widths[USECS_PER_CALL], sep,
			(int)column_widths[CALLS], sep,
			(int)column_widths[ERRORS], sep,
			(int)column_widths[SYSCALL_NAME], sep
	);
}

static void	print_summary_syscall(const t_summary *summary, const double total_seconds) {
	fprintf(stderr, "%*.*f %*f %*zu %*zu %*.0zu %-*s\n",
			(int)column_widths[TIME_PERCENTAGE], 2, summary->seconds / total_seconds * 100,
			(int)column_widths[TOTALSECONDS], summary->seconds,
			(int)column_widths[USECS_PER_CALL], (size_t)(summary->seconds * 1000000 / (double)summary->calls),
			(int)column_widths[CALLS], summary->calls,
			(int)column_widths[ERRORS], summary->errors ,
			(int)column_widths[SYSCALL_NAME], syscalls[summary->syscallNb].name
	);
}

static double	get_total_seconds(t_summary *summaries[]) {
	double	total_seconds = 0;

	for (size_t i = 0; summaries[i]; i++) {
		total_seconds += summaries[i]->seconds;
	}
	return (total_seconds);
}

static void	print_footer(t_summary *summaries[]) {
	size_t	total_calls = 0;
	size_t	total_errors = 0;
	size_t	avg_usecs_per_call;
	double	total_seconds = 0;

	for (size_t i = 0; summaries[i]; i++) {
		total_calls += summaries[i]->calls;
		total_errors += summaries[i]->errors;
		total_seconds += summaries[i]->seconds;
	}
	avg_usecs_per_call = (size_t)(total_seconds * 1000000 / (double)total_calls + 0.5f);


	fprintf(stderr, "%.*s %.*s %.*s %.*s %.*s %.*s\n",
			(int)column_widths[TIME_PERCENTAGE], sep,
			(int)column_widths[TOTALSECONDS], sep,
			(int)column_widths[USECS_PER_CALL], sep,
			(int)column_widths[CALLS], sep,
			(int)column_widths[ERRORS], sep,
			(int)column_widths[SYSCALL_NAME], sep
	);
	fprintf(stderr, "%.*s %*f %*d %*zu %*zu %.*s\n",
			(int)column_widths[TIME_PERCENTAGE], footers[0],
			(int)column_widths[TOTALSECONDS], total_seconds,
			(int)column_widths[USECS_PER_CALL], (int)avg_usecs_per_call,
			(int)column_widths[CALLS], total_calls,
			(int)column_widths[ERRORS], total_errors,
			(int)column_widths[SYSCALL_NAME], footers[1]
	);
}

static void	init_summary_structs() {
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

	init_summary_structs();

	bzero(summaries, sizeof(t_summary *) * (amount + 1));
	populate_list(summaries);
	total_seconds = get_total_seconds(summaries);

	sort_summaries(summaries, amount);

	setlocale(LC_NUMERIC, "");
	print_header();
	for (size_t i = 0; summaries[i]; i++) {
		print_summary_syscall(summaries[i], total_seconds);
	}
	print_footer(summaries);
}
