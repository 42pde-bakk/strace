//
// Created by peerdb on 29-6-22.
//

#include "strace.h"

unsigned int parse_flags(int argc, char **argv, int *error) {
	unsigned int flags = 0u;

	for (int i = 1; i < argc && argv[i]; i++) {
		if (argv[i][0] != '-') {
			break ;
		}
		for (unsigned int i2 = 1; argv[i][i2]; i2++) {
			switch (argv[i][i2]) {
				case FLAG_SUMMARY_CHAR:
					flags |= FLAG_SUMMARY_VALUE;
					break ;
				case FLAG_OUTPUT_REDIR_CHAR:
					flags |= FLAG_OUTPUT_REDIR_VALUE;
					break ;
				default:
					*error = 1;
			}
		}
	}
	return (flags);
}
