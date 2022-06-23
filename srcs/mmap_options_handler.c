//
// Created by Peer de Bakker on 6/23/22.
//

#include <stdio.h>
#include <stdbool.h>
#include <sys/mman.h>

typedef struct s_mmap_option {
	unsigned int	value;
	const char		*name;
}	t_mmap_option;

static const t_mmap_option mmap_prots[] = {
		{PROT_READ, "PROT_READ"},
		{PROT_WRITE, "PROT_WRITE"},
		{PROT_EXEC, "PROT_EXEC"},
		{PROT_NONE, "PROT_NONE"},
		{PROT_GROWSDOWN, "PROT_GROWSDOWN"},
		{PROT_GROWSUP, "PROT_GROWSUP"},
		{-1, NULL}
};

static const t_mmap_option mmap_flags[] = {
//		{MAP_FILE, "MAP_FILE"},
//		{MAP_SHARED, "MAP_SHARED"},
//		{MAP_PRIVATE, "MAP_PRIVATE"},
//		{MAP_SHARED_VALIDATE, "MAP_SHARED_VALIDATE"},
		{MAP_FIXED, "MAP_FIXED"},
		{MAP_ANONYMOUS, "MAP_ANONYMOUS"},
		{MAP_32BIT, "MAP_32BIT"},
		{MAP_GROWSDOWN, "MAP_GROWSDOWN"},
		{MAP_DENYWRITE, "MAP_DENYWRITE"},
		{MAP_EXECUTABLE, "MAP_EXECUTABLE"},
		{MAP_LOCKED, "MAP_LOCKED"},
		{MAP_NORESERVE, "MAP_NORESERVE"},
		{MAP_POPULATE, "MAP_POPULATE"},
		{MAP_NONBLOCK, "MAP_NONBLOCK"},
		{MAP_STACK, "MAP_STACK"},
		{MAP_HUGETLB, "MAP_HUGETLB"},
		{MAP_SYNC, "MAP_SYNC"},
		{MAP_FIXED_NOREPLACE, "MAP_FIXED_NOREPLACE"},
		{-1, NULL}
};

void	mmap_handle_option(const unsigned long long regval, const t_mmap_option *options, bool first_print) {
	for (size_t i = 0; options[i].name; i++) {
		const t_mmap_option option = options[i];
		if ((unsigned int)regval & option.value) {
			if (!first_print) {
				fprintf(stderr, "|");
			}
			fprintf(stderr, "%s", option.name);
			first_print = false;
		}
	}
}

void	mmap_handle_prots(const unsigned long long regval) {
	mmap_handle_option(regval, mmap_prots, true);
}

void	mmap_sharing_type(const unsigned long long int regval) {
	/* Sharing types (must choose one and only one of these).  */
	if (regval & MAP_SHARED && regval & MAP_PRIVATE)
		fprintf(stderr, "%s", "MAP_SHARED_VALIDATE");
	else if (regval & MAP_SHARED)
		fprintf(stderr, "%s", "MAP_SHARED");
	else if (regval & MAP_PRIVATE)
		fprintf(stderr, "%s", "MAP_PRIVATE");
	else
		fprintf(stderr, "%s", "MAP_FILE");
}

void	mmap_handle_flags(const unsigned long long regval) {
	mmap_sharing_type(regval);
	if (regval == MAP_SHARED_VALIDATE)
		fprintf(stderr, "%s", mmap_flags[MAP_SHARED_VALIDATE].name);
	mmap_handle_option(regval, mmap_flags, false);
}