//
// Created by Peer de Bakker on 7/4/22.
//

#include <elf.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "syscalls.h"
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

e_arch cleanup(char *fileptr, size_t len, int fd, e_arch ret) {
	if (fd) {
		close(fd);
	}
	if (fileptr) {
		munmap(fileptr, len);
	}
	return (ret);
}

e_arch get_architecture(const char *filename) {
	int fd = -1;
	struct stat buf;
	char *fileptr = NULL;

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		return (INVALID);
	}
	bzero(&buf, sizeof(buf));
	if (fstat(fd, &buf) == -1 || buf.st_size <= 0 || S_ISDIR(buf.st_mode)) {
		return (cleanup(fileptr, buf.st_size, fd, INVALID));
	}
	fileptr = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (fileptr == MAP_FAILED) {
		return (cleanup(fileptr, buf.st_size, fd, INVALID));
	}
	if (strncmp(fileptr, ELFMAG, SELFMAG) == 0) {
		if (fileptr[EI_CLASS] == ELFCLASS32) {
			arch = ARCH_32;
			max_syscall_nb = MAX_SYSCALL_NB_32;
			syscalls = syscalls_32;
			fprintf(stderr, "its a 32 bits one!\n");
			return (cleanup(fileptr, buf.st_size, fd, ARCH_32));
		}
		else if (fileptr[EI_CLASS] == ELFCLASS64) {
			arch = ARCH_64;
			max_syscall_nb = MAX_SYSCALL_NB_64;
			syscalls = syscalls_64;
			return (cleanup(fileptr, buf.st_size, fd, ARCH_64));
		}
	}
	return (cleanup(fileptr, buf.st_size, fd, INVALID));
}
