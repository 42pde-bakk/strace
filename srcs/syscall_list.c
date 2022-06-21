//
// Created by Peer de Bakker on 6/20/22.
//

#include "syscalls.h"

t_syscall syscalls[] = {
		{.rax = 0, .return_value = SSIZE_T, .name = "read", INT, VOID_POINTER, SIZE_T, NONE, NONE, NONE},
		{.rax = 1, .return_value = SSIZE_T, .name = "write", INT, VOID_POINTER, SIZE_T, NONE, NONE, NONE},
		{.rax = 2, .return_value = INT, .name = "open", STRING, INT, NONE, NONE, NONE, NONE},
		{.rax = 3, .return_value = INT, .name = "close", INT, NONE, NONE, NONE, NONE, NONE},
		{.rax = 4, .return_value = INT, .name = "stat", STRING, POINTER, NONE, NONE, NONE, NONE},
		{.rax = 5, .return_value = INT, .name = "fstat", INT, POINTER, NONE, NONE, NONE, NONE},
		{.rax = 6, .return_value = INT, .name = "lstat", STRING, POINTER, NONE, NONE, NONE, NONE},
		{.rax = 7, .return_value = INT, .name = "poll", POINTER, STRUCT, POINTER, POINTER, NONE, NONE},
		{.rax = 8, .return_value = INT, .name = "lseek", INT, INT, INT, NONE, NONE, NONE},
		{.rax = 9, .return_value = VOID_POINTER, .name = "mmap", VOID_POINTER, SIZE_T, INT, INT, INT, INT}
};
