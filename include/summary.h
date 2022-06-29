//
// Created by peerdb on 29-6-22.
//

#ifndef FT_STRACE_SUMMARY_H
#define FT_STRACE_SUMMARY_H

#include <stddef.h>

typedef struct s_summary {
	double	seconds;
	size_t	calls;
	size_t	errors;
	size_t	syscallNb;
}	t_summary;

#endif //FT_STRACE_SUMMARY_H
